#include "recursivedes.hpp"

#define SNTX_ERR fprintf(stderr, "syntax error: %.2lf (line: %d)\n", tokens[*ip]->data, __LINE__); \
                 exit(0);

Node* GetTree(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* val = GetOp(num_of_nodes, tokens, ip, code_error);

    if(tokens[*ip]->data != EOT) {SNTX_ERR}
    (*ip)++;

    return val;
}

Node* GetOp(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    if((Operations)tokens[*ip]->data == IF) {
        (*ip)++;

        Node* node       = GetIf(num_of_nodes, tokens, ip, code_error);
        Node* right_node = GetOp(num_of_nodes, tokens, ip, code_error);
        Node* left_node  = node;

        node = _SEM(left_node, right_node);

        return node;
    }
    else if((Operations)tokens[*ip]->data == WHILE) {
        (*ip)++;

        Node* node       = GetWhile(num_of_nodes, tokens, ip, code_error);
        Node* right_node = GetOp(num_of_nodes, tokens, ip, code_error);
        Node* left_node  = node;

        node = _SEM(left_node, right_node);

        return node;
    }
    else if((Operations)tokens[*ip]->type == FUNC_IDE) {

        Node* node       = GetFuncCall(num_of_nodes, tokens, ip, code_error);

        if((Operations)tokens[*ip]->data != SEM) {SNTX_ERR}
        (*ip)++;

        Node* right_node = GetOp(num_of_nodes, tokens, ip, code_error);
        Node* left_node  = node;

        node = _SEM(left_node, right_node);

        return node;
    }
    else if((Operations)tokens[*ip]->data != EOT && (Operations)tokens[*ip]->data != R_FBR){
        Node* node = GetAssign(num_of_nodes, tokens, ip, code_error);

        if((Operations)tokens[*ip]->data != SEM) {SNTX_ERR}
        (*ip)++;

        Node* right_node = GetOp(num_of_nodes, tokens, ip, code_error);
        Node* left_node  = node;

        node = _SEM(left_node, right_node);

        return node;
    }
    else {
        return NULL;
    }

}

Node* GetIf(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    if((Operations)tokens[*ip]->data != L_BR) {SNTX_ERR}
    (*ip)++;

    Node* cond_node = GetCond(num_of_nodes, tokens, ip, code_error);

    if((Operations)tokens[*ip]->data != R_BR) {SNTX_ERR}
    (*ip)++;

    if((Operations)tokens[*ip]->data != L_FBR) {SNTX_ERR}
    (*ip)++;

    Node* node = GetOp(num_of_nodes, tokens, ip, code_error);

    if((Operations)tokens[*ip]->data != R_FBR) {SNTX_ERR}
    (*ip)++;

    Node* if_node   = _DEF_OP(cond_node, node);
    Node* else_node = GetElseIf(num_of_nodes, tokens, ip, code_error);
    Node* res_node = _IF(if_node, else_node);

    return res_node;
}

Node* GetElseIf(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* node = NULL;

    while((Operations)tokens[*ip]->data == EL_IF) {
        (*ip)++;

        if((Operations)tokens[*ip]->data != L_BR) {SNTX_ERR}
        (*ip)++;

        Node* cond_node = GetCond(num_of_nodes, tokens, ip, code_error);

        if((Operations)tokens[*ip]->data != R_BR) {SNTX_ERR}
        (*ip)++;

        if((Operations)tokens[*ip]->data != L_FBR) {SNTX_ERR}
        (*ip)++;

        Node* main_node = GetOp(num_of_nodes, tokens, ip, code_error);

        if((Operations)tokens[*ip]->data != R_FBR) {SNTX_ERR}
        (*ip)++;

        Node* el_if_node = _DEF_OP(cond_node, main_node);
        Node* else_node  = GetElseIf(num_of_nodes, tokens, ip, code_error);

        node = _EL_IF(el_if_node, else_node);
    }

    if((Operations)tokens[*ip]->data == ELSE) {
        (*ip)++;

        if((Operations)tokens[*ip]->data != L_FBR) {SNTX_ERR}
        (*ip)++;

        Node* else_node = GetOp(num_of_nodes, tokens, ip, code_error);
        node = _ELSE(else_node, NULL);

        if((Operations)tokens[*ip]->data != R_FBR) {SNTX_ERR}
        (*ip)++;
    }

    return node;
}

