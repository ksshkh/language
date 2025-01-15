#ifndef DSL_HPP
#define DSL_HPP

#define _NUM(num) NodeCtor(num_of_nodes, NUM, num, NULL, NULL, NULL, code_error)
#define _VAR(var) NodeCtor(num_of_nodes, VAR, var, NULL, NULL, NULL, code_error)

#define _FUNC_IDE(func_ide) NodeCtor(num_of_nodes, FUNC_IDE, func_ide, NULL, NULL, NULL, code_error)

#define _EOT      NodeCtor(num_of_nodes, DEF_TYPE,  EOT,   NULL, NULL, NULL, code_error)
#define _DEF      NodeCtor(num_of_nodes, DEF_TYPE,  DEF,   NULL, NULL, NULL, code_error)
#define _L_BR     NodeCtor(num_of_nodes, DEF_TYPE,  L_BR,  NULL, NULL, NULL, code_error)
#define _R_BR     NodeCtor(num_of_nodes, DEF_TYPE,  R_BR,  NULL, NULL, NULL, code_error)

#define _EQU(left, right) NodeCtor(num_of_nodes, OP,  EQU, left, right, NULL, code_error)
#define _ADD(left, right) NodeCtor(num_of_nodes, OP,  ADD, left, right, NULL, code_error)
#define _SUB(left, right) NodeCtor(num_of_nodes, OP,  SUB, left, right, NULL, code_error)
#define _MUL(left, right) NodeCtor(num_of_nodes, OP,  MUL, left, right, NULL, code_error)
#define _DIV(left, right) NodeCtor(num_of_nodes, OP,  DIV, left, right, NULL, code_error)
#define _DEG(left, right) NodeCtor(num_of_nodes, OP,  DEG, left, right, NULL, code_error)
#define _SEM(left, right) NodeCtor(num_of_nodes, DEF_TYPE,  SEM,   left, right, NULL, code_error)
#define _SIN(left)        NodeCtor(num_of_nodes, OP,  SIN, left, _NUM(0.0), NULL, code_error)
#define _COS(left)        NodeCtor(num_of_nodes, OP,  COS, left, _NUM(0.0), NULL, code_error)
#define _LN(left)         NodeCtor(num_of_nodes, OP,  LN,  left, _NUM(0.0), NULL, code_error)

#define _IF(left, right)    NodeCtor(num_of_nodes, IDE,  IF,     left, right, NULL, code_error)
#define _WHILE(left, right) NodeCtor(num_of_nodes, IDE,  WHILE,  left, right, NULL, code_error)

#define _DIFF(node)         DiffTree(num_of_nodes, node, code_error)
#define _COPY(node, parent) CopyTree(num_of_nodes, node,  parent, code_error)
#define _FREE(node)         FreeNode(num_of_nodes, node, code_error)

#define UNARY_CHECK(data) data == SIN || data == COS || data == LN

#endif // DSL_HPP