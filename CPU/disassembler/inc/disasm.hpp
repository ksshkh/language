#ifndef DISASM_HPP
#define DISASM_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../../inc/errors.hpp"
#include "../../inc/stack.hpp"
#include "../../inc/commands.hpp"

struct Disassembler {
    const char* file_name_input = NULL;
    const char* file_name_output = NULL;

    int* buf_input = NULL;

    size_t size_code = 0;
};

void DisasmCtor(Disassembler* disasmblr, int* code_error);

void DisasmRun(Disassembler* disasmblr, int* code_error);

void PrintArgument(int* buff, size_t* i, FILE* result, int* code_error);

void PrintRegs(int reg, FILE* result, int* code_error);

void DisasmDtor(Disassembler* disasmblr, int* code_error);

#endif // DISASM_HPP