Node* GetWhile(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    if((Operations)tokens[*ip]->data != L_BR) {SNTX_ERR}
    (*ip)++;

    Node* cond_node = GetCond(num_of_nodes, tokens, ip, code_error);

    if((Operations)tokens[*ip]->data != R_BR) {SNTX_ERR}
    (*ip)++;

    if((Operations)tokens[*ip]->data != L_FBR) {SNTX_ERR}
    (*ip)++;

    Node* node = GetOp(num_of_nodes, tokens, ip, code_error);

    if((Operations)tokens[*ip]->data != R_FBR) {SNTX_ERR}
    (*ip)++;

    Node* res_node = _WHILE(cond_node, node);

    return res_node;
}

Node* GetAssign(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    if(tokens[*ip]->type == VAR) {
        Node* left_node  = GetVar(num_of_nodes, tokens, ip, code_error);

        if(tokens[*ip]->data != VAR_S) {SNTX_ERR}
        (*ip)++;

        Node* right_node = NULL;

        if(tokens[*ip]->type == FUNC_IDE && (Operations)tokens[*ip]->data != PRINT && (Operations)tokens[*ip]->data != INPUT) {
            right_node = GetFuncCall(num_of_nodes, tokens, ip, code_error);
        }
        else {
            right_node = GetAddAndSub(num_of_nodes, tokens, ip, code_error);
        }

        return _VAR_S(left_node, right_node);
    }

    return GetAddAndSub(num_of_nodes, tokens, ip, code_error);
}

Node* GetFuncCall(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Operations op = (Operations)tokens[*ip]->data;
    (*ip)++;

    if((Operations)tokens[*ip]->data != L_BR) {SNTX_ERR}
    (*ip)++;

    Node* param_node = GetParam(num_of_nodes, tokens, ip, code_error);

    if((Operations)tokens[*ip]->data != R_BR) {SNTX_ERR}
    (*ip)++;

    switch(op) {
            case PRINT: {
                return _PRINT(param_node);
            }
            case INPUT: {
                return _INPUT(param_node);
            }
            default: {
                return _FCALL(op, param_node);
            }
        }
}

Node* GetAddAndSub(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* node = GetMulAndDiv(num_of_nodes, tokens, ip, code_error);

    while((Operations)tokens[*ip]->data == ADD || (Operations)tokens[*ip]->data == SUB) {
        Operations op = (Operations)tokens[*ip]->data;
        (*ip)++;

        Node* right_node = GetMulAndDiv(num_of_nodes, tokens, ip, code_error);
        Node* left_node  = node;

        switch(op) {
            case ADD: {
                node = _ADD(left_node, right_node);
                break;
            }
            case SUB: {
                node = _SUB(left_node, right_node);
                break;
            }
            default: {
                break;
            }
        }
    }

    return node;
}

Node* GetMulAndDiv(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* node = GetDeg(num_of_nodes, tokens, ip, code_error);

    while((Operations)tokens[*ip]->data == MUL || (Operations)tokens[*ip]->data == DIV) {
        Operations op = (Operations)tokens[*ip]->data;
        (*ip)++;

        Node* right_node = GetDeg(num_of_nodes, tokens, ip, code_error);
        Node* left_node  = node;

        switch(op) {
            case MUL: {
                node = _MUL(left_node, right_node);
                break;
            }
            case DIV: {
                node = _DIV(left_node, right_node);
                break;
            }
            default: {
                (*ip)++;
                break;
            }
        }
    }

    return node;
}

