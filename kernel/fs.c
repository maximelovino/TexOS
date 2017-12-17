#include "fs.h"

static file_descriptor_t descriptors[FILE_DESCRIPTOR_TABLE_COUNT] = {0};
static int next_free_descriptor = 0;
static uint16_t free_descriptor_count = FILE_DESCRIPTOR_TABLE_COUNT;
static tex_fs_metadata_t* fs;
static tex_fs_superblock_t* sb;

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

void files_list() {
	stat_t st;
	file_iterator_t it = file_iterator();
	char filename[MAX_FILENAME_LENGTH];
	while (file_has_next(&it)) {
		file_next(filename, &it);
		int ret = file_stat(filename, &st);
		display_printf("%s (%d bytes)\n", filename, st.size, ret);
	}
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
	//TODO so now basically, we have to get the descriptor and read count bytes at the current offset
	file_descriptor_t* desc = &descriptors[fd];
	tex_fs_inode_t* inode = desc->inode;
	return read_bytes(inode, buf, desc->offset, count);
}

int read_bytes(tex_fs_inode_t* inode, void* buf, uint32_t start_offset, uint32_t count) {
	if (start_offset == inode->size - 1) {
		return 0;
	}
	uint32_t in_block_offset = start_offset % sb->block_size;
	uint32_t file_block_containing_offset = start_offset / sb->block_size;
	uint32_t count_modulo = count % sb->block_size;
	uint32_t blocks_to_read = count / sb->block_size + (count_modulo != 0);
	if (in_block_offset + count_modulo > sb->block_size)
		blocks_to_read++;

	uint8_t block[sb->block_size];
	uint32_t count_remaining = count;
	uint32_t to_copy = 0;
	for (int i = 0; i < blocks_to_read; i++) {
		int block_num = bmap(inode, file_block_containing_offset + i);
		if (block_num == -1) {
			break;
		}
		read_block(block_num, block);
		if (in_block_offset) {
			if (i == 0) {
				to_copy = sb->block_size - in_block_offset;
				memcpy(buf, &block[in_block_offset], to_copy);
			} else {
				to_copy = MIN(count_remaining, sb->block_size);
				memcpy(&buf[(sb->block_size - in_block_offset) + (i - 1) * sb->block_size], block, to_copy);
			}
		} else {
			to_copy = MIN(count_remaining, sb->block_size);
			memcpy(&buf[i * sb->block_size], block, to_copy);
		}
		count_remaining -= to_copy;
	}
	return count - count_remaining;

}

int bmap(tex_fs_inode_t* inode, uint32_t absolute_block_of_file) {
	if (absolute_block_of_file < DIRECT_BLOCKS) {
		return inode->direct_blocks[absolute_block_of_file] == 0 ? -1 : inode->direct_blocks[absolute_block_of_file];
	} else {
		uint32_t block_indexes_per_block = sb->block_size / BYTES_BLOCK_ADDRESS;
		absolute_block_of_file -= DIRECT_BLOCKS;
		uint16_t indirect_block_index = absolute_block_of_file / block_indexes_per_block;
		uint16_t block_in_indirect = absolute_block_of_file % block_indexes_per_block;
		if (indirect_block_index >= INDIRECT_BLOCKS)
			return -1;

		if (inode->indirect_blocks[indirect_block_index] == 0)
			return -1;

		uint32_t block[sb->block_size / BYTES_BLOCK_ADDRESS];
		read_block(inode->indirect_blocks[indirect_block_index], block);
		return block[block_in_indirect] == 0 ? -1 : block[block_in_indirect];
	}
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
	result.inode_number = find_next_used_inode(0);
	return result;
}

int find_next_used_inode(uint32_t start_index) {
	for (uint32_t i = start_index; i < sb->inode_count; i++) {
		if (fs->inode_map[i]) {
			return i;
		}
	}
	return -1;
}

bool file_has_next(file_iterator_t* it) {
	return it->inode_number != -1;
}

void file_next(char* filename, file_iterator_t* it) {
	if (it->inode_number == -1) return;

	memcpy(filename, fs->inode_list[it->inode_number].name, MAX_FILENAME_LENGTH);
	it->inode_number = find_next_used_inode(it->inode_number + 1);
}

void read_superblock(tex_fs_superblock_t* superblock) {
	uint8_t sector_data[SECTOR_SIZE];
	memset(sector_data, 0, SECTOR_SIZE);
	read_sector(0, sector_data);
	memcpy(superblock, sector_data, sizeof(tex_fs_superblock_t));
}

void fs_init(tex_fs_metadata_t* fs_meta) {
	fs = fs_meta;
	sb = fs->superblock;
	read_image();
}

void read_image() {
	read_bitmap(fs->block_map, sb->block_count, sb->block_map);
	read_bitmap(fs->inode_map, sb->inode_count, sb->inode_bitmap);

	uint32_t inode_list_blocks_taken = sb->first_data_block - sb->inode_list;
	uint8_t* raw_data_inode_blocks[inode_list_blocks_taken * sb->block_size];

	for (uint32_t i = 0; i < inode_list_blocks_taken; i++) {
		read_block(sb->inode_list + i, &raw_data_inode_blocks[i * sb->block_size]);
	}

	memcpy(fs->inode_list, raw_data_inode_blocks, sizeof(tex_fs_inode_t) * sb->inode_count);
}

void read_bitmap(void* bitmap_data, uint32_t bitmap_size, uint32_t start_block) {
	uint32_t number_blocks_to_read = bitmap_size / sb->block_size + (bitmap_size % sb->block_size != 0);

	uint8_t* raw_block_bitmap[number_blocks_to_read * sb->block_size];
	memset(raw_block_bitmap, 0, number_blocks_to_read * sb->block_size);

	for (int i = 0; i < number_blocks_to_read; i++) {
		read_block(start_block + i, &raw_block_bitmap[sb->block_size * i]);
	}
	memcpy(bitmap_data, raw_block_bitmap, bitmap_size);

}

void read_block(uint32_t block_number, void* block_data) {
	uint16_t sectors_for_block = sb->block_size / SECTOR_SIZE;

	uint32_t first_sector = block_number * sectors_for_block;

	for (int i = 0; i < sectors_for_block; i++) {
		read_sector(first_sector + i, &block_data[SECTOR_SIZE * i]);
	}
}

tex_fs_inode_t* find_inode_of_file(char* filename) {
	for (uint32_t i = 0; i < sb->inode_count; i++) {
		if (fs->inode_map[i] && strcmp(fs->inode_list[i].name, filename) == 0) {
			return &fs->inode_list[i];
		}
	}
	return NULL;
}

