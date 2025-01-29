#include "backend.hpp"

static const char* ASM_FILE = "asm.txt";

void ConvertToAsm(Tree* tree, int* code_error) {

    FILE* printout = fopen(ASM_FILE, "w");
    MY_ASSERT(printout != NULL, FOPEN_ERROR);

    AsmPrint(tree->root, printout, code_error);

    MY_ASSERT(fclose(printout) == 0, FCLOSE_ERROR);
}

void AsmPrint(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

    if(!node) {
        return;
    }

    AsmPrint(node->left, stream, code_error);

    AsmPrint(node->right, stream, code_error);

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
            AsmPrintOp(node, stream, code_error);
            break;
        }
        case IDE:
        case FUNC_IDE:
        case PAR:
        default: {
            break;
        }
    }

}

void AsmPrintOp(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

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
        case BELOW:
        case OR:
        case AND:
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

    if((Operations)node->parent->data != VAR_S) {
        fprintf(stream, "push [%d]\n", (int)node->data);
    }
}

void AsmPrintNum(TreeElem num, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

    fprintf(stream, "push %d\n", (int)num);
}
