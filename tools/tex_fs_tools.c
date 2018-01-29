/**
 * Tex FS tools header file
 * @file 	tex_fs_tools.h
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	December 21, 2017
 */

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
	printf("Block map block number: %d\n", superblock->block_map);
	printf("Block count: %d\n", superblock->block_count);
	printf("Inode bitmap block number: %d\n", superblock->inode_bitmap);
	printf("Inode list block number: %d\n", superblock->inode_list);
	printf("Inode count: %d\n", superblock->inode_count);
	printf("First data block number: %d\n", superblock->first_data_block);
}

void read_image(FILE* image, tex_fs_metadata_t* fs) {
	fs->superblock = malloc(sizeof(tex_fs_superblock_t));
	if (!fs->superblock) {
		printf("Exiting because malloc failed\n");
		exit(EXIT_FAILURE);
	}
	read_superblock(image, fs->superblock);
	fs->block_map = malloc(sizeof(uint8_t) * fs->superblock->block_count);
	if (!fs->block_map) {
		printf("Exiting because malloc failed\n");
		exit(EXIT_FAILURE);
	}
	seek_to_block(image, fs->superblock->block_map, fs->superblock->block_size);
	fread(fs->block_map, 1, fs->superblock->block_count, image);

	seek_to_block(image, fs->superblock->inode_bitmap, fs->superblock->block_size);
	fs->inode_map = malloc(sizeof(uint8_t) * fs->superblock->inode_count);
	if (!fs->inode_map) {
		printf("Exiting because malloc failed\n");
		exit(EXIT_FAILURE);
	}
	fread(fs->inode_map, 1, fs->superblock->inode_count, image);
	seek_to_block(image, fs->superblock->inode_list, fs->superblock->block_size);
	fs->inode_list = malloc(
			sizeof(tex_fs_inode_t) * fs->superblock->inode_count);
	if (!fs->inode_list) {
		printf("Exiting because malloc failed\n");
		exit(EXIT_FAILURE);
	}
	fread(fs->inode_list, sizeof(tex_fs_inode_t), fs->superblock->inode_count,
		  image);
}

void free_tex_fs_metadata(tex_fs_metadata_t* fs) {
	free(fs->superblock);
	free(fs->block_map);
	free(fs->inode_map);
	free(fs->inode_list);
}

uint32_t compute_max_file_size(tex_fs_superblock_t* superblock) {
	return superblock->block_size * DIRECT_BLOCKS +
		   INDIRECT_BLOCKS * superblock->block_size / BYTES_BLOCK_ADDRESS * superblock->block_size;
}

uint32_t compute_free_blocks_count(tex_fs_metadata_t* fs) {
	uint32_t free_blocks_count = 0;
	for (uint32_t i = 0; i < fs->superblock->block_count; i++) {
		free_blocks_count += !fs->block_map[i];
	}
	return free_blocks_count;
}

uint32_t compute_free_inodes_count(tex_fs_metadata_t* fs) {
	uint32_t free_inodes_count = 0;
	for (uint32_t i = 0; i < fs->superblock->inode_count; i++) {
		free_inodes_count += !fs->inode_map[i];
	}
	return free_inodes_count;
}

void list_all_files(tex_fs_metadata_t* fs) {
	for (uint32_t i = 0; i < fs->superblock->inode_count; i++) {
		if (fs->inode_map[i]) {
			printf("%s\t%d bytes\n", fs->inode_list[i].name, fs->inode_list[i].size);
		}
	}
}

bool does_file_exist(char* filename, tex_fs_metadata_t* fs) {
	return find_inode_number_of_file(filename, fs) != -1;
}

int find_inode_number_of_file(char* filename, tex_fs_metadata_t* fs) {
	for (uint32_t i = 0; i < fs->superblock->inode_count; i++) {
		if (fs->inode_map[i] && strcmp(fs->inode_list[i].name, filename) == 0) {
			return i;
		}
	}
	return -1;
}

void free_all_blocks_for_file(tex_fs_inode_t* inode, tex_fs_metadata_t* fs, FILE* image) {
	uint32_t blocks_cleared = 0;
	uint32_t block_to_clear = 0;
	for (uint32_t i = 0; i < DIRECT_BLOCKS; i++) {
		block_to_clear = inode->direct_blocks[i];
		if (block_to_clear != 0) {
			blocks_cleared++;
			fs->block_map[block_to_clear] = 0;
		}
	}
	uint32_t block[fs->superblock->block_size / sizeof(uint32_t)];

	for (uint32_t i = 0; i < INDIRECT_BLOCKS; i++) {
		block_to_clear = inode->indirect_blocks[i];
		if (block_to_clear != 0) {
			blocks_cleared++;
			fs->block_map[block_to_clear] = 0;
			memset(block, 0, fs->superblock->block_size);
			seek_to_block(image, block_to_clear, fs->superblock->block_size);
			fread(block, 1, fs->superblock->block_size, image);

			for (uint32_t j = 0; j < (fs->superblock->block_size / (uint32_t) sizeof(uint32_t)); j++) {
				block_to_clear = block[j];
				if (block_to_clear != 0) {
					blocks_cleared++;
					fs->block_map[block_to_clear] = 0;
				}
			}
		}
	}
	printf("Cleared %d blocks\n", blocks_cleared);
}

void seek_to_block(FILE* file, uint32_t block_number, uint16_t block_size) {
	fseek(file, block_number * block_size, SEEK_SET);
}

void print_inode(tex_fs_inode_t* inode) {
	printf("Name: %s\n", inode->name);
	printf("Size: %d bytes\n", inode->size);
	printf("Direct Blocks: ");
	for (int i = 0; i < DIRECT_BLOCKS; i++) {
		printf("%d ", inode->direct_blocks[i]);
	}
	printf("\n");
	printf("Indirect blocks: ");
	for (int i = 0; i < INDIRECT_BLOCKS; i++) {
		printf("%d ", inode->indirect_blocks[i]);
	}
	printf("\n");
}

int find_free_inode_index(tex_fs_metadata_t* fs) {
	for (uint32_t i = 0; i < fs->superblock->inode_count; i++) {
		if (!fs->inode_map[i]) {
			return i;
		}
	}
	return -1;
}

uint32_t get_size_of_file(FILE* file) {
	fseek(file, 0, SEEK_END);
	uint32_t file_size = (uint32_t) ftell(file);
	rewind(file);
	return file_size;
}

void write_bitmap(FILE* file, uint8_t* bitmap, uint32_t block_number, uint32_t size, uint16_t block_size) {
	seek_to_block(file, block_number, block_size);
	fwrite(bitmap, 1, size, file);
}

uint32_t find_blocks(uint32_t* blocks, uint8_t* block_map, uint32_t fs_blk_count, uint32_t blocks_needed) {
	uint32_t block_index = 0;
	for (uint32_t j = 0; j < fs_blk_count && block_index < blocks_needed; j++) {
		if (!block_map[j]) {
			blocks[block_index++] = j;
		}
	}
	return block_index;
}
