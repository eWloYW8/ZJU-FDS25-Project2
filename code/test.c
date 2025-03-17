#include "ast.h"
#include "utils.h"
#include "astmath.h"

#include <stdlib.h>
#include <stdio.h>

int main() {
    char expression[] = "x*ln(x*y)+y*cos(x)+y*sin(2*x)";
    remove_blankspace(expression);
    int current_position = 0;
    Node *node = build_ast(expression, &current_position);
    printf("%s\n", node_to_string(node));
    
    char **variables = find_variables(node);
    for (int i = 0; variables[i] != NULL; i++) {
        printf("%s: ", variables[i]);
        Node *derivative = differentiate(node, variables[i]);
        derivative = simple_simplify(derivative);
        printf("%s\n", node_to_string(derivative));
        Node *derivative_rebuilt = rebuild_ast(derivative);
        printf("%s\n", node_to_string(derivative_rebuilt));
        free_node(derivative_rebuilt);
        free_node(derivative);
        free(variables[i]); 
    }
    free_node(node);
}