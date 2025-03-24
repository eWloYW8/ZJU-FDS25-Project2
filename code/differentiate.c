#include "ast.h"
#include "utils.h"
#include "astmath.h"
#include "monomial.h"

#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <expression> [<variable(default all)>]\n", argv[0]);
        return 1;
    }
    else if (argc == 2) {   
        char *expression = argv[1];
        remove_blankspace(expression);
        int current_position = 0;
        Node *node = build_ast(expression, &current_position);
        // printf("%s\n", node_to_string(node));
        
        char **variables = find_variables(node);
        for (int i = 0; variables[i] != NULL; i++) {
            printf("%s: ", variables[i]);
            Node *derivative = differentiate(node, variables[i]);
            printf("%s\n", node_to_string(derivative));
            free_node(derivative);
            free(variables[i]); 
        }
        free_node(node);
    }
    else if (argc == 3) {
        char *expression = argv[1];
        char *variable = argv[2];
        remove_blankspace(expression);
        int current_position = 0;
        Node *node = build_ast(expression, &current_position);
        // printf("%s\n", node_to_string(node));
        
        Node *derivative = differentiate(node, variable);
        printf("%s\n", node_to_string(derivative));
        free_node(derivative);
        free_node(node);
    }
}