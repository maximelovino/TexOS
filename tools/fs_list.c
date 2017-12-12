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

}