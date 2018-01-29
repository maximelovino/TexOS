/**
 * FS list source file
 * @file 	fs_list.c
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	December 21, 2017
 */

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
		return EXIT_FAILURE;
	}

	char* filename = argv[1];

	tex_fs_metadata_t fs;
	FILE* image = fopen(filename, "rb");
	if (!image) {
		printf("The image %s doesn't exist\n", filename);
		return EXIT_FAILURE;
	}
	read_image(image, &fs);
	if (!valid_magic(fs.superblock->magic)) {
		printf("Magic is wrong, this is not a TexFS image\n");
		return EXIT_FAILURE;
	}
	list_all_files(&fs);
	free_tex_fs_metadata(&fs);
	fclose(image);
	return 0;
}