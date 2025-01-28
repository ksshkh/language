#ifndef TREEDUMP_HPP
#define TREEDUMP_HPP

#include "tree.hpp"

void DotTreeDump(Tree* tree, int* code_error);

void PrintDotNode(Node* node, FILE* stream);

void GraphCreate(void);

void HtmlDump(int *code_error);

void PrintTree(Tree* tree, int* code_error);

void PreorderPrinting(Node* node, FILE* stream, int* code_error);

void InorderPrinting(Node* node, FILE* stream, int* code_error);

#endif // TREEDUMP_HPP