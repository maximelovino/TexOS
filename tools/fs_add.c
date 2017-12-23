#include "tex_fs_tools.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>


void print_usage() {
	printf("./fs_add <file_to_add> <image_file>\n");
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		print_usage();
		return EXIT_FAILURE;
	}
	char* filepath = argv[1];
	char* image_name = argv[2];
	char* filename = basename(filepath);

	if (strlen(filename) > MAX_FILENAME_LENGTH) {
		printf("The name of the file you want to add is too long, maximum %d chars\n", MAX_FILENAME_LENGTH);
		return EXIT_FAILURE;
	}
	FILE* image_file = fopen(image_name, "r+b");

	if (!image_file) {
		printf("The image %s doesn't exist\n", image_name);
		return EXIT_FAILURE;
	}

	tex_fs_metadata_t fs;
	read_image(image_file, &fs);

	if (!valid_magic(&fs)) {
		printf("Magic is wrong, this is not a TexFS image\n");
		return EXIT_FAILURE;
	}

	print_superblock(fs.superblock);

	FILE* file_to_add = fopen(filepath, "rb");

	if (!file_to_add) {
		printf("The file you want to add doesn't exist\n");
		return EXIT_FAILURE;
	}

	if (is_file_already_present(filename, &fs)) {
		printf("The file %s you want to add is already present on the filesystem\n", filename);
		return EXIT_FAILURE;
	}


	int free_inode_index = find_free_inode_index(&fs);

	if (free_inode_index == -1) {
		printf("Couldn't find an empty inode for your file\n");
		return EXIT_FAILURE;
	} else {
		printf("Found an inode for the file, inode number %d\n", free_inode_index);

	}

	tex_fs_inode_t* file_inode = &fs.inode_list[free_inode_index];

	uint32_t file_size = get_size_of_file(file_to_add);

	if (file_size >= compute_max_file_size(fs.superblock)) {
		printf("File too big for this filesystem\n");
		return EXIT_FAILURE;
	}

	uint32_t data_blocks_needed = file_size / fs.superblock->block_size + (file_size % fs.superblock->block_size != 0);

	printf("You need %d blocks for the data of the file of size %d bytes\n", data_blocks_needed, file_size);

	uint32_t indirect_blocks_needed = 0;
	if (data_blocks_needed > DIRECT_BLOCKS) {
		//TODO check here with the +1 and all other blocks_needed (bitmaps, inode table etc)
		indirect_blocks_needed =
				((data_blocks_needed - DIRECT_BLOCKS) * BYTES_BLOCK_ADDRESS) / fs.superblock->block_size + 1;
	}

	printf("For storing these blocks, you will need %d indirect blocks\n", indirect_blocks_needed);

	uint32_t blocks_to_write[data_blocks_needed + indirect_blocks_needed];

	uint32_t blocks_found = find_blocks_for_file(blocks_to_write, fs.block_map, fs.superblock->block_count,
												 data_blocks_needed + indirect_blocks_needed);

	if (blocks_found < (data_blocks_needed + indirect_blocks_needed)) {
		printf("Not enough free blocks to allocate the file\n");
		return EXIT_FAILURE;
	} else {
		printf("Found enough blocks for the file %d\n", blocks_found);
	}

	strcpy(file_inode->name, filename);
	file_inode->size = file_size;
	//we take the first n blocks to be indirect blocks, so they will contain indexes
	//so here basically we take we split blocks_to_write like this: indirect_blocks / direct_data_blocks / indirect_data_blocks
	uint32_t* direct_data_blocks_indices = &blocks_to_write[indirect_blocks_needed];
	uint32_t* indirect_data_blocks_indices = &blocks_to_write[indirect_blocks_needed + DIRECT_BLOCKS];

	memset(file_inode->indirect_blocks, 0, INDIRECT_BLOCKS);
	memset(file_inode->direct_blocks, 0, DIRECT_BLOCKS);
	for (uint32_t i = 0; i < data_blocks_needed && i < DIRECT_BLOCKS; i++) {
		file_inode->direct_blocks[i] = direct_data_blocks_indices[i];
	}

	if (indirect_blocks_needed) {
		uint32_t indices_per_block = (uint32_t) fs.superblock->block_size / BYTES_BLOCK_ADDRESS;
		for (uint32_t i = 0; i < indirect_blocks_needed; i++) {
			file_inode->indirect_blocks[i] = blocks_to_write[i];
			fs.block_map[blocks_to_write[i]] = 1;
			uint32_t index_block[indices_per_block];
			memset(index_block, 0, indices_per_block);
			for (uint32_t j = 0; j < indices_per_block; j++) {
				index_block[j] = indirect_data_blocks_indices[i * indices_per_block + j];
			}
			seek_to_block(image_file, file_inode->indirect_blocks[i], fs.superblock->block_size);
			fwrite(index_block, sizeof(uint32_t), indices_per_block, image_file);
		}
	}

	seek_to_block(image_file, fs.superblock->inode_list, fs.superblock->block_size);
	fseek(image_file, free_inode_index * sizeof(tex_fs_inode_t), SEEK_CUR);
	fwrite(file_inode, sizeof(tex_fs_inode_t), 1, image_file);

	fs.inode_map[free_inode_index] = 1;
	write_bitmap_to_file(image_file, fs.inode_map, fs.superblock->inode_bitmap, fs.superblock->inode_count,
						 fs.superblock->block_size);

	uint8_t block[fs.superblock->block_size];
	uint32_t remaining_file_size = file_size;

	for (uint32_t k = 0; k < data_blocks_needed; k++) {
		memset(block, 0, fs.superblock->block_size);
		uint32_t size_to_read =
				remaining_file_size < fs.superblock->block_size ? remaining_file_size : fs.superblock->block_size;
		fread(block, 1, size_to_read, file_to_add);
		remaining_file_size -= size_to_read;
		seek_to_block(image_file, direct_data_blocks_indices[k], fs.superblock->block_size);
		fwrite(block, 1, fs.superblock->block_size, image_file);
		fs.block_map[direct_data_blocks_indices[k]] = 1;
	}

	write_bitmap_to_file(image_file, fs.block_map, fs.superblock->block_map, fs.superblock->block_count,
						 fs.superblock->block_size);

	printf("The inode of the newly created file is:\n");
	print_inode(file_inode);

	free_tex_fs_metadata(&fs);
	fclose(image_file);
	fclose(file_to_add);
	return 0;
}