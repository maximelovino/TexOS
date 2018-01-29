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

uint random[] = {35615, 51965, 51296, 28625, 22181, 29834, 41933, 35022, 24714, 30536, 23199, 47129, 17358, 48716, 37329, 20876, 1308, 32744, 21453, 6322, 6485, 25548, 11497, 58371, 32155, 52743, 15471, 39931, 58412, 23565, 33543, 50665, 39109, 64383, 13966, 35311, 887, 23126, 18155, 62987, 62114, 28232, 35813, 20959, 37997, 49876, 17006, 40319, 45228, 18583, 11800, 5974, 33403, 50391, 4285, 21517, 55022, 50069, 54667, 64573, 18287, 62400, 48965, 6130, 29827, 61357, 56154, 23522, 1612, 4692, 44765, 51324, 25794, 54508, 20735, 45716, 62118, 52614, 63348, 55824, 9637, 38145, 8612, 45824, 41595, 41991, 40585, 59354, 10880, 25832, 42549, 5744, 55747, 44899, 44874, 61679, 62478, 18812, 28469, 63419, 3002, 12360, 38705, 38823, 1657, 3012, 40244, 58267, 35663, 945, 63132, 11055, 62916, 13358, 4022, 23329, 38062, 88, 35341, 57158, 20707, 7656, 45116, 13623, 57791, 65178, 10323, 62612, 29841, 34314, 58492, 56112, 11124, 32629, 55422, 41218, 14860, 48, 26807, 4158, 30446, 50432, 16630, 43399, 64283, 1489, 8062, 39207, 26666, 25877, 12246, 32527, 43066, 46547, 22461, 59654, 25986, 31380, 1920, 27484, 57867, 37140, 17273, 52498, 34418, 41534, 38659, 17412, 49843, 3678, 61564, 38767, 19195, 51475, 6161, 25285, 47726, 4451, 1763, 54952, 14017, 56429, 31850, 31091, 5294, 57132, 19526, 3747, 21100, 23598, 7874, 19055, 29267, 65172, 27908, 8352, 53673, 31623, 34199, 9508, 61726, 45030, 42212, 22494, 12299, 15936, 23111, 5125, 55328, 21898, 30668, 7222, 59066, 42993, 23485, 58933, 26365, 5516, 28757, 6114, 60759, 60141, 16775, 52226, 27325, 56714, 56602, 5206, 33309, 36266, 17251, 17137, 54721, 54982, 36766, 64436, 23571, 6665, 52164, 10818, 31027, 41558, 43908, 3352, 49044, 19970, 36576, 44880, 799, 50405};

#define RANDOM_LENGTH (sizeof(random)/sizeof(random[0]))

void srand(uint seed){
    for (int i = 0; i < seed; ++i) {
        rand();
    }
}

uint rand(){
    uint val = random[0] ^ random[147];
    for (int i = 0; i < RANDOM_LENGTH-1; ++i) {
        random[i] = random[i+1];
    }
    random[RANDOM_LENGTH-1] = val;
    return val;
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
