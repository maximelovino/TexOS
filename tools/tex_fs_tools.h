/**
 * Tex FS tools header file
 * @file 	tex_fs_tools.h
 * @project	TexOS
 * @author	Maxime Lovino, Loic Willy
 * @date	December 21, 2017
 */

#ifndef _TEX_FS_TOOLS_H_
#define _TEX_FS_TOOLS_H_

#include "../common/tex_fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Function to load all the content of the image metadata in RAM
 * @param image A file pointer to the image
 * @param fs	A pointer to the structure which will contain the image
 */
extern void read_image(FILE* image, tex_fs_metadata_t* fs);

/**
 * Function to free the structure containing the image metadata in RAM
 * @param fs The structure to free
 */
extern void free_tex_fs_metadata(tex_fs_metadata_t* fs);

/**
 * Function to read the superblock of an image
 * @param image	A file pointer to the image
 * @param superblock	A pointer to an allocated superblock structure
 */
extern void read_superblock(FILE* image, tex_fs_superblock_t* superblock);

/**
 * Function to print to the console the content of a superblock
 * @param superblock A pointer to a superblock structure
 */
extern void print_superblock(tex_fs_superblock_t* superblock);

/**
 * Function to print to the console the content of an inode
 * @param inode A pointer to an inode structure
 */
extern void print_inode(tex_fs_inode_t* inode);

/**
 * Function to compute the maximum file size on a filesystem
 * @param superblock A pointer to the superblock structure of the filesystem
 * @return The maximum size of a file, in bytes
 */
extern uint32_t compute_max_file_size(tex_fs_superblock_t* superblock);

/**
 * Function to compute the number of free blocks on the filesystem
 * @param fs A pointer to the metadata structure
 * @return The number of free blocks
 */
extern uint32_t compute_free_blocks_count(tex_fs_metadata_t* fs);

/**
 * Function to compute the number of free inodes on the filesystem
 * @param fs A pointer to the metadata structure
 * @return The number of free inodes
 */
extern uint32_t compute_free_inodes_count(tex_fs_metadata_t* fs);

/**
 * Function to print to the console the list of files present on the filesystem
 * with the format "<filename>\t<size_bytes>"
 * @param fs A pointer to the metadata structure
 */
extern void list_all_files(tex_fs_metadata_t* fs);

/**
 * Function to check if a file exists on the filesystem
 * @param filename The name of the file
 * @param fs A pointer to the metadata structure
 * @return If the file exists or not
 */
extern bool does_file_exist(char* filename, tex_fs_metadata_t* fs);

/**
 * Function to find the inode number for a file
 * @param filename The name of the file
 * @param fs A pointer to the metadata structure
 * @return The inode number, or -1 if the file doesn't exist
 */
extern int find_inode_number_of_file(char* filename, tex_fs_metadata_t* fs);

/**
 * Function to free all blocks for a given file (given its inode)
 * @param inode The inode of the file to free
 * @param fs A pointer to the metadata structure
 * @param image A file pointer to the opened image
 */
extern void free_all_blocks_for_file(tex_fs_inode_t* inode, tex_fs_metadata_t* fs, FILE* image);

/**
 * Function to seek in the open file to the start given block number
 * @param file	A file pointer to the opened image
 * @param block_number	The block number to jump to
 * @param block_size	The block size
 */
extern void seek_to_block(FILE* file, uint32_t block_number, uint16_t block_size);

/**
 * Function to find a free inode index to store a new file
 * @param fs A pointer to the metadata structure
 * @return The number of a free inode or -1 if there isn't one
 */
extern int find_free_inode_index(tex_fs_metadata_t* fs);

/**
 * Function to get the size of an opened file
 * @param file A file pointer to the opened image
 * @return The size of the file, in bytes
 */
extern uint32_t get_size_of_file(FILE* file);

/**
 * Function to write a bitmap (inode or block) to an opened image file
 * @param file A file pointer to the opened image
 * @param bitmap 	A pointer to the bitmap to write
 * @param block_number	The block number at which to write the bitmap
 * @param size 	The size of the bitmap
 * @param block_size The block_size of the filesystem
 */
extern void write_bitmap(FILE* file, uint8_t* bitmap, uint32_t block_number, uint32_t size, uint16_t block_size);

/**
 * Function to find all blocks required for a file
 * @param blocks An array in which to store the block indices
 * @param block_map	A pointer to the block bitmap
 * @param fs_blk_count	The number of blocks of the filesystem (size of the bitmap)
 * @param blocks_needed	The number of blocks needed
 * @return	The number of blocks found
 */
extern uint32_t find_blocks(uint32_t* blocks, uint8_t* block_map, uint32_t fs_blk_count, uint32_t blocks_needed);
#endif //_TEX_FS_TOOLS_H_
