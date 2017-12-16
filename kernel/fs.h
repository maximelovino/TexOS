#ifndef _FS_H_
#define _FS_H_

#include "../common/tex_fs.h"
#include "ide.h"
#include "min_std_lib.h"

typedef struct stat_st {

} stat_t;

typedef struct file_iterator_st {

} file_iterator_t;

typedef struct file_descriptor_st {

} file_descriptor_t;

int file_stat(char* filename, stat_t* stat);

bool file_exists(char* filename);

int file_open(char* filename);

int file_read(int fd, void* buf, uint count);

int file_seek(int fd, uint offset);

void file_close(int fd);

file_iterator_t file_iterator();

bool file_has_next(file_iterator_t* it);

void file_next(char* filename, file_iterator_t* it);

void read_superblock(tex_fs_superblock_t* superblock);

void read_image(tex_fs_metadata_t* fs);

void read_block(uint32_t block_number, void* block_data, uint16_t block_size);

#endif
