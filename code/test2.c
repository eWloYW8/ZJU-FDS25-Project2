#include "ast.h"
#include "utils.h"
#include "astmath.h"
#include "simplificator/one_layer.h"
#include "simplificator/monomial.h"

#include <stdlib.h>
#include <stdio.h>

int main() {
    char expression[] = "x^4/y^3*sin(x*y)/x/y";
    remove_blankspace(expression);
    int current_position = 0;
    Node *node = build_ast(expression, &current_position);
    node = simple_simplify(node);
    printf("%s\n", node_to_string(node));
    node = monomial_ast_rebuild(node);
    printf("%s\n", node_to_string(node));
    
    free_node(node);
}