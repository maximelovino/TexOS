/**
 * Tex FS source file
 * @file 	tex_fs.h
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	December 21, 2017
 */

#include "tex_fs.h"

uint32_t size_to_blocks(uint32_t size, uint16_t block_size) {
	return size / block_size + (size % block_size != 0);
}

bool valid_magic(uint16_t magic) {
	return magic == TEX_FS_MAGIC;
}