Node* GetDeg(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* node = GetUnaryOp(num_of_nodes, tokens, ip, code_error);

    while((Operations)tokens[*ip]->data == DEG) {
        Operations op = (Operations)tokens[*ip]->data;
        (*ip)++;

        Node* node_right = GetUnaryOp(num_of_nodes, tokens, ip, code_error);
        Node* node_left  = node;

        switch(op) {
            case DEG: {
                node = _DEG(node_left, node_right);
                break;
            }
            default: {
                break;
            }
        }
    }

    return node;

}

Node* GetUnaryOp(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    while(tokens[*ip]->type == OP && (Operations)tokens[*ip]->data != L_BR && (Operations)tokens[*ip]->data != R_BR) {

        Operations op = (Operations)tokens[*ip]->data;
        (*ip)++;

        Node* left_node = GetBrackets(num_of_nodes, tokens, ip, code_error);

        switch(op) {
            case SIN: {
                return _SIN(left_node);
            }
            case COS: {
                return _COS(left_node);
            }
            case LN: {
                return _LN(left_node);
            }
            case SQRT: {
                return _SQRT(left_node);
            }
            default: {
                break;
            }
        }

    }
    Node* node = GetBrackets(num_of_nodes, tokens, ip, code_error);

    return node;
}

Node* GetBrackets(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    if((Operations)tokens[*ip]->data == L_BR) {
        (*ip)++;
        Node* node = GetAddAndSub(num_of_nodes, tokens, ip, code_error);

        if((Operations)tokens[*ip]->data != R_BR) {SNTX_ERR}
        (*ip)++;

        return node;
    }
    else if(tokens[*ip]->type == VAR) {
        return GetVar(num_of_nodes, tokens, ip, code_error);
    }
    else {
        return GetNum(num_of_nodes, tokens, ip, code_error);
    }
}

Node* GetCond(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* node = GetComp(num_of_nodes, tokens, ip, code_error);

    while((Operations)tokens[*ip]->data == AND || (Operations)tokens[*ip]->data == OR) {
        Operations op = (Operations)tokens[*ip]->data;
        (*ip)++;

        Node* right_node = GetComp(num_of_nodes, tokens, ip, code_error);
        Node* left_node  = node;

        switch(op) {
            case AND: {
                node = _AND(left_node, right_node);
                break;
            }
            case OR: {
                node = _OR(left_node, right_node);
                break;
            }
            default: {
                (*ip)++;
                break;
            }
        }
    }

    return node;
}

Node* GetComp(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    Node* left_node = GetAddAndSub(num_of_nodes, tokens, ip, code_error);

    Operations op = (Operations)tokens[*ip]->data;
    (*ip)++;

    Node* right_node = GetAddAndSub(num_of_nodes, tokens, ip, code_error);

    switch(op) {
        case EQU: {
            return _EQU(left_node, right_node);
        }
        case NEQ: {
            return _NEQ(left_node, right_node);
        }
        case AE: {
            return _AE(left_node, right_node);
        }
        case BE: {
            return _BE(left_node, right_node);
        }
        case ABOVE: {
            return _ABO(left_node, right_node);
        }
        case BELOW: {
            return _BEL(left_node, right_node);
        }
        default: {
            SNTX_ERR
        }
    }
}

Node* GetNum(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    size_t old_ip = *ip;

    Node* ret_node = tokens[*ip];
    if(tokens[*ip]->type == NUM) (*ip)++;

    if(old_ip == *ip) {SNTX_ERR}

    return ret_node;
}

Node* GetVar(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    size_t old_ip = *ip;

    Node* ret_node = tokens[*ip];
    if(tokens[*ip]->type == VAR) (*ip)++;

    if(old_ip == *ip) {SNTX_ERR}

    return ret_node;
}

Node* GetParam(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    size_t old_ip = *ip;

    Node* ret_node = tokens[*ip];
    if(tokens[*ip]->type == PAR) (*ip)++;

    if(old_ip == *ip) {SNTX_ERR}

    return ret_node;
}

#undef SNTX_ERROR