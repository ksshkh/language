#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../inc/errors.hpp"
#include "../../inc/stack.hpp"
#include "../../inc/commands.hpp"

const size_t InitCapacity = 5;
const size_t REG_SIZE = 4;
const size_t RAM_SIZE = 10 * 10;
const size_t ram_line = 10;

struct SPU {
    const char* file_name_input = NULL;

    size_t ip = 0;

    size_t code_size = 0;
    int* code = NULL;

    Stack_t stk = {};
    Stack_t func_stk = {};

    int* registers = NULL;
    StackElem_t* ram = NULL;
};

void SPUCtor(SPU* spu, int* code_error);

void CodeReader(SPU* spu, int* code_error);

void SPURun(SPU* spu, int* code_error);

StackElem_t* GetArgument(SPU* spu, int current_cmd, int* code_error);

void Draw(SPU* spu, StackElem_t line, int* code_error);

void SPUDump(SPU* spu, int* code_error);

void SPUDtor(SPU* spu, int* code_error);

#endif // PROCESSOR_HPP