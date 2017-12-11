#ifndef _TEX_FS_TOOLS_H_
#define _TEX_FS_TOOLS_H_

#include "../common/tex_fs.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct tex_fs_metadata_st {
	tex_fs_superblock_t* superblock;
	uint8_t* block_map;
	uint8_t* inode_map;
	tex_fs_inode_t* inode_list;
} __attribute__((packed)) tex_fs_metadata_t;

void read_image(char* filename, tex_fs_metadata_t* fs);

void read_superblock(FILE* image, tex_fs_superblock_t* superblock);

void print_superblock(tex_fs_superblock_t* superblock);

uint32_t compute_max_file_size(tex_fs_superblock_t* superblock);

uint32_t compute_free_blocks_count(tex_fs_metadata_t* fs);

#endif //_TEX_FS_TOOLS_H_
