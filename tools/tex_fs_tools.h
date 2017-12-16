#ifndef _TEX_FS_TOOLS_H_
#define _TEX_FS_TOOLS_H_

#include "../common/tex_fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct tex_fs_metadata_st {
	tex_fs_superblock_t* superblock;
	uint8_t* block_map;
	uint8_t* inode_map;
	tex_fs_inode_t* inode_list;
} __attribute__((packed)) tex_fs_metadata_t;

void read_image(FILE* image, tex_fs_metadata_t* fs);

void free_tex_fs_metadata(tex_fs_metadata_t* fs);

void read_superblock(FILE* image, tex_fs_superblock_t* superblock);

void print_superblock(tex_fs_superblock_t* superblock);

void print_inode(tex_fs_inode_t* inode);

uint32_t compute_max_file_size(tex_fs_superblock_t* superblock);

uint32_t compute_free_blocks_count(tex_fs_metadata_t* fs);

uint32_t compute_free_inodes_count(tex_fs_metadata_t* fs);

void list_all_files(tex_fs_metadata_t* fs);

bool is_file_already_present(char* filename, tex_fs_metadata_t* fs);

bool valid_magic(tex_fs_metadata_t* fs);

int find_inode_number_of_file(char* filename, tex_fs_metadata_t* fs);

void free_all_blocks_for_file(tex_fs_inode_t* inode, tex_fs_metadata_t* fs, FILE* image);

void seek_to_block(FILE* file, uint32_t block_number, uint16_t block_size);

int find_free_inode_index(tex_fs_metadata_t* fs);

uint32_t get_size_of_file(FILE* file);

void write_bitmap_to_file(FILE* file, uint8_t* bitmap, uint32_t block_number, uint32_t size, uint16_t block_size);

uint32_t
find_blocks_for_file(uint32_t* blocks, uint8_t* block_map, uint32_t fs_block_count, uint32_t total_blocks_needed);

//TODO make a reverse bits function that takes a block map pointer and an array of block indices and reverses the bits for those blocks

#endif //_TEX_FS_TOOLS_H_
