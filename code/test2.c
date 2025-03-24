#include "ast.h"
#include "utils.h"
#include "astmath.h"
#include "one_layer.h"
#include "monomial.h"

#include <stdlib.h>
#include <stdio.h>

int main() {
    char expression[] = "x/(x*(y-(x-y)))";
    remove_blankspace(expression);
    int current_position = 0;
    Node *node = build_ast(expression, &current_position);
    printf("%s\n", node_to_string(node));
    
    node = simple_simplify(node);
    node = monomial_simplify_recursive(node);
    node = simple_simplify(node);
    printf("%s\n", node_to_string(node));

    free_node(node);
}