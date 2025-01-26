#include "input.hpp"

int main(void) {
    Tree tree = {};
    TableName tbl_nm = {};
    int code_error = 0;

    TreeCtor(&tree, &code_error);
    TableNameCtor(&tbl_nm, &code_error);

    TokensParcing(&tree, &(tree.num_of_nodes), &tbl_nm, &code_error);

    for(size_t i = 0; i < tree.size_data_base; i++) {
        if(!tree.tokens[i]) {
            break;
        }
        printf("%d %lf\n", tree.tokens[i]->type, tree.tokens[i]->data);
    }
    printf("Name table:\n");
    for(size_t i = 0; i < tbl_nm.free_id; i++) {
        printf("%d %s %ld\n", i, tbl_nm.table_name[i].name, tbl_nm.table_name[i].length);
    }

    size_t ip = 0;
    tree.root = GetTree(&(tree.num_of_nodes), tree.tokens, &ip, &code_error);
    DotTreeDump(&tree, &code_error);

    // TreeDtor(&tree, &code_error);
    TableNameDtor(&tbl_nm, &code_error);

    return 0;
}