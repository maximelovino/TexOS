#ifndef _TEX_FS_H_
#define _TEX_FS_H_

#include "types.h"

#define TEX_FS_MAGIC ((u_int16_t) 0xD0D0)
#define TEX_FS_VERSION 1
#define SECTOR_SIZE 512
#define DIRECT_BLOCKS 8
#define INDIRECT_BLOCKS 4
#define BYTES_BLOCK_ADDRESS 4

typedef struct tex_fs_superblock_st {
	u_int16_t magic;
	uint8_t version;
	char label[30];
	uint16_t block_size;
	uint32_t block_map;
	uint32_t block_count;
	uint32_t inode_bitmap;
	uint32_t inode_list;
	uint32_t inode_count;
	uint32_t first_data_block;
} __attribute__((packed)) tex_fs_superblock_t;

typedef struct tex_fs_inode_st {
	char name[64];
	uint32_t size;
	uint32_t direct_blocks[DIRECT_BLOCKS];
	uint32_t indirect_blocks[INDIRECT_BLOCKS];
} __attribute__((packed)) tex_fs_inode_t;


#endif //_TEX_FS_H_
