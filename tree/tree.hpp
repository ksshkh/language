#ifndef TREE_HPP
#define TREE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "../frontend/errors.hpp"
#include "../frontend/utils.hpp"

static const char* INPUT_FILE = "../input.txt";

#define DEBUG

typedef double TreeElem;

const double DEF_VAL = -666.0;
const int DEF_IP     = -1;

enum Side {
    LEFT,
    RIGHT,
};

enum Type {
    DEF_TYPE = 0,

    OP,
    VAR,
    NUM,
    IDE,
    FUNC_IDE,
    PAR,
};

enum Operations {
    DEF_OP = -1,

    ADD  = '+',
    SUB  = '-',
    MUL  = '*',
    DIV  = '/',
    DEG  = '^',
    SIN  = 's',
    COS  = 'c',
    LN   = 'l',

    EOT   = '$',
    L_BR  = '(',
    R_BR  = ')',
    L_FBR = '{',
    R_FBR = '}',
    SEM   = ';',
    VAR_S = '=',

    EQU   = '==',
    NEQ   = '!=',
    AE    = '>=',
    BE    = '<=',
    ABOVE = '>',
    BELOW = '<',
    OR    = '|',
    AND   = '&',

    IF    = 'i',
    WHILE = 'w',
    DEF   = 'd',
    ELSE  = 'e',
};

enum Vars {
    X = 'x',
};

struct Node {
    TreeElem data = 0;

    Type type = DEF_TYPE;

    Node* left = NULL;
    Node* right = NULL;
    Node* parent = NULL;
};

struct Tree {
    Node* root = NULL;
    size_t num_of_nodes = 0;

    Node** tokens = NULL;

    char* data_base = 0;
    long int size_data_base = 0;
};

#ifdef DEBUG

    #define TREE_ASSERT(tree) {                                               \
        int err = TreeVerification(tree, code_error);                         \
        if (err != NO_ERROR) {                                                \
            DotTreeDump(tree, code_error);                                    \
            fprintf(stderr, "error %d (file: %s, function: %s, line: %d)\n",  \
                    *code_error, __FILE__, __func__, __LINE__);               \
            ErrorsPrint(stderr, code_error);                                  \
        }                                                                     \
    }

    int TreeVerification(const Tree* tree, int* code_error);

    int NodeVerificator(const Node* node, int* code_error);

#else
    #define TREE_ASSERT(...)
#endif

void TreeCtor(Tree* tree, int* code_error);

Node* NodeCtor(size_t* num_of_nodes, Type type, TreeElem data, Node* left, Node* right, Node* parent, int* code_error);

void AddNewNode(size_t* num_of_nodes, Type type, Node* node, TreeElem data, Side side, int* code_error);

Node* ReplaceNode(size_t* num_of_nodes, Node* node, Node* new_node, int* code_error);

void TreeDtor(Tree* tree, int* code_error);

void FreeNode(size_t* num_of_nodes, Node* node, int* code_error);

void ReadTree(Tree* tree, int* code_error);

Node* ReadNode(Tree* tree, Node* node, Node* parent, int* code_error);

#endif // TREE_HPP