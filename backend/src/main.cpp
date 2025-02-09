#include "../inc/backend.hpp"

static const char* INPUT_TREE_FILE = "../tree.txt";

int main(void) {
    Tree tree = {};
    int code_error = 0;

    TreeCtor(&tree, INPUT_TREE_FILE, &code_error);
    ReadTree(&tree, &code_error);
    DotTreeDump(&tree, &code_error);
    ConvertToAsm(&tree, &code_error);

    return 0;
}