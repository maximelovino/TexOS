#include "fs.h"

static file_descriptor_t descriptors[FILE_DESCRIPTOR_TABLE_COUNT] = {0};
static int next_free_descriptor = 0;
static uint16_t free_descriptor_count = FILE_DESCRIPTOR_TABLE_COUNT;
static tex_fs_metadata_t* fs;

int file_stat(char* filename, stat_t* stat) {
	tex_fs_inode_t* inode = find_inode_of_file(filename);
	if (!inode) {
		return -1;
	}
	stat->size = inode->size;
	return 0;
}

bool file_exists(char* filename) {
	return find_inode_of_file(filename) != NULL;
}

int find_next_free_descriptor() {
	if (!free_descriptor_count)
		return -1;

	for (int i = 0; i < FILE_DESCRIPTOR_TABLE_COUNT; i++) {
		if (!descriptors[i].open) {
			return i;
		}
	}
	return -1;
}

int file_open(char* filename) {
	tex_fs_inode_t* inode = find_inode_of_file(filename);
	if (!inode) {
		return -1;
	}
	//create descriptor and return index
	if (!free_descriptor_count || next_free_descriptor == -1) {
		return -1;
	}

	free_descriptor_count--;
	int my_descriptor = next_free_descriptor;
	file_descriptor_t* desc = &descriptors[my_descriptor];
	desc->inode = inode;
	desc->offset = 0;
	desc->open = true;

	next_free_descriptor = find_next_free_descriptor();

	return my_descriptor;
}

int file_read(int fd, void* buf, uint count) {
	if (fd >= FILE_DESCRIPTOR_TABLE_COUNT && !descriptors[fd].open)
		return -1;
	return 0;
}

int file_seek(int fd, uint offset) {
	if (fd >= FILE_DESCRIPTOR_TABLE_COUNT && !descriptors[fd].open)
		return -1;

	if (offset <= descriptors[fd].inode->size) {
		descriptors[fd].offset = offset;
		return offset;
	} else {
		return -1;
	}
}

void file_close(int fd) {
	if (fd >= FILE_DESCRIPTOR_TABLE_COUNT && !descriptors[fd].open)
		return;
	descriptors[fd].open = false;
	if (!free_descriptor_count || next_free_descriptor == -1) {
		next_free_descriptor = fd;
	}
	free_descriptor_count++;
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

void fs_init(tex_fs_metadata_t* fs_meta) {
	fs = fs_meta;
	read_image();
}

void read_image() {
	read_bitmap(fs->block_map, fs->superblock->block_count, fs->superblock->block_map, fs->superblock->block_size);
	read_bitmap(fs->inode_map, fs->superblock->inode_count, fs->superblock->inode_bitmap, fs->superblock->block_size);

	uint32_t inode_list_blocks_taken = fs->superblock->first_data_block - fs->superblock->inode_list;
	uint8_t* raw_data_inode_blocks[inode_list_blocks_taken * fs->superblock->block_size];

	for (uint32_t i = 0; i < inode_list_blocks_taken; i++) {
		read_block(fs->superblock->inode_list + i, &raw_data_inode_blocks[i * fs->superblock->block_size],
				   fs->superblock->block_size);
	}

	memcpy(fs->inode_list, raw_data_inode_blocks, sizeof(tex_fs_inode_t) * fs->superblock->inode_count);
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

tex_fs_inode_t* find_inode_of_file(char* filename) {
	for (uint32_t i = 0; i < fs->superblock->inode_count; i++) {
		if (fs->inode_map[i] && strcmp(fs->inode_list[i].name, filename) == 0) {
			return &fs->inode_list[i];
		}
	}
	return NULL;
}

