#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "../tree/tree.hpp"
#include "../tree/treedump.hpp"

void ConvertToAsm(Tree* tree, int* code_error);

void AsmPrint(Node* node, FILE* stream, int* code_error);

void AsmPrintOp(Operations op, FILE* stream, int* code_error);

void AsmPrintNum(TreeElem num, FILE* stream, int* code_error);

#endif