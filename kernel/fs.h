/**
 * FS header file
 * @file 	fs.h
 * @project	TexOS
 * @author	Maxime Lovino, Loic Willy
 * @date	December 21, 2017
 */

#ifndef _FS_H_
#define _FS_H_

#include "../common/tex_fs.h"
#include "ide.h"
#include "min_std_lib.h"
#include "display.h"

#define FILE_DESCRIPTOR_TABLE_COUNT 128
/**
 * The stat structure, contains only the size
 */
typedef struct stat_st {
	uint32_t size;
} stat_t;

/**
 * The file iterator structure, contains the number of the current inode (-1 if no inode)
 */
typedef struct file_iterator_st {
	int inode_number;
} file_iterator_t;

/**
 * The file descriptor structure, contains a bool stating if open, a pointer to the file inode and a current offset
 */
typedef struct file_descriptor_st {
	bool open;
	tex_fs_inode_t* inode;
	uint32_t offset;
} file_descriptor_t;

/**
 * Function to get the stats for a file
 * @param filename	The name of the file we want to get the stats for
 * @param stat	The stat structure in which to return the result
 * @return	0 if successful, -1 otherwise
 */
extern int file_stat(char* filename, stat_t* stat);

/**
 * Function to list the content of the disk, with the format "name (size bytes)"
 */
extern void files_list();

/**
 * Function to check if a file exists
 * @param filename	The name of the file to check
 * @return	If the file exists or not
 */
extern bool file_exists(char* filename);

/**
 * Function to open a file descriptor for a file
 * @param filename	The name of the file to open
 * @return	A file descriptor for the file or -1 if the file couldn't be opened
 */
extern int file_open(char* filename);

/**
 * Function to read count bytes from a file
 * @param fd	The file descriptor to read
 * @param buf	The buffer in which to store the data read
 * @param count	The number of bytes to read
 * @return	The number of bytes read, 0 if the file was over, -1 in case of error
 */
extern int file_read(int fd, void* buf, uint count);

/**
 * Function to seek in a file
 * @param fd	The file descriptor to seek
 * @param offset	The offset at which to seek
 * @return	The offset seeked, or -1 in case of error
 */
extern int file_seek(int fd, uint offset);

/**
 * Function to close a file descriptor
 * @param fd	The file descriptor to close
 */
extern void file_close(int fd);

/**
 * @return	A file iterator for the files on the disk
 */
extern file_iterator_t file_iterator();

/**
 * Function to check if there is a next file
 * @param it	The file iterator
 * @return	if there is a next file
 */
extern bool file_has_next(file_iterator_t* it);

/**
 * Function to put the name of the next file in filename
 * @param filename	The string in which to put the filename of the next file
 * @param it	The file iterator
 */
extern void file_next(char* filename, file_iterator_t* it);

/**
 * Function to read the superblock from disk into the superblock structure
 * @param superblock	The superblock structure in which to store the superblock
 */
extern void read_superblock(tex_fs_superblock_t* superblock);

/**
 * Function to initialize the filesystem, will store the pointer to the structure allocated in kernel here,
 * and then load all metadata from the FS in RAM
 * @param fs_meta	The pointer to the allocated structure that will contain all FS metadata
 * @return 0 in case of success, 1 in case of error
 */
extern int fs_init(tex_fs_metadata_t* fs_meta);

#endif
