#include "input.hpp"

#define SNTX_ERR fprintf(stderr, "syntax error: %c (line: %d)\n", (char)tokens[*ip]->data, __LINE__); \
                 exit(0);

Token tkns[NUM_OF_TOKENS] = {{"sin",     SIN},
                             {"cos",     COS},
                             {"ln",       LN},
                             {"if",       IF},
                             {"while", WHILE}};

void TokensParcing(Tree* tree, size_t* num_of_nodes, TableName* tbl_nm, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    size_t data_base_ip = 0;
    size_t tokens_ip    = 0;

    for(data_base_ip = 0; (long int)data_base_ip < tree->size_data_base; tokens_ip++) {

        while(isspace(tree->data_base[data_base_ip])) {
            data_base_ip++;
        }

        if(tree->data_base[data_base_ip] == '$') {
            tree->tokens[tokens_ip] = _EOT;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == 'x') {
            tree->tokens[tokens_ip] = _VAR(tree->data_base[data_base_ip]);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '(') {
            tree->tokens[tokens_ip] = _L_BR;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == ')') {
            tree->tokens[tokens_ip] = _R_BR;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == ';') {
            tree->tokens[tokens_ip] = _SEM;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '+') {
            tree->tokens[tokens_ip] = _ADD(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '-') {
            tree->tokens[tokens_ip] = _SUB(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '*') {
            tree->tokens[tokens_ip] = _MUL(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '/') {
            tree->tokens[tokens_ip] = _DIV(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '^') {
            tree->tokens[tokens_ip] = _DEG(NULL, NULL);
            data_base_ip++;
        }
        else if('0' <= tree->data_base[data_base_ip] && tree->data_base[data_base_ip] <= '9') {

            double num = 0;
            char* begin = tree->data_base + data_base_ip;

            while(('0' <= tree->data_base[data_base_ip] && tree->data_base[data_base_ip] <= '9') || tree->data_base[data_base_ip] == '.') {
                data_base_ip++;
            }

            char old_sym = tree->data_base[data_base_ip];

            tree->data_base[data_base_ip] = '\0';

            num = strtod(begin, NULL);

            tree->data_base[data_base_ip] = old_sym;

            tree->tokens[tokens_ip] = _NUM(num);
        }
        else {

            char* begin = tree->data_base + data_base_ip;
            size_t length = 0;

            while(isalpha(tree->data_base[data_base_ip])) {
                length++;
                data_base_ip++;
            }

            Operations op_code = FindToken(begin, length, code_error);

            switch(op_code) {
                case DEF_OP: {
                    int found_id = FindName(tbl_nm, begin, length, code_error);
                    if(found_id != DEF_IP) {
                        tree->tokens[tokens_ip] = _VAR(found_id);
                    }
                    else {
                        AddNewName(tbl_nm, begin, length, code_error);
                        tree->tokens[tokens_ip] = _VAR(tbl_nm->free_id - 1);
                    }
                    break;
                }
                case SIN: {
                    tree->tokens[tokens_ip] = _SIN(NULL);
                    break;
                }
                case COS: {
                    tree->tokens[tokens_ip] = _COS(NULL);
                    break;
                }
                case LN: {
                    tree->tokens[tokens_ip] = _LN(NULL);
                    break;
                }
                case IF: {
                    tree->tokens[tokens_ip] = _IF(NULL, NULL);
                    break;
                }
                case WHILE: {
                    tree->tokens[tokens_ip] = _WHILE(NULL, NULL);
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }

}

Operations FindToken(char* word, size_t length, int* code_error) {

    MY_ASSERT(word != NULL, PTR_ERROR);

    for(size_t i = 0; i < NUM_OF_TOKENS; i++) {
        if(!strncmp(word, tkns[i].name, length)) {
            return tkns[i].code;
        }
    }

    return DEF_OP;
}

int FindName(TableName* tbl_nm, char* name, size_t length, int* code_error) {

    MY_ASSERT(tbl_nm != NULL, PTR_ERROR);
    MY_ASSERT(name   != NULL, PTR_ERROR);

    for(size_t i = 0; i < tbl_nm->free_id; i++) {
        if(length == tbl_nm->table_name[i].length) {
            if(!strncmp(name, tbl_nm->table_name[i].name, length)) {
                return (int)i;
            }
        }
    }

    return DEF_IP;
}

void TableNameCtor(TableName* tbl_nm, int* code_error) {

    MY_ASSERT(tbl_nm != NULL, PTR_ERROR);

    tbl_nm->free_id = 0;
    tbl_nm->num_of_names = INIT_NUM_OF_NAMES;

    tbl_nm->table_name = (Name*)calloc(INIT_NUM_OF_NAMES, sizeof(Name));
}

void AddNewName(TableName* tbl_nm, char* name, size_t length, int* code_error) {

    MY_ASSERT(tbl_nm != NULL, PTR_ERROR);
    MY_ASSERT(name   != NULL, PTR_ERROR);

    if(tbl_nm->free_id == tbl_nm->num_of_names) {
        tbl_nm->num_of_names *= 2;
        tbl_nm->table_name = (Name*)realloc(tbl_nm->table_name, tbl_nm->num_of_names * sizeof(Name));
        MY_ASSERT(tbl_nm != NULL, PTR_ERROR);
    }

    tbl_nm->table_name[tbl_nm->free_id].name   = name;
    tbl_nm->table_name[tbl_nm->free_id].length = length;

    tbl_nm->free_id++;
}

void TableNameDtor(TableName* tbl_nm, int* code_error) {

    MY_ASSERT(tbl_nm != NULL, PTR_ERROR);

    tbl_nm->free_id = 0;
    tbl_nm->num_of_names = 0;

    free(tbl_nm->table_name);
    tbl_nm->table_name = NULL;
}

Node* GetTree(size_t* num_of_nodes, Node** tokens, size_t* ip, int* code_error) {

    MY_ASSERT(num_of_nodes != NULL, PTR_ERROR);
    MY_ASSERT(tokens       != NULL, PTR_ERROR);
    MY_ASSERT(ip           != NULL, PTR_ERROR);

    if(tokens[*ip]->data != EOT) {SNTX_ERR}
    (*ip)++;

    Node* val = GetAddAndSub(num_of_nodes, tokens, ip, code_error);

    if(tokens[*ip]->data != EOT) {SNTX_ERR}
    (*ip)++;

    return val;
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

#undef SNTX_ERROR