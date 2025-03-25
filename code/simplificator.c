#include "ast.h" // Abstract Syntax Tree functions
#include "utils.h" // Utility functions
#include "one_layer.h" // One layer simplification functions
#include "monomial.h" // Monomial simplification functions

#include <stdio.h> // Standard I/O functions

int main(int argc, char *argv[]) {
    if (argc < 2) {
        // Check if expression is provided as argument
        printf("Usage: %s <expression>\n", argv[0]);
        return 1;
    }
    char *expression = argv[1];
    expression = expadd0(expression); // Modify the expression to handle negative numbers
    expression = expadd_multiply(expression); // Modify the expression to handle implicit multiplication
    remove_blankspace(expression); // Remove any blank spaces from the expression
    int current_position = 0;
    Node *node = build_ast(expression, &current_position); // Build AST from expression
    
    node = simple_simplify(node); // Perform simple simplification
    node = monomial_simplify_recursive(node); // Perform monomial simplification
    node = simple_simplify(node); // Perform simple simplification again
    printf("%s\n", node_to_string(node)); // Print the simplified expression

    free_node(node); // Free the allocated memory for the AST
}