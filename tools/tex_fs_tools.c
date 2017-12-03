#include "tex_fs_tools.h"

void read_superblock(FILE* image, tex_fs_superblock_t* superblock) {
	fseek(image, 0, SEEK_SET);
	fread(superblock, sizeof(tex_fs_superblock_t), 1, image);
}

void print_superblock(tex_fs_superblock_t* superblock) {
	printf("Superblock:\n");
	printf("Magic: %x\n", superblock->magic);
	printf("Version: %d\n", superblock->version);
	printf("Label: %s\n", superblock->label);
	printf("Block size: %d\n", superblock->block_size);
	printf("Block map: %d\n", superblock->block_map);
	printf("Block count: %d\n", superblock->block_count);
	printf("Inode list: %d\n", superblock->inode_list);
	printf("Inode count: %d\n", superblock->inode_count);
	printf("First data block: %d\n", superblock->first_data_block);
}

void read_image(char* filename, tex_fs_metadata_t* fs) {
	FILE* image = fopen(filename, "rb");
	fs->superblock = malloc(sizeof(tex_fs_superblock_t));
	read_superblock(image, fs->superblock);
	fs->block_map = malloc(sizeof(uint8_t) * fs->superblock->block_count);
	fseek(image, fs->superblock->block_map * fs->superblock->block_size,
		  SEEK_SET);
	fread(fs->block_map, 1, fs->superblock->block_count, image);
	fseek(image, fs->superblock->inode_list * fs->superblock->block_size,
		  SEEK_SET);
	fs->inode_list = malloc(
			sizeof(tex_fs_inode_t) * fs->superblock->inode_count);
	fread(fs->inode_list, sizeof(tex_fs_inode_t), fs->superblock->inode_count,
		  image);
	fclose(image);
}
