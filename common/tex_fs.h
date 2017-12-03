#ifndef _TEX_FS_H_
#define _TEX_FS_H_

#include "types.h"

#define TEX_FS_MAGIC ((u_int16_t) 0xD0D0)
#define TEX_FS_VERSION 1
#define SECTOR_SIZE 512

typedef struct tex_fs_superblock_st {
	u_int16_t magic;
	uint8_t version;
	char label[30];
	int16_t block_size;
	int16_t block_map;
	int16_t block_count;
	int16_t inode_list;
	int16_t inode_count;
	int16_t first_data_block;
} __attribute__((packed)) tex_fs_superblock_t;

typedef struct tex_fs_inode_st {
	char name[64];
	int16_t size;
	int16_t direct_blocks[8];
	int16_t indirect_blocks;
} __attribute__((packed)) tex_fs_inode_t;


#endif //_TEX_FS_H_
