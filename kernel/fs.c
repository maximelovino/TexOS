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
