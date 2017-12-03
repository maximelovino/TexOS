#include "fs_create.h"

void display_usage() {
	printf("fs_create <label> <block_size> <block_count> <max_file_count> <image_name>\n");
}

int main(int argc, char* argv[]) {
	if (argc != 6) {
		display_usage();
		return EXIT_FAILURE;
	}
	printf("Welcome to the creation of your TexFS image\n");
	printf("============================\n");

	char* label = argv[1];
	int16_t block_size = (int16_t) atoi(argv[2]);
	if (block_size < 0 || block_size % SECTOR_SIZE != 0) {
		printf("Block size must be a multiple of %d bytes, and be positive\n",
			   SECTOR_SIZE);
		return 1;
	}
	int16_t block_count = (int16_t) atoi(argv[3]);
	int16_t max_file_count = (int16_t) atoi(argv[4]);
	char* image_name = argv[5];
	printf("The selected parameters are:\nLabel: %s\nBlock Size: %d\nBlock count: %d\nFilename: %s\nMax File count: %d\n",
		   label, block_size, block_count, image_name, max_file_count);
	printf("============================\n");

	FILE* image_file = fopen(image_name, "wb");
	tex_fs_superblock_t superblock;
	superblock.block_size = block_size;
	memset(superblock.label, 0, 30);
	strcpy(superblock.label, label);
	superblock.block_map = 1;
	superblock.magic = TEX_FS_MAGIC;
	superblock.version = TEX_FS_VERSION;
	superblock.block_count = block_count;
	superblock.inode_count = max_file_count;


	int16_t block_count_block_map = (int16_t) (
			superblock.block_count / superblock.block_size + 1);
	uint8_t block_map[block_count];
	memset(block_map, 0, block_count);


	superblock.inode_list = superblock.block_map + block_count_block_map;

	int16_t block_count_inode_list = (int16_t) (
			(superblock.inode_count * sizeof(tex_fs_inode_t)) /
			superblock.block_size + 1);

	superblock.first_data_block =
			superblock.inode_list + block_count_inode_list;

	for (int i = 0; i < superblock.first_data_block; i++) {
		block_map[i] = 1;
	}
	tex_fs_inode_t inodes[superblock.inode_count];
	memset(inodes, 0, superblock.inode_count * sizeof(tex_fs_inode_t));

	fwrite(&superblock, sizeof(superblock), 1, image_file);
	fseek(image_file, superblock.block_map * superblock.block_size, SEEK_SET);
	fwrite(block_map, 1, block_count, image_file);
	fseek(image_file, superblock.inode_list * superblock.block_size, SEEK_SET);
	fwrite(inodes, sizeof(tex_fs_inode_t), superblock.inode_count, image_file);
	fseek(image_file, superblock.first_data_block * superblock.block_size,
		  SEEK_SET);

	print_superblock(&superblock);
	int data_size = (block_count - superblock.first_data_block) * block_size;
	void* data = calloc(data_size, 1);
	fwrite(data, 1, data_size, image_file);
	fclose(image_file);
	printf("File closed, your image %s is ready\n", image_name);
}