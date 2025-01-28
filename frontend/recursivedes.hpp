#ifndef RECURSIVEDES_HPP
#define RECURSIVEDES_HPP

#include "input.hpp"

Node* GetTree     (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetOp       (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetIf       (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetElseIf   (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetWhile    (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetAssign   (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetAddAndSub(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetMulAndDiv(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetDeg      (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetUnaryOp  (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetBrackets (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetCond     (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetComp     (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetNum      (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);
Node* GetVar      (size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error);

#endif // RECURSIVEDES_HPP