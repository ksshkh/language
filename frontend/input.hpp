#ifndef INPUT_HPP
#define INPUT_HPP

#include <stdio.h>
#include <stdlib.h>

#include "../tree/tree.hpp"
#include "../tree/treedump.hpp"
#include "dsl.hpp"

const int TOKEN_NAME_LENGTH = 7;
const int NUM_OF_TOKENS     = 10;
const int INIT_NUM_OF_NAMES = 10;

struct Name {
    char* name = NULL;
    size_t length = 0;
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

void AddNewName(TableName* tbl_nm, char* name, size_t length, int* code_error);

void TableNameDtor(TableName* tbl_nm, int* code_error);

void  TokensParcing(Tree* tree, size_t* num_of_nodes, TableName* tbl_nm, int* code_error);

int FindName(TableName* tbl_nm, char* name, size_t length, int* code_error);

Operations FindToken(char* word, size_t length, int* code_error);

Node* GetTree     (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetOp       (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetIf       (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetWhile    (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetEqual    (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetAddAndSub(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetMulAndDiv(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetDeg      (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetUnaryOp  (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetBrackets (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetNum      (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetVar      (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);

#endif // INPUT_HPP