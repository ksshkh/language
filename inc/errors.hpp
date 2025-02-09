#ifndef ERRORS_HPP
#define ERRORS_HPP

#define ERR(str) "\x1b[31m" str "\x1b[0m\n"

#include <stdio.h>

enum Errors {
    NO_ERROR               = 0,

    PTR_ERROR              = 1 << 1,
    SIZE_ERROR             = 1 << 2,
    STACK_UNDERFLOW        = 1 << 3,
    NO_STACK               = 1 << 4,
    FILE_ERROR             = 1 << 5,
    FOPEN_ERROR            = 1 << 6,
    FCLOSE_ERROR           = 1 << 7,
    READ_ERROR             = 1 << 8,
    BAD_CAPACITY           = 1 << 9,
    NO_DATA                = 1 << 10,
    BAD_DATA_RIGHT_CANARY  = 1 << 11,
    BAD_DATA_LEFT_CANARY   = 1 << 12,
    BAD_STACK_RIGHT_CANARY = 1 << 13,
    BAD_STACK_LEFT_CANARY  = 1 << 14,
    BAD_DATA_HASH          = 1 << 15,
    BAD_STACK_HASH         = 1 << 16,
    BAD_HASH               = 1 << 17,
    BAD_DATA_CANARIES      = 1 << 18,
    BAD_STACK_CANARIES     = 1 << 19,
    NO_TREE                = 1 << 20,
    NO_ROOT                = 1 << 21,
    BASE_ERROR             = 1 << 22,
    CONNECT_ERROR          = 1 << 23,

    N_ERROR                = 24
};

#define MY_ASSERT(expression, err) if(!(expression)) {                                                                      \
    fprintf(stderr, ERR("%s: %d (%s) My assertion failed: \"" #expression "\""), __FILE__, __LINE__, __func__);             \
    *code_error |= err;                                                                                                     \
}

void ErrorsPrint(FILE* stream, int* code_error);

#endif // ERRORS_HPP