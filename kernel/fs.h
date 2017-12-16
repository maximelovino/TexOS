#ifndef _FS_H_
#define _FS_H_

#include "../common/tex_fs.h"

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

#endif
