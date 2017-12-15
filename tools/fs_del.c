#include "tex_fs_tools.h"

void print_usage() {
	printf("./fs_del <file_to_delete> <image_file>\n");
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		print_usage();
		return EXIT_FAILURE;
	}

	char* filename = argv[1];
	char* image_file = argv[2];
	FILE* image = fopen(image_file, "r+b");
	if (!image) {
		printf("The image file doesn't exist\n");
		return EXIT_FAILURE;
	}
	tex_fs_metadata_t fs;
	read_image(image_file, &fs);

	int inode_index_to_del = find_inode_number_for_file(filename, &fs);

	if (inode_index_to_del == -1) {
		printf("The file %s you want to delete doesn't exist on this filesystem\n", filename);
		return EXIT_FAILURE;
	}

	fs.inode_map[inode_index_to_del] = 0;
	//TODO rewrite inode_map to disk

	tex_fs_inode_t* inode_to_del = &fs.inode_list[inode_index_to_del];
	free_all_blocks_for_file(inode_to_del, &fs, image);

	//TODO rewrite block map to disk


	return 0;
}