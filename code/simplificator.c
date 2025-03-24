#include "ast.h"
#include "utils.h"
#include "one_layer.h"
#include "monomial.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <expression>\n", argv[0]);
        return 1;
    }
    char *expression = argv[1];
    remove_blankspace(expression);
    int current_position = 0;
    Node *node = build_ast(expression, &current_position);
    
    node = simple_simplify(node);
    node = monomial_simplify_recursive(node);
    node = simple_simplify(node);
    printf("%s\n", node_to_string(node));

    free_node(node);
}