#include "../inc/commands.hpp"

long int count_size_file(FILE* program, int* code_error) {

    MY_ASSERT(program != NULL, FILE_ERROR);

    struct stat file_info = {};
    fstat(fileno(program), &file_info);

    return file_info.st_size;
}