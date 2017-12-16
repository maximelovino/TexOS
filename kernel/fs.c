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
	uint32_t number_blocks_to_read = fs->superblock->block_count / fs->superblock->block_size +
									 (fs->superblock->block_count % fs->superblock->block_size != 0);

	uint8_t* raw_block_bitmap[number_blocks_to_read * fs->superblock->block_size];
	memset(raw_block_bitmap, 0, number_blocks_to_read * fs->superblock->block_size);
	for (int i = 0; i < number_blocks_to_read; i++) {
		read_block(fs->superblock->block_map + i, &raw_block_bitmap[fs->superblock->block_size * i],
				   fs->superblock->block_size);
	}
	memcpy(fs->block_map, raw_block_bitmap, fs->superblock->block_count);
}

void read_block(uint32_t block_number, void* block_data, uint16_t block_size) {
	uint16_t sectors_for_block = block_size / SECTOR_SIZE;

	uint32_t first_sector = block_number * sectors_for_block;

	for (int i = 0; i < sectors_for_block; i++) {
		read_sector(first_sector + i, &block_data[SECTOR_SIZE * i]);
	}
}

