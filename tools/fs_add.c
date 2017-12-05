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
	tex_fs_metadata_t fs;
	read_image(image_name, &fs);
	print_superblock(fs.superblock);
	FILE* image_file = fopen(image_name, "ab");

	FILE* file_to_add = fopen(filename, "rb");
	if (!file_to_add) {
		printf("The file you want to add doesn't exist\n");
		return EXIT_FAILURE;
	}



	//TODO Don't forget the free

	int free_inode_index = -1;
	for (int i = 0; i < fs.superblock->inode_count; i++) {
		if (!fs.inode_map[i]) {
			free_inode_index = i;
			break;
		}
	}
	if (free_inode_index == -1) {
		printf("Couldn't find an empty inode for your file\n");
		return EXIT_FAILURE;
	}

	tex_fs_inode_t* file_inode = &fs.inode_list[free_inode_index];
	strcpy(file_inode->name, filename);

	fseek(file_to_add, 0, SEEK_END);
	int file_size = ftell(file_to_add);
	rewind(file_to_add);
	if (file_size >= DIRECT_BLOCKS * fs.superblock->block_size +
					 (fs.superblock->block_size / 2) *
					 fs.superblock->block_size) {
		printf("File too big for filesystem\n");
		return EXIT_FAILURE;
	}

	int blocks_needed_for_file = file_size / fs.superblock->block_size + 1;

	printf("You need %d blocks for the file of size %d\n",
		   blocks_needed_for_file,
		   file_size);

	uint16_t blocks_to_write[blocks_needed_for_file];
	int block_index;
	for (int j = 0; j < fs.superblock->block_count; j++) {
		if (!fs.block_map[j]) {
			blocks_to_write[block_index++] = j;
		}
	}

	if (block_index < (blocks_needed_for_file - 1)) {
		printf("Not enough free blocks to allocate the file\n");
		return EXIT_FAILURE;
	}

	file_inode->size = file_size;


	void* buffer = malloc(fs.superblock->block_size);

	for (int k = 0; k < blocks_needed_for_file; k++) {
		if (k < 8) {
			//put in direct block
		} else {
			//put in indirect block
		}
		fread(buffer, fs.superblock->block_size, 1, file_to_add);
	}


	fseek(image_file,
		  fs.superblock->inode_list * fs.superblock->block_size +
		  sizeof(tex_fs_inode_t) * free_inode_index, SEEK_SET);
	fwrite(file_inode, sizeof(tex_fs_inode_t), 1, image_file);

	free(fs.superblock);
	free(fs.inode_list);
	free(fs.block_map);
	free(fs.inode_map);
	fclose(image_file);
}