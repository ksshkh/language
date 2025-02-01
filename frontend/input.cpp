#include "input.hpp"

#define TOKEN_NO_EXIST fprintf(stderr, "token error: %c (line %d)\n", (char)tree->data_base[data_base_ip], __LINE__); \
                       exit(0);

Token tkns[NUM_OF_TOKENS] = {{"sin",       SIN},
                             {"cos",       COS},
                             {"ln",         LN},
                             {"if",         IF},
                             {"while",   WHILE},
                             {"def",       DEF},
                             {"else",     ELSE},
                             {"else_if", EL_IF},
                             {"input",   INPUT},
                             {"print",   PRINT},
                             {"sqrt",     SQRT}};

static const char* TABLE_NAME_FILE = "../frontend/table_name.txt";

void TokensParcing(Tree* tree, size_t* num_of_nodes, TableName* tbl_nm, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    size_t data_base_ip = 0;
    size_t tokens_ip    = 0;

    for(data_base_ip = 0; (long int)data_base_ip < tree->size_data_base; tokens_ip++) {

        while(isspace(tree->data_base[data_base_ip])) {
            data_base_ip++;
        }

        if(tree->data_base[data_base_ip] == L_BR) {
            tree->tokens[tokens_ip] = _L_BR;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == R_BR) {
            tree->tokens[tokens_ip] = _R_BR;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == L_FBR) {
            tree->tokens[tokens_ip] = _L_FBR;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == R_FBR) {
            tree->tokens[tokens_ip] = _R_FBR;
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == SEM) {
            tree->tokens[tokens_ip] = _SEM(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == VAR_S && data_base_ip < tree->size_data_base - 1 && tree->data_base[data_base_ip + 1] != VAR_S) {
            tree->tokens[tokens_ip] = _VAR_S(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == ADD) {
            tree->tokens[tokens_ip] = _ADD(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == SUB) {
            tree->tokens[tokens_ip] = _SUB(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == MUL) {
            tree->tokens[tokens_ip] = _MUL(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == DIV) {
            tree->tokens[tokens_ip] = _DIV(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == DEG) {
            tree->tokens[tokens_ip] = _DEG(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '=') {
            data_base_ip++;
            if(tree->data_base[data_base_ip] != '=') {TOKEN_NO_EXIST}
            tree->tokens[tokens_ip] = _EQU(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '!') {
            data_base_ip++;
            if(tree->data_base[data_base_ip] != '=') {TOKEN_NO_EXIST}
            tree->tokens[tokens_ip] = _NEQ(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '|') {
            data_base_ip++;
            if(tree->data_base[data_base_ip] != '|') {TOKEN_NO_EXIST}
            tree->tokens[tokens_ip] = _OR(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '&') {
            data_base_ip++;
            if(tree->data_base[data_base_ip] != '&') {TOKEN_NO_EXIST}
            tree->tokens[tokens_ip] = _AND(NULL, NULL);
            data_base_ip++;
        }
        else if(tree->data_base[data_base_ip] == '>') {
            if(tree->data_base[data_base_ip + 1] == '=') {
                tree->tokens[tokens_ip] = _AE(NULL, NULL);
                data_base_ip += 2;
            }
            else {
                tree->tokens[tokens_ip] = _ABO(NULL, NULL);
                data_base_ip++;
            }
        }
        else if(tree->data_base[data_base_ip] == '<') {
            if(tree->data_base[data_base_ip + 1] == '=') {
                tree->tokens[tokens_ip] = _BE(NULL, NULL);
                data_base_ip += 2;
            }
            else {
                tree->tokens[tokens_ip] = _BEL(NULL, NULL);
                data_base_ip++;
            }
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

            while(isalpha(tree->data_base[data_base_ip]) || tree->data_base[data_base_ip] == '_') {
                length++;
                data_base_ip++;
            }

            Operations op_code = FindToken(begin, length, code_error);

            switch(op_code) {
                case DEF_OP: {
                    int found_id = FindName(tbl_nm, begin, length, code_error);

                    if(found_id != DEF_IP) {
                        if(tokens_ip >= 2 && (Type)tree->tokens[tokens_ip - 2]->type == FUNC_IDE) {
                            tree->tokens[tokens_ip] = _PAR((TreeElem)found_id);
                        }
                        else if(tbl_nm->table_name[found_id].type == FUNC_IDE) {
                            tree->tokens[tokens_ip] = _FUNC_IDE((TreeElem)found_id);
                        }
                        else {
                            tree->tokens[tokens_ip] = _VAR((TreeElem)found_id);
                        }
                    }
                    else {
                        if(tokens_ip && (Operations)tree->tokens[tokens_ip - 1]->data == DEF) {
                            AddNewName(tbl_nm, begin, length, FUNC_IDE, code_error);
                            tree->tokens[tokens_ip] = _FUNC_IDE((TreeElem)(tbl_nm->free_id - 1));
                        }
                        else {
                            AddNewName(tbl_nm, begin, length, VAR, code_error);
                            if(tokens_ip >= 2 && (Type)tree->tokens[tokens_ip - 2]->type == FUNC_IDE) {
                                tree->tokens[tokens_ip] = _PAR((TreeElem)(tbl_nm->free_id - 1));
                            }
                            else {
                                tree->tokens[tokens_ip] = _VAR((TreeElem)(tbl_nm->free_id - 1));
                            }
                        }
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
                case SQRT: {
                    tree->tokens[tokens_ip] = _SQRT(NULL);
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
                case ELSE: {
                    tree->tokens[tokens_ip] = _ELSE(NULL, NULL);
                    break;
                }
                case EL_IF: {
                    tree->tokens[tokens_ip] = _EL_IF(NULL, NULL);
                    break;
                }
                case INPUT: {
                    tree->tokens[tokens_ip] = _INPUT(NULL);
                    break;
                }
                case PRINT: {
                    tree->tokens[tokens_ip] = _PRINT(NULL);
                    break;
                }
                case DEF: {
                    tree->tokens[tokens_ip] = _DEF;
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    tree->tokens[tokens_ip] = _EOT;
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

void AddNewName(TableName* tbl_nm, char* name, size_t length, Type type, int* code_error) {

    MY_ASSERT(tbl_nm != NULL, PTR_ERROR);
    MY_ASSERT(name   != NULL, PTR_ERROR);

    if(tbl_nm->free_id == tbl_nm->num_of_names) {
        tbl_nm->num_of_names *= 2;
        tbl_nm->table_name = (Name*)realloc(tbl_nm->table_name, tbl_nm->num_of_names * sizeof(Name));
        MY_ASSERT(tbl_nm != NULL, PTR_ERROR);
    }

    tbl_nm->table_name[tbl_nm->free_id].name   = name;
    tbl_nm->table_name[tbl_nm->free_id].length = length;
    tbl_nm->table_name[tbl_nm->free_id].type   = type;

    tbl_nm->free_id++;
}

void TableNameDtor(TableName* tbl_nm, int* code_error) {

    MY_ASSERT(tbl_nm != NULL, PTR_ERROR);

    tbl_nm->free_id = 0;
    tbl_nm->num_of_names = 0;

    free(tbl_nm->table_name);
    tbl_nm->table_name = NULL;
}

void PrintTableName(TableName* tbl_nm, int* code_error) {

    MY_ASSERT(tbl_nm != NULL, PTR_ERROR);

    FILE* printout = fopen(TABLE_NAME_FILE, "w");
    MY_ASSERT(printout != NULL, FOPEN_ERROR);

    for(size_t i = 0; i < tbl_nm->free_id; i++) {
        fprintf(printout, "%ld ", i);
        for(size_t j = 0; j < tbl_nm->table_name[i].length; j++) {
            fprintf(printout, "%c", *(tbl_nm->table_name[i].name + j));
        }
        fprintf(printout, " %d\n", tbl_nm->table_name[i].type);
    }

    MY_ASSERT(fclose(printout) == 0, FCLOSE_ERROR);

}