#include "tex_fs_tools.h"

void print_usage() {
	printf("./fs_add <file_to_add> <image_file>\n");
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		print_usage();
		return EXIT_FAILURE;
	}
	char* filename = argv[1];
	char* image_name = argv[2];
	tex_fs_metadata_t fs;
	read_image(image_name, &fs);
	print_superblock(fs.superblock);
}