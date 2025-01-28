#include "treedump.hpp"
#include "../utils.hpp"

#define NODE_NUM_COLOR     "\"#8EECF5\""
#define NODE_VAR_COLOR     "\"#98F5E1\""
#define NODE_OP_COLOR      "\"#CFBAF0\""
#define NODE_DEF_COLOR     "\"#FFCFD2\""
#define NODE_IDE_COLOR     "\"#F1C0E8\""
#define NODE_FUNC_COLOR    "\"#A3C4F3\""
#define NODE_BORDER_COLOR  "\"#5F5B6A\""
#define BACKGROUND_COLOR   "\"#FFFEEC\""

static const char* DOT_FILE_NAME       = "../debug/tree.dot";
static const char* IMAGE_NAME          = "../debug/tree_image.svg";
static const char* HTML_FILE_NAME      = "../debug/tree.html";
static const char* OUTPUT_FILE         = "../tree.txt";

void DotTreeDump(Tree* tree, int* code_error) {

    FILE* dot_file = fopen(DOT_FILE_NAME, "w");

    if(dot_file) {
        fprintf(dot_file, "digraph Tree {\n");
        fprintf(dot_file, "\trankdir = TB;\n");
        fprintf(dot_file, "\tnode [shape = record];\n");
        fprintf(dot_file, "\tedge [color = " NODE_BORDER_COLOR "];\n");
        fprintf(dot_file, "\tbgcolor = " BACKGROUND_COLOR ";\n");

        if(tree) {
            if(tree->root) {
                PrintDotNode(tree->root, dot_file);
            }
            else {
                fprintf(stderr, "tree is empty\n");
            }
        }
        else {
            fprintf(stderr, "no tree\n");
        }

        fprintf(dot_file, "}\n");

        if(fclose(dot_file)) {
            fprintf(stderr, "file did not close\n");
        }

        GraphCreate();
        HtmlDump(code_error);
    }
    else {
        fprintf(stderr, "file did not open\n");
    }
}

void PrintDotNode(Node* node, FILE* stream) {

    if(!node) return;

    if(node->type == NUM) {
        fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_NUM_COLOR ", label = \"{%.2lf}\"];\n", node, node->data);
    }
    else if(node->type == OP) {
        fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_OP_COLOR ", label = \"{", node);
        switch((Operations)node->data) {
            case EQU: {
                fprintf(stream, "==");
                break;
            }
            case NEQ: {
                fprintf(stream, "!=");
                break;
            }
            case ABOVE: {
                fprintf(stream, "&gt;");
                break;
            }
            case BELOW: {
                fprintf(stream, "&lt;");
                break;
            }
            case AE: {
                fprintf(stream, "&gt;=");
                break;
            }
            case BE: {
                fprintf(stream, "&lt;=");
                break;
            }
            case AND: {
                fprintf(stream, "&&");
                break;
            }
            case OR: {
                fprintf(stream, "\\| \\|");
                break;
            }
            case COS: {
                fprintf(stream, "cos");
                break;
            }
            case SIN: {
                fprintf(stream, "sin");
                break;
            }
            case LN: {
                fprintf(stream, "ln");
                break;
            }
            default: {
                fprintf(stream, "%c", (char)node->data);
                break;
            }
        }
        fprintf(stream, "}\"];\n");
    }
    else if(node->type == DEF_TYPE) {
        fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_DEF_COLOR ", label = \"{", node);
        switch((Operations)node->data) {
            case DEF_OP: {
                fprintf(stream, "no op");
                break;
            }
            default: {
                fprintf(stream, "%c", (char)node->data);
                break;
            }
        }
        fprintf(stream, "}\"];\n");
    }
    else if(node->type == IDE) {
        fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_IDE_COLOR ", label = \"{", node);
        switch((Operations)node->data) {
            case IF: {
                fprintf(stream, "if");
                break;
            }
            case WHILE: {
                fprintf(stream, "while");
                break;
            }
            case ELSE: {
                fprintf(stream, "else");
                break;
            }
            case EL_IF: {
                fprintf(stream, "else_if");
                break;
            }
            default: {
                fprintf(stream, "%c", (char)node->data);
                break;
            }
        }
        fprintf(stream, "}\"];\n");
    }
    else if(node->type == FUNC_IDE) {
        fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_FUNC_COLOR ", label = \"{%d}\"];\n", node, (int)node->data);
    }
    else {
        fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_VAR_COLOR ", label = \"{%d}\"];\n", node, (int)node->data);
    }


    if(node->left) {
        fprintf(stream, "\t\tnode%p -> node%p\n", node, node->left);
        PrintDotNode(node->left,  stream);
    }

    if(node->right) {
        fprintf(stream, "\t\tnode%p -> node%p\n", node, node->right);
        PrintDotNode(node->right, stream);
    }

}

