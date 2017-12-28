/**
 * Tex FS header file
 * @file 	tex_fs.h
 * @project	TexOS
 * @author	Maxime Lovino, Loic Willy
 * @date	December 21, 2017
 */

#ifndef _TEX_FS_H_
#define _TEX_FS_H_

#include "types.h"

#define TEX_FS_MAGIC ((uint16_t) 0xD0D0)
#define TEX_FS_VERSION 1
#define SECTOR_SIZE 512
#define DIRECT_BLOCKS 8
#define INDIRECT_BLOCKS 4
#define BYTES_BLOCK_ADDRESS 4
#define MAX_LABEL_LENGTH 30
#define MAX_FILENAME_LENGTH 64

typedef struct tex_fs_superblock_st {
	uint16_t magic;
	uint8_t version;
	char label[MAX_LABEL_LENGTH];
	uint16_t block_size;
	uint32_t block_map;
	uint32_t block_count;
	uint32_t inode_bitmap;
	uint32_t inode_list;
	uint32_t inode_count;
	uint32_t first_data_block;
} __attribute__((packed)) tex_fs_superblock_t;

typedef struct tex_fs_inode_st {
	char name[MAX_FILENAME_LENGTH];
	uint32_t size;
	uint32_t direct_blocks[DIRECT_BLOCKS];
	uint32_t indirect_blocks[INDIRECT_BLOCKS];
} __attribute__((packed)) tex_fs_inode_t;

typedef struct tex_fs_metadata_st {
	tex_fs_superblock_t* superblock;
	uint8_t* block_map;
	uint8_t* inode_map;
	tex_fs_inode_t* inode_list;
} __attribute__((packed)) tex_fs_metadata_t;

/**
 * Function to convert a size in bytes to a number of blocks required for the size
 * @param size	The size in bytes
 * @param block_size	The block size in bytes
 * @return	The number of blocks required
 */
extern uint32_t size_to_blocks(uint32_t size, uint16_t block_size);

/**
 * Function to check the validity of the magic
 * @param magic The magic to test
 * @return If the magic is valid or not
 */
bool valid_magic(uint16_t magic);

#endif //_TEX_FS_H_
