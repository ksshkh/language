#include "backend.hpp"

static const char* ASM_FILE = "asm.txt";

void ConvertToAsm(Tree* tree, int* code_error) {

    FILE* printout = fopen(ASM_FILE, "w");
    MY_ASSERT(printout != NULL, FOPEN_ERROR);

    size_t label_id = 0;

    AsmPrint(tree->root, &label_id, printout, code_error);

    MY_ASSERT(fclose(printout) == 0, FCLOSE_ERROR);
}

void AsmPrint(Node* node, size_t* label_id, FILE* stream, int* code_error) {

    MY_ASSERT(stream   != NULL, FILE_ERROR);
    MY_ASSERT(label_id != NULL,  PTR_ERROR);

    if(!node) {
        return;
    }

    if((Operations)node->data == IF) {
        (*label_id)++;
    }
    size_t old_label_id = *label_id;

    AsmPrint(node->left, label_id, stream, code_error);

    AsmPrint(node->right, label_id, stream, code_error);

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
        case IDE: {
            AsmPrintIf(node, &old_label_id, stream, code_error);
            break;
        }
        case FUNC_IDE:
        case PAR:
        default: {
            break;
        }
    }

}

void AsmPrintOp(Node* node, size_t* label_id, FILE* stream, int* code_error) {

    MY_ASSERT(stream   != NULL, FILE_ERROR);
    MY_ASSERT(node     != NULL,  PTR_ERROR);
    MY_ASSERT(label_id != NULL,  PTR_ERROR);

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
        case DEG:
        case SIN:
        case COS:
        case LN:
        case EOT:
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
        case AND:
        default: {
            break;
        }
    }

}

void AsmPrintIf(Node* node, size_t* label_id, FILE* stream, int* code_error) {

    MY_ASSERT(stream   != NULL, FILE_ERROR);
    MY_ASSERT(node     != NULL,  PTR_ERROR);
    MY_ASSERT(label_id != NULL,  PTR_ERROR);

    fprintf(stream, "label%d:\n", *label_id);
    // (*label_id)++;
}

void AsmPrintInequality(Node* node, size_t* label_id, FILE* stream, int* code_error) {

    MY_ASSERT(stream   != NULL, FILE_ERROR);
    MY_ASSERT(node     != NULL,  PTR_ERROR);
    MY_ASSERT(label_id != NULL,  PTR_ERROR);

    switch((Operations)node->data) {
        case EQU: {
            fprintf(stream, "jne label%d\n", *label_id);
            break;
        }
        case NEQ: {
            fprintf(stream, "je label%d\n", *label_id);
            break;
        }
        case AE: {
            fprintf(stream, "jb label%d\n", *label_id);
            break;
        }
        case BE: {
            fprintf(stream, "ja label%d\n", *label_id);
            break;
        }
        case ABOVE: {
            fprintf(stream, "jbe label%d\n", *label_id);
            break;
        }
        case BELOW: {
            fprintf(stream, "jae label%d\n", *label_id);
            break;
        }
        default: {
            break;
        }
    }
}

void AsmPrintAssigment(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);
    MY_ASSERT(node   != NULL,  PTR_ERROR);

    fprintf(stream, "pop [%d]\n", (int)node->left->data);
}

void AsmPrintVar(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);
    MY_ASSERT(node   != NULL,  PTR_ERROR);

    if((Operations)node->parent->data != VAR_S) {
        fprintf(stream, "push [%d]\n", (int)node->data);
    }
}

void AsmPrintNum(TreeElem num, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

    fprintf(stream, "push %d\n", (int)num);
}