void GraphCreate(void) {

    char command[] = "dot -Tsvg /home/ksenia/huawei/language/debug/tree.dot -o /home/ksenia/huawei/language/debug/tree_image.svg";
    system(command);
}

void HtmlDump(int *code_error) {

    FILE* html = fopen(HTML_FILE_NAME, "a");
    MY_ASSERT(html != NULL, FOPEN_ERROR);

    long int image_size = 0;

    char *image_data = ReadInBuff(IMAGE_NAME, &image_size, code_error);
    MY_ASSERT(image_data != NULL, PTR_ERROR);

    fprintf(html, "%s\n", image_data);

    MY_ASSERT(fclose(html) == 0, FCLOSE_ERROR);
}

void PrintTree(Tree* tree, int* code_error) {

    TREE_ASSERT(tree);

    FILE* printout = fopen(OUTPUT_FILE, "w");
    MY_ASSERT(printout != NULL, FOPEN_ERROR);

    PreorderPrinting(tree->root, printout, code_error);

    MY_ASSERT(fclose(printout) == 0, FCLOSE_ERROR);
}

void PreorderPrinting(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

    if(!node) {
        fprintf(stream, "_ ");
        return;
    }

    fprintf(stream, "(");

    fprintf(stream, "%d ", node->type);
    if (node->type == NUM) {
        fprintf(stream, "%.2lf ", node->data);
    }
    else {
        fprintf(stream, "%d ", (int)node->data);
    }
    // else {
    //     switch((Operations)node->data) {
    //         case SIN: {
    //             fprintf(stream, " sin ");
    //             break;
    //         }
    //         case COS: {
    //             fprintf(stream, " cos ");
    //             break;
    //         }
    //         case LN: {
    //             fprintf(stream, " ln ");
    //             break;
    //         }
    //         default: {
    //             fprintf(stream, " %c ", (int)node->data);
    //         }
    //     }
    // }

    PreorderPrinting(node->left, stream, code_error);
    PreorderPrinting(node->right, stream, code_error);

    fprintf(stream, ")");

}

void InorderPrinting(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

    if(!node) {
        return;
    }

    fprintf(stream, "(");

    InorderPrinting(node->left, stream, code_error);

    if (node->type == NUM) {
        fprintf(stream, " %.2lf ", node->data);
    }
    else {
        switch((Operations)node->data) {
            case SIN: {
                fprintf(stream, " sin ");
                break;
            }
            case COS: {
                fprintf(stream, " cos ");
                break;
            }
            case LN: {
                fprintf(stream, " ln ");
                break;
            }
            default: {
                fprintf(stream, " %c ", (int)node->data);
            }
        }
    }

    InorderPrinting(node->right, stream, code_error);

    fprintf(stream, ")");

}

#undef NODE_NUM_COLOR
#undef NODE_BORDER_COLOR
#undef BACKGROUND_COLOR
#undef NODE_DEF_COLOR
#undef NODE_IDE_COLOR
#undef NODE_VAR_COLOR
#undef NODE_OP_COLOR
#undef NODE_FUNC_COLOR