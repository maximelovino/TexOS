#ifndef _FS_H_
#define _FS_H_

#include "../common/tex_fs.h"
#include "ide.h"
#include "min_std_lib.h"

#define FILE_DESCRIPTOR_TABLE_COUNT 128

typedef struct stat_st {
	uint32_t size;
} stat_t;

typedef struct file_iterator_st {
	int inode_number;
} file_iterator_t;

typedef struct file_descriptor_st {
	bool open;
	tex_fs_inode_t* inode;
	uint32_t offset;
} file_descriptor_t;

int file_stat(char* filename, stat_t* stat);

int find_next_free_descriptor();

bool file_exists(char* filename);

int file_open(char* filename);

int file_read(int fd, void* buf, uint count);

int file_seek(int fd, uint offset);

void file_close(int fd);

file_iterator_t file_iterator();

bool file_has_next(file_iterator_t* it);

void file_next(char* filename, file_iterator_t* it);

void read_superblock(tex_fs_superblock_t* superblock);

void read_image();

void read_block(uint32_t block_number, void* block_data, uint16_t block_size);

void read_bitmap(void* bitmap_data, uint32_t bitmap_size, uint32_t start_block);

tex_fs_inode_t* find_inode_of_file(char* filename);

int find_next_used_inode(uint32_t start_index);

void fs_init(tex_fs_metadata_t* fs_meta);

#endif
