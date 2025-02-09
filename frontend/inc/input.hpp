#ifndef INPUT_HPP
#define INPUT_HPP

#include <stdio.h>
#include <stdlib.h>

#include "../../inc/tree.hpp"
#include "../../inc/treedump.hpp"
#include "../../inc/dsl.hpp"

const int TOKEN_NAME_LENGTH = 10;
const int NUM_OF_TOKENS     = 11;
const int INIT_NUM_OF_NAMES = 10;

struct Name {
    char* name = NULL;
    size_t length = 0;
    Type type = DEF_TYPE;
};

struct TableName {
    Name* table_name = NULL;
    size_t free_id = 0;
    size_t num_of_names = 0;
};

struct Token {
    char name[TOKEN_NAME_LENGTH] = {};
    Operations code = DEF_OP;
};

void TableNameCtor(TableName* tbl_nm, int* code_error);

void AddNewName(TableName* tbl_nm, char* name, size_t length, Type type, int* code_error);

void TableNameDtor(TableName* tbl_nm, int* code_error);

void  TokensParcing(Tree* tree, size_t* num_of_nodes, TableName* tbl_nm, int* code_error);

int FindName(TableName* tbl_nm, char* name, size_t length, int* code_error);

Operations FindToken(char* word, size_t length, int* code_error);

void PrintTableName(TableName* tbl_nm, int* code_error);

#endif // INPUT_HPP