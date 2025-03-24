#include "ast.h"
#include "utils.h"
#include "astmath.h"
#include "one_layer.h"
#include "monomial.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // Check if the number of arguments is less than 2
    if (argc < 2) {
        // Print usage information
        printf("Usage: %s <expression> [<variable(default all)>]\n", argv[0]);
        return 1;
    }
    // If there are exactly 2 arguments
    else if (argc == 2) {   
        char *expression = argv[1];
        expression = expadd0(expression); // Modify the expression to handle negative numbers
        // Remove any blank spaces from the expression
        remove_blankspace(expression);
        int current_position = 0;
        // Build the abstract syntax tree (AST) from the expression
        Node *node = build_ast(expression, &current_position);
        
        // Find all variables in the expression
        char **variables = find_variables(node);
        for (int i = 0; variables[i] != NULL; i++) {
            printf("%s: ", variables[i]);
            // Differentiate the expression with respect to the current variable
            Node *derivative = differentiate(node, variables[i]);
            // Simplify the derivative
            derivative = simple_simplify(derivative);
            derivative = monomial_simplify_recursive(derivative);
            derivative = simple_simplify(derivative);
            // Print the simplified derivative
            printf("%s\n", node_to_string(derivative));
            // Free the memory allocated for the derivative and variable
            free_node(derivative);
            free(variables[i]); 
        }
        // Free the memory allocated for the AST
        free_node(node);
    }
    // If there are exactly 3 arguments
    else if (argc == 3) {
        char *expression = argv[1];
        expression = expadd0(expression); // Modify the expression to handle negative numbers
        char *variable = argv[2];
        // Remove any blank spaces from the expression
        remove_blankspace(expression);
        int current_position = 0;
        // Build the abstract syntax tree (AST) from the expression
        Node *node = build_ast(expression, &current_position);
        
        // Differentiate the expression with respect to the specified variable
        Node *derivative = differentiate(node, variable);
        // Simplify the derivative
        derivative = simple_simplify(derivative);
        derivative = monomial_simplify_recursive(derivative);
        derivative = simple_simplify(derivative);
        // Print the simplified derivative
        printf("%s\n", node_to_string(derivative));
        // Free the memory allocated for the derivative and AST
        free_node(derivative);
        free_node(node);
    }
}