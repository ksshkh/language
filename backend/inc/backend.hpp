#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "../../inc/tree.hpp"
#include "../../inc/treedump.hpp"

void ConvertToAsm(Tree* tree, int* code_error);

void AsmPrint(Node* node, size_t* label_id, size_t* loop_id, FILE* stream, int* code_error);

void AsmPrintOp(Node* node, size_t* label_id, FILE* stream, int* code_error);

size_t GetIfDepth(Node* node, int* code_error);

void AsmPrintIf(Node* node, size_t old_label_id, size_t* label_id, FILE* stream, int* code_error);

void AsmPrintInequality(Node* node, size_t* label_id, FILE* stream, int* code_error);

void AsmPrintAssigment(Node* node, FILE* stream, int* code_error);

void AsmPrintVar(Node* node, FILE* stream, int* code_error);

void AsmPrintNum(TreeElem num, FILE* stream, int* code_error);

void AsmPrintPar(Node* node, FILE* stream, int* code_error);

#endif // BACKEND_HPP