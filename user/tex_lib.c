#include "tex_lib.h"
#include "../common/syscall_nb.h"
#include "../common/min_std_lib.h"
#include "../kernel/x86.h"

extern int syscall(uint32_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);
extern void exitpoint();

int file_open(char* filename){
    return syscall(SYSCALL_FILE_OPEN, (uint32_t)filename, 0, 0, 0);
}

void file_close(int fd){
    syscall(SYSCALL_FILE_CLOSE, fd, 0,0,0);
}

int file_read(int fd, void* buf, uint count){
    return syscall(SYSCALL_FILE_READ, (uint32_t)fd, (uint32_t)buf, count, 0);
}

int file_stat(char* filename, stat_t* stat){
    return syscall(SYSCALL_FILE_STAT, (uint32_t)filename, (uint32_t)stat, 0, 0);
}

file_iterator_t file_iterator(){
    file_iterator_t iterator;
    syscall(SYSCALL_FILE_ITERATOR, (uint32_t)&iterator, 0, 0, 0);
    return iterator;
}

bool file_has_next(file_iterator_t* it){
    return it->inode_number != -1;
}

void file_next(char* filename, file_iterator_t* it){
    syscall(SYSCALL_FILE_NEXT, (uint32_t)filename, (uint32_t)it, 0, 0);
}

int exec(char* filename){
    return syscall(SYSCALL_EXEC, (uint32_t)filename, 0, 0, 0);
}

void exit(){
    exitpoint();
}

void putc(char c){
    syscall(SYSCALL_PUTS, (uint32_t)c, 0, 0, 0);
}

void puts(char* str){
    syscall(SYSCALL_PUTS, (uint32_t)str, 0, 0, 0);
}

//TODO Refactor code
void printf(char* fmt, ...){
    char* current_char = fmt;
    int next_param_shift = 1;
    while (*current_char) {
        if (strncmp(current_char, "%d", 2) == 0) {
            int* value = (void*) &fmt + next_param_shift * STACK_JUMP;
            char buffer[ITOA_BUFFER_SIZE] = {0};
            itoa(*value, false, buffer);
            puts(buffer);
            next_param_shift++;
            current_char++;
        } else if (strncmp(current_char, "%x", 2) == 0) {
            int* hexValue = (void*) &fmt + next_param_shift * STACK_JUMP;
            char hexBuffer[ITOA_BUFFER_SIZE] = {0};
            itoa(*hexValue, true, hexBuffer);
            puts("0x");
            puts(hexBuffer);
            next_param_shift++;
            current_char++;
        } else if (strncmp(current_char, "%s", 2) == 0) {
            char** string = (void*) &fmt + next_param_shift * STACK_JUMP;
            puts(*string);
            next_param_shift++;
            current_char++;
        } else if (strncmp(current_char, "%c", 2) == 0) {
            char* character = (void*) &fmt + next_param_shift * STACK_JUMP;
            putc(*character);
            next_param_shift++;
            current_char++;
        } else {
            putc(*current_char);
        }
        current_char++;
    }
}

int getc(){
    return syscall(SYSCALL_GETC, 0, 0, 0, 0);
}

int keypressed(){
    return syscall(SYSCALL_KEYPRESSED, 0, 0, 0, 0);
}



void sleep(uint ms){
    syscall(SYSCALL_SLEEP, ms, 0, 0, 0);
}

uint get_ticks(){
    return syscall(SYSCALL_GET_TICKS, 0, 0, 0, 0);
}

void srand(uint seed){
}

uint rand(){
}

bool starts_with(char* pattern, char* string){
    while(*pattern != '\0'){
        if(*string == '\0' || *pattern++ != *string++){
            return false;
        }
    }
    return true;
}

char* trim(char* string){
    while (*string == ' '){
        string++;
    }
    char *begin = string;
    char *first_space = NULL;
    while (*string != '\0'){
        if(*string == ' '){
            if(first_space == NULL){
                first_space = string;
            }
        }else{
            first_space = NULL;
        }
        string++;
    }
    if(first_space != NULL){
        *first_space = '\0';
    }
    return begin;
}
