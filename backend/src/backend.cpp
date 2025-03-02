#include "../inc/backend.hpp"

static const char* ASM_FILE = "../CPU/programs/program.txt";

#define SNTX_ERR fprintf(stderr, "syntax error: %.2lf (line %d)\n", node->data, __LINE__); \
                 exit(0);

void ConvertToAsm(Tree* tree, int* code_error) {

    FILE* printout = fopen(ASM_FILE, "w");
    MY_ASSERT(printout != NULL, FOPEN_ERROR);

    size_t label_id = 0;
    size_t loop_id  = 0;

    AsmPrint(tree->root, &label_id, &loop_id, printout, code_error);

    MY_ASSERT(fclose(printout) == 0, FCLOSE_ERROR);
}

void AsmPrint(Node* node, size_t* label_id, size_t* loop_id, FILE* stream, int* code_error) {

    MY_ASSERT(stream   != NULL, FILE_ERROR);
    MY_ASSERT(label_id != NULL,  PTR_ERROR);

    if(!node) {
        return;
    }

    if((Operations)node->data == DEF_OP || (Operations)node->data == WHILE) {
        (*label_id)++;
    }
    size_t old_label_id = *label_id;

    if((Operations)node->data == WHILE) {
        (*loop_id)++;
        fprintf(stream, "loop%ld:\n", *loop_id);
    }
    size_t old_loop_id = *loop_id;

    AsmPrint(node->left, label_id, loop_id, stream, code_error);

    AsmPrint(node->right, label_id, loop_id, stream, code_error);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-enum"

    switch(node->type) {
        case NUM: {
            AsmPrintNum(node->data, stream, code_error);
            break;
        }
        case VAR: {
            AsmPrintVar(node, stream, code_error);
            break;
        }
        case OP: {
            AsmPrintOp(node, label_id, stream, code_error);
            break;
        }
        case DEF_TYPE: {
            if((Operations)node->data == DEF_OP) {
                AsmPrintIf(node, old_label_id, label_id, stream, code_error);
            }
            break;
        }
        case IDE: {
            if((Operations)node->data == WHILE) {
                fprintf(stream, "jmp loop%ld\nlabel%ld:\n", old_loop_id, old_label_id);
            }
            break;
        }
        case FUNC_IDE:
        case PAR: {
            AsmPrintPar(node, stream, code_error);
            break;
        }
        default: {
            break;
        }
    }

    #pragma GCC diagnostic pop

}

void AsmPrintOp(Node* node, size_t* label_id, FILE* stream, int* code_error) {

    MY_ASSERT(stream   != NULL, FILE_ERROR);
    MY_ASSERT(node     != NULL,  PTR_ERROR);
    MY_ASSERT(label_id != NULL,  PTR_ERROR);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-enum"

    switch((Operations)node->data) {
        case ADD: {
            fprintf(stream, "add\n");
            break;
        }
        case SUB: {
            fprintf(stream, "sub\n");
            break;
        }
        case MUL: {
            fprintf(stream, "mul\n");
            break;
        }
        case DIV: {
            fprintf(stream, "div\n");
            break;
        }
        case SIN: {
            fprintf(stream, "sin\n");
            break;
        }
        case COS: {
            fprintf(stream, "cos\n");
            break;
        }
        case LN: {
            fprintf(stream, "ln\n");
            break;
        }
        case SQRT: {
            fprintf(stream, "sqrt\n");
            break;
        }
        case VAR_S: {
            AsmPrintAssigment(node, stream, code_error);
            break;
        }
        case EQU:
        case NEQ:
        case AE:
        case BE:
        case ABOVE:
        case BELOW: {
            AsmPrintInequality(node, label_id, stream, code_error);
            break;
        }
        case OR:
        case AND: {
            break;
        }
        default: {
            SNTX_ERR
            break;
        }
    }

    #pragma GCC diagnostic pop

}

size_t GetIfDepth(Node* node, int* code_error) {

    MY_ASSERT(node != NULL,  PTR_ERROR);

    size_t depth = 0;

    while(node->right) {
        if(((Operations)node->right->data == EL_IF || (Operations)node->right->data == ELSE)) {
            depth++;
        }
        node = node->right;
    }

    return depth;
}

void AsmPrintIf(Node* node, size_t old_label_id, size_t* label_id, FILE* stream, int* code_error) {

    MY_ASSERT(stream   != NULL, FILE_ERROR);
    MY_ASSERT(node     != NULL,  PTR_ERROR);
    MY_ASSERT(label_id != NULL,  PTR_ERROR);

    if((Operations)node->right->data != ELSE) {
        size_t depth = GetIfDepth(node->parent, code_error);
        if(depth) {
            fprintf(stream, "jmp label%ld\n", *label_id + depth);
        }
    }

    fprintf(stream, "label%ld:\n", old_label_id);
}

void AsmPrintInequality(Node* node, size_t* label_id, FILE* stream, int* code_error) {

    MY_ASSERT(stream   != NULL, FILE_ERROR);
    MY_ASSERT(node     != NULL,  PTR_ERROR);
    MY_ASSERT(label_id != NULL,  PTR_ERROR);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-enum"

    switch((Operations)node->data) {
        case EQU: {
            fprintf(stream, "jne label%ld\n", *label_id);
            break;
        }
        case NEQ: {
            fprintf(stream, "je label%ld\n", *label_id);
            break;
        }
        case AE: {
            fprintf(stream, "jb label%ld\n", *label_id);
            break;
        }
        case BE: {
            fprintf(stream, "ja label%ld\n", *label_id);
            break;
        }
        case ABOVE: {
            fprintf(stream, "jbe label%ld\n", *label_id);
            break;
        }
        case BELOW: {
            fprintf(stream, "jae label%ld\n", *label_id);
            break;
        }
        default: {
            break;
        }
    }

    #pragma GCC diagnostic pop
}

void AsmPrintAssigment(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);
    MY_ASSERT(node   != NULL,  PTR_ERROR);

    fprintf(stream, "pop [%d]\n", (int)node->left->data);
}

void AsmPrintVar(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);
    MY_ASSERT(node   != NULL,  PTR_ERROR);

    if((Operations)node->parent->data != VAR_S || (node->parent->left->type == VAR && node == node->parent->right)) {
        fprintf(stream, "push [%d]\n", (int)node->data);
    }
}

void AsmPrintNum(TreeElem num, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

    fprintf(stream, "push %d\n", (int)num);
}

void AsmPrintPar(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);
    MY_ASSERT(node   != NULL,  PTR_ERROR);

    if((Operations)node->parent->data == INPUT) {
        fprintf(stream, "in\npop [%d]\n", (int)node->data);
    }
    else if((Operations)node->parent->data == PRINT) {
        fprintf(stream, "push [%d]\nout\n", (int)node->data);
    }
}

#undef SNTX_ERR
