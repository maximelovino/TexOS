#include "tex_fs_tools.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


void print_usage() {
	printf("./fs_add <file_to_add> <image_file>\n");
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		print_usage();
		return EXIT_FAILURE;
	}
	char* filename = argv[1];
	char* image_name = argv[2];

	if (strlen(filename) > MAX_FILENAME_LENGTH) {
		printf("The name of the file you want to add is too long, maximum %d chars\n", MAX_FILENAME_LENGTH);
		return EXIT_FAILURE;
	}

	tex_fs_metadata_t fs;
	read_image(image_name, &fs);
	print_superblock(fs.superblock);

	FILE* image_file = fopen(image_name, "ab");
	FILE* file_to_add = fopen(filename, "rb");

	if (!file_to_add) {
		printf("The file you want to add doesn't exist\n");
		return EXIT_FAILURE;
	}

	if (!image_file) {
		printf("The image %s doesn't exist\n", image_name);
		return EXIT_FAILURE;
	}


	int free_inode_index = -1;

	for (uint32_t i = 0; i < fs.superblock->inode_count; i++) {
		if (!fs.inode_map[i]) {
			free_inode_index = i;
			break;
		}
	}

	if (free_inode_index == -1) {
		printf("Couldn't find an empty inode for your file\n");
		return EXIT_FAILURE;
	} else {
		printf("Found an inode for the file, inode number %d\n", free_inode_index);

	}

	tex_fs_inode_t* file_inode = &fs.inode_list[free_inode_index];


	fseek(file_to_add, 0, SEEK_END);
	uint32_t file_size = (uint32_t) ftell(file_to_add);
	rewind(file_to_add);

	if (file_size >= compute_max_file_size(fs.superblock)) {
		printf("File too big for this filesystem\n");
		return EXIT_FAILURE;
	}

	uint32_t data_blocks_needed = file_size / fs.superblock->block_size + 1;

	printf("You need %d blocks for the data of the file of size %d bytes\n",
		   data_blocks_needed,
		   file_size);

	uint32_t indirect_blocks_needed =
			((data_blocks_needed - DIRECT_BLOCKS) * BYTES_BLOCK_ADDRESS) / fs.superblock->block_size + 1;
	printf("For storing these blocks, you will need %d indirect blocks\n", indirect_blocks_needed);

	uint32_t blocks_to_write[data_blocks_needed + indirect_blocks_needed];
	uint32_t block_index = 0;
	for (uint32_t j = 0;
		 j < fs.superblock->block_count && block_index < (data_blocks_needed + indirect_blocks_needed); j++) {
		if (!fs.block_map[j]) {
			blocks_to_write[block_index++] = j;
		}
	}

	if (block_index < (data_blocks_needed + indirect_blocks_needed)) {
		printf("Not enough free blocks to allocate the file\n");
		return EXIT_FAILURE;
	} else {
		printf("Found enough blocks for the file %d\n", block_index);
	}

	strcpy(file_inode->name, filename);
	file_inode->size = file_size;
	//we take the first n blocks to be indirect blocks, so they will contain indexes
	//so here basically we take we split blocks_to_write like this: indirect_blocks / direct_data_blocks / indirect_data_blocks
	uint32_t* direct_data_blocks_indices = &blocks_to_write[indirect_blocks_needed];
	uint32_t* indirect_data_blocks_indices = &blocks_to_write[indirect_blocks_needed + DIRECT_BLOCKS];
	//TODO you will need to add to block map that all those blocks are used, and rewrite the block map
	//TODO in inode map, add the inode used, and rewrite the inodemap as well

	memset(file_inode->indirect_blocks, 0, INDIRECT_BLOCKS);
	memset(file_inode->direct_blocks, 0, DIRECT_BLOCKS);
	for (uint32_t i = 0; i < data_blocks_needed && i < DIRECT_BLOCKS; i++) {
		file_inode->direct_blocks[i] = direct_data_blocks_indices[i];
	}

	if (indirect_blocks_needed) {
		uint32_t indices_per_block = (uint32_t) fs.superblock->block_size / BYTES_BLOCK_ADDRESS;
		for (uint32_t i = 0; i < indirect_blocks_needed; i++) {
			file_inode->indirect_blocks[i] = blocks_to_write[i];
			uint32_t index_block[indices_per_block];
			memset(index_block, 0, indices_per_block);
			for (uint32_t j = 0; j < indices_per_block; j++) {
				index_block[j] = indirect_data_blocks_indices[i * indices_per_block + j];
			}
			//fseek(image_file, file_inode->indirect_blocks[i] * fs.superblock->block_size, SEEK_SET);
			//fwrite(index_block, sizeof(uint32_t), indices_per_block, image_file);
		}
	}

	fseek(image_file, fs.superblock->inode_list * fs.superblock->block_size + free_inode_index * sizeof(tex_fs_inode_t),
		  SEEK_SET);
	fwrite(file_inode, sizeof(tex_fs_inode_t), 1, image_file);
/*
	void* buffer = malloc(fs.superblock->block_size);
	if (!buffer) {
		exit(1);
	}

	for (uint32_t k = 0; k < data_blocks_needed; k++) {
		if (k < 8) {
			//put in direct block
		} else {
			//put in indirect block
		}
		//fread(buffer, fs.superblock->block_size, 1, file_to_add);
		printf("%d ", blocks_to_write[k]);
	}
	printf("\n");


	fseek(image_file,
		  fs.superblock->inode_list * fs.superblock->block_size +
		  sizeof(tex_fs_inode_t) * free_inode_index, SEEK_SET);
	fwrite(file_inode, sizeof(tex_fs_inode_t), 1, image_file);
	*/
	free_tex_fs_metadata(&fs);
	fclose(image_file);
	fclose(file_to_add);
	return 0;
}