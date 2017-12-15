#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../common/tex_fs.h"
#include <string.h>
#include "tex_fs_tools.h"

void display_usage() {
	printf("./fs_create <label> <block_size> <block_count> <max_file_count> <image_name>\n");
}

int main(int argc, char* argv[]) {
	if (argc != 6) {
		display_usage();
		return EXIT_FAILURE;
	}

	printf("Welcome to the creation of your TexFS image\n");
	printf("============================\n");

	char* label = argv[1];
	uint16_t block_size = (uint16_t) atoi(argv[2]);
	uint32_t block_count = (uint32_t) atoi(argv[3]);
	uint32_t max_file_count = (uint32_t) atoi(argv[4]);
	char* image_name = argv[5];


	if (block_size % SECTOR_SIZE != 0) {
		printf("Block size must be a multiple of %d bytes, and be positive\n", SECTOR_SIZE);
		return EXIT_FAILURE;
	}

	if (strlen(label) > MAX_LABEL_LENGTH) {
		printf("The name of the label you want to use is too long, maximum %d chars\n", MAX_LABEL_LENGTH);
		return EXIT_FAILURE;
	}

	printf("The selected parameters are:\nLabel: %s\nBlock Size: %d\nBlock count: %d\nFilename: %s\nMax File count: %d\n",
		   label, block_size, block_count, image_name, max_file_count);
	printf("============================\n");

	FILE* image_file = fopen(image_name, "wb");
	if (!image_file) {
		printf("Couldn't open the file %s for writing\n", image_name);
		return EXIT_FAILURE;
	}

	tex_fs_superblock_t superblock;
	superblock.block_size = block_size;
	memset(superblock.label, 0, 30);
	strcpy(superblock.label, label);
	superblock.block_map = 1;
	superblock.magic = TEX_FS_MAGIC;
	superblock.version = TEX_FS_VERSION;
	superblock.block_count = block_count;
	superblock.inode_count = max_file_count;


	uint32_t block_count_block_map = (uint32_t) (superblock.block_count / superblock.block_size + 1);
	uint8_t block_map[superblock.block_count];
	memset(block_map, 0, superblock.block_count);

	superblock.inode_bitmap = superblock.block_map + block_count_block_map;

	uint32_t block_count_inode_map = (uint32_t) (superblock.inode_count / superblock.block_size + 1);
	uint8_t inode_map[superblock.inode_count];
	memset(inode_map, 0, superblock.inode_count);

	superblock.inode_list = superblock.inode_bitmap + block_count_inode_map;


	uint32_t block_count_inode_list = (uint32_t) (
			(superblock.inode_count * sizeof(tex_fs_inode_t)) / superblock.block_size + 1);

	tex_fs_inode_t inodes[superblock.inode_count];
	memset(inodes, 0, superblock.inode_count * sizeof(tex_fs_inode_t));

	superblock.first_data_block = superblock.inode_list + block_count_inode_list;

	for (uint32_t i = 0; i < superblock.first_data_block; i++) {
		block_map[i] = 1;
	}
	print_superblock(&superblock);
	printf("Writing to file now...\n");

	fseek(image_file, 0, SEEK_SET);
	fwrite(&superblock, sizeof(superblock), 1, image_file);

	fseek(image_file, superblock.block_map * superblock.block_size, SEEK_SET);
	fwrite(block_map, 1, block_count, image_file);

	fseek(image_file, superblock.inode_bitmap * superblock.block_size, SEEK_SET);
	fwrite(inode_map, 1, max_file_count, image_file);

	fseek(image_file, superblock.inode_list * superblock.block_size, SEEK_SET);
	fwrite(inodes, sizeof(tex_fs_inode_t), superblock.inode_count, image_file);

	fseek(image_file, superblock.first_data_block * superblock.block_size, SEEK_SET);

	uint32_t remaining_blocks_padding = (superblock.block_count - superblock.first_data_block);
	printf("Remaining blocks to pad %d\n", remaining_blocks_padding);
	uint8_t data[superblock.block_size];
	memset(data, 0, superblock.block_size);
	for (uint32_t i = 0; i < remaining_blocks_padding; i++) {
		fwrite(data, 1, superblock.block_size, image_file);
	}

	fclose(image_file);
	printf("File closed, your image %s is ready\n", image_name);
	return 0;
}