#include "fs.h"

int file_stat(char* filename, stat_t* stat) {
	return 0;
}

bool file_exists(char* filename) {
	return 0;
}

int file_open(char* filename) {
	return 0;
}

int file_read(int fd, void* buf, uint count) {
	return 0;
}

int file_seek(int fd, uint offset) {
	return 0;
}

void file_close(int fd) {

}

file_iterator_t file_iterator() {
	file_iterator_t result;
	return result;
}

bool file_has_next(file_iterator_t* it) {
	return 0;
}

void file_next(char* filename, file_iterator_t* it) {

}

void read_superblock(tex_fs_superblock_t* superblock) {
	uint8_t sector_data[SECTOR_SIZE];
	memset(sector_data, 0, SECTOR_SIZE);
	read_sector(0, sector_data);
	memcpy(superblock, sector_data, sizeof(tex_fs_superblock_t));
}

void read_image(tex_fs_metadata_t* fs) {
	read_bitmap(fs->block_map, fs->superblock->block_count, fs->superblock->block_map, fs->superblock->block_size);
	read_bitmap(fs->inode_map, fs->superblock->inode_count, fs->superblock->inode_bitmap, fs->superblock->block_size);

	//TODO now read inode_list
}

void read_bitmap(void* bitmap_data, uint32_t bitmap_size, uint32_t start_block, uint16_t block_size) {
	uint32_t number_blocks_to_read = bitmap_size / block_size + (bitmap_size % block_size != 0);

	uint8_t* raw_block_bitmap[number_blocks_to_read * block_size];
	memset(raw_block_bitmap, 0, number_blocks_to_read * block_size);

	for (int i = 0; i < number_blocks_to_read; i++) {
		read_block(start_block + i, &raw_block_bitmap[block_size * i], block_size);
	}
	memcpy(bitmap_data, raw_block_bitmap, bitmap_size);

}

void read_block(uint32_t block_number, void* block_data, uint16_t block_size) {
	uint16_t sectors_for_block = block_size / SECTOR_SIZE;

	uint32_t first_sector = block_number * sectors_for_block;

	for (int i = 0; i < sectors_for_block; i++) {
		read_sector(first_sector + i, &block_data[SECTOR_SIZE * i]);
	}
}

