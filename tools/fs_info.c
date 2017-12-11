#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "tex_fs_tools.h"

void print_usage() {
	printf("./fs_info <image_file>\n");
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		print_usage();
		exit(1);
	}
	char* image_name = argv[1];
	tex_fs_metadata_t fs;
	read_image(image_name, &fs);
	uint32_t free_blocks = compute_free_blocks_count(&fs);
	printf("Displaying infos about %s\n", image_name);
	printf("===============\n");
	print_superblock(fs.superblock);
	printf("===============\n");
	printf("Free blocks: %d / %d blocks\n", free_blocks, fs.superblock->block_count);
	printf("Free space: %d / %d bytes\n", free_blocks * fs.superblock->block_size,
		   fs.superblock->block_count * fs.superblock->block_size);
}