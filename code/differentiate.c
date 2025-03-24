#include "ast.h"
#include "utils.h"
#include "astmath.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // Check if there are enough arguments
    if (argc < 2) {
        // Print usage message if not enough arguments
        printf("Usage: %s <expression> [<variable(default all)>]\n", argv[0]);
        return 1;
    }
    else if (argc == 2) {   
        // If only expression is provided
        char *expression = argv[1];
        expression = expadd0(expression); // Modify the expression to handle negative numbers
        remove_blankspace(expression); // Remove any blank spaces from the expression
        int current_position = 0;
        Node *node = build_ast(expression, &current_position); // Build AST from the expression
        
        char **variables = find_variables(node); // Find all variables in the expression
        for (int i = 0; variables[i] != NULL; i++) {
            printf("%s: ", variables[i]);
            Node *derivative = differentiate(node, variables[i]); // Differentiate with respect to each variable
            printf("%s\n", node_to_string(derivative)); // Print the derivative
            free_node(derivative); // Free the derivative node
            free(variables[i]); // Free the variable string
        }
        free_node(node); // Free the original AST node
    }
    else if (argc == 3) {
        // If expression and specific variable are provided
        char *expression = argv[1];
        expression = expadd0(expression); // Modify the expression to handle negative numbers
        char *variable = argv[2];
        remove_blankspace(expression); // Remove any blank spaces from the expression
        int current_position = 0;
        Node *node = build_ast(expression, &current_position); // Build AST from the expression
        
        Node *derivative = differentiate(node, variable); // Differentiate with respect to the specific variable
        printf("%s\n", node_to_string(derivative)); // Print the derivative
        free_node(derivative); // Free the derivative node
        free_node(node); // Free the original AST node
    }
}