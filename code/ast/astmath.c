#include "astmath.h"

#include <stdlib.h>
#include <string.h>

// Function to differentiate a given node with respect to a variable
Node* differentiate(Node *node, const char *variable) {
    Node *temp;
    if (node == NULL) return NULL; // If the node is NULL, return NULL
    Node *result = (Node*)malloc(sizeof(Node)); // Allocate memory for the result node
    if (node->type == NODE_CONSTANT) { // If the node is a constant
        result->type = NODE_CONSTANT; // The derivative of a constant is 0
        result->data.constant = 0;
    } else if (node->type == NODE_VARIABLE) { // If the node is a variable
        result->type = NODE_CONSTANT; // The derivative of a variable is 1 if it matches the differentiation variable, otherwise 0
        if (strcmp(node->data.variable, variable) == 0) {
            result->data.constant = 1;
        } else {
            result->data.constant = 0;
        }
    } else { // If the node is a function
        result->type = node->type; // Set the result type to the same as the node type
        switch (node->type) {
            case FUNCTION_ADD: // Derivative of addition
                result->data.function.left = differentiate(node->data.function.left, variable);
                result->data.function.right = differentiate(node->data.function.right, variable);
                break;
            case FUNCTION_SUBTRACT: // Derivative of subtraction
                if (node->data.function.left == NULL) { // If left node is NULL, handle unary minus
                    result->type = FUNCTION_SUBTRACT;
                    result->data.function.left = NULL;
                    result->data.function.right = differentiate(node->data.function.right, variable);
                } else {
                    result->data.function.left = differentiate(node->data.function.left, variable);
                    result->data.function.right = differentiate(node->data.function.right, variable);
                }
                break;
            case FUNCTION_MULTIPLY: // Derivative of multiplication (product rule)
                result->type = FUNCTION_ADD;
                result->data.function.left = (Node*)malloc(sizeof(Node));
                result->data.function.left->type = FUNCTION_MULTIPLY;
                result->data.function.left->data.function.left = differentiate(node->data.function.left, variable);
                result->data.function.left->data.function.right = deep_copy_node(node->data.function.right);
                result->data.function.right = (Node*)malloc(sizeof(Node));
                result->data.function.right->type = FUNCTION_MULTIPLY;
                result->data.function.right->data.function.left = deep_copy_node(node->data.function.left);
                result->data.function.right->data.function.right = differentiate(node->data.function.right, variable);
                break;
            case FUNCTION_DIVIDE: // Derivative of division (quotient rule)
                result->type = FUNCTION_DIVIDE;
                result->data.function.left = (Node*)malloc(sizeof(Node));
                result->data.function.left->type = FUNCTION_SUBTRACT;
                result->data.function.left->data.function.left = (Node*)malloc(sizeof(Node));
                result->data.function.left->data.function.left->type = FUNCTION_MULTIPLY;
                result->data.function.left->data.function.left->data.function.left = differentiate(node->data.function.left, variable);
                result->data.function.left->data.function.left->data.function.right = deep_copy_node(node->data.function.right);
                result->data.function.left->data.function.right = (Node*)malloc(sizeof(Node));
                result->data.function.left->data.function.right->type = FUNCTION_MULTIPLY;
                result->data.function.left->data.function.right->data.function.left = deep_copy_node(node->data.function.left);
                result->data.function.left->data.function.right->data.function.right = differentiate(node->data.function.right, variable);
                result->data.function.right = (Node*)malloc(sizeof(Node));
                result->data.function.right->type = FUNCTION_POWER;
                result->data.function.right->data.function.left = deep_copy_node(node->data.function.right);
                result->data.function.right->data.function.right = create_node(NODE_CONSTANT, "2", 0, 1);
                break;
            case FUNCTION_POWER: // Derivative of power (chain rule)
                result->type = FUNCTION_MULTIPLY;
                result->data.function.left = deep_copy_node(node);
                result->data.function.right = (Node*)malloc(sizeof(Node));
                result->data.function.right->type = FUNCTION_ADD;
                result->data.function.right->data.function.left = (Node*)malloc(sizeof(Node));
                result->data.function.right->data.function.left->type = FUNCTION_MULTIPLY;
                result->data.function.right->data.function.left->data.function.left = differentiate(node->data.function.right, variable);
                result->data.function.right->data.function.left->data.function.right = (Node*)malloc(sizeof(Node));
                result->data.function.right->data.function.left->data.function.right->type = FUNCTION_LN;
                result->data.function.right->data.function.left->data.function.right->data.function.left = deep_copy_node(node->data.function.left);
                result->data.function.right->data.function.left->data.function.right->data.function.right = NULL;
                result->data.function.right->data.function.right = (Node*)malloc(sizeof(Node));
                result->data.function.right->data.function.right->type = FUNCTION_DIVIDE;
                result->data.function.right->data.function.right->data.function.left = (Node*)malloc(sizeof(Node));
                result->data.function.right->data.function.right->data.function.left->type = FUNCTION_MULTIPLY;
                result->data.function.right->data.function.right->data.function.left->data.function.left = deep_copy_node(node->data.function.right);
                result->data.function.right->data.function.right->data.function.left->data.function.right = differentiate(node->data.function.left, variable);
                result->data.function.right->data.function.right->data.function.right = deep_copy_node(node->data.function.left);
                break;
            case FUNCTION_SIN: // Derivative of sine
                result->type = FUNCTION_MULTIPLY;
                result->data.function.left = create_node(FUNCTION_COS, NULL, 0, 0);
                result->data.function.left->data.function.left = deep_copy_node(node->data.function.left);
                result->data.function.right = differentiate(node->data.function.left, variable);
                break;
            case FUNCTION_COS: // Derivative of cosine
                result->type = FUNCTION_MULTIPLY;
                result->data.function.left = create_node(FUNCTION_SUBTRACT, NULL, 0, 0);
                result->data.function.left->data.function.left = NULL;
                result->data.function.left->data.function.right = differentiate(node->data.function.left, variable);
                result->data.function.right = create_node(FUNCTION_SIN, NULL, 0, 0);
                result->data.function.right->data.function.left = deep_copy_node(node->data.function.left);
                break;
            case FUNCTION_TAN: // Derivative of tangent
                result->type = FUNCTION_DIVIDE;
                result->data.function.left = differentiate(node->data.function.left, variable);
                result->data.function.right = create_node(FUNCTION_POWER, NULL, 0, 0);
                result->data.function.right->data.function.left = create_node(FUNCTION_COS, NULL, 0, 0);
                result->data.function.right->data.function.left->data.function.left = deep_copy_node(node->data.function.left);
                result->data.function.right->data.function.right = create_node(NODE_CONSTANT, "2", 0, 1);
                break;
            case FUNCTION_LN: // Derivative of natural logarithm
                result->type = FUNCTION_DIVIDE;
                result->data.function.left = differentiate(node->data.function.left, variable);
                result->data.function.right = deep_copy_node(node->data.function.left);
                break;
            case FUNCTION_LOG: // Derivative of logarithm
                temp = create_node(FUNCTION_DIVIDE, NULL, 0, 0);
                temp->data.function.left = create_node(FUNCTION_LN, NULL, 0, 0);
                temp->data.function.right = create_node(FUNCTION_LN, NULL, 0, 0);
                temp->data.function.left->data.function.left = deep_copy_node(node->data.function.right);
                temp->data.function.right->data.function.left = deep_copy_node(node->data.function.left);
                result = differentiate(temp, variable);
                free_node(temp);
                break;
            case FUNCTION_EXP: // Derivative of exponential
                result->type = FUNCTION_MULTIPLY;
                result->data.function.left = differentiate(node->data.function.left, variable);
                result->data.function.right = deep_copy_node(node);
                break;
            default:
                break;
        }
    }
    return result;
}

// Function to find all variables in a node
// It returns an array of strings, with the last element being NULL
char** find_variables(Node *node) {
    if (node == NULL) return NULL; // If the node is NULL, return NULL
    char **variables = (char**)malloc(sizeof(char*)); // Allocate memory for the variables array
    int count = 0; // Initialize the count of variables
    if (node->type == NODE_VARIABLE) { // If the node is a variable
        variables[count] = (char*)malloc(VARIABLE_NAME_SIZE); // Allocate memory for the variable name
        strcpy(variables[count], node->data.variable); // Copy the variable name
        count++;
    }
    else if (node->type >= FUNCTION_ADD && node->type <= FUNCTION_EXP) { // If the node is a function
        char **left_variables = find_variables(node->data.function.left); // Find variables in the left subtree
        for (int i = 0; left_variables[i] != NULL; i++) { // Iterate through the left variables
            int duplicate = 0;
            for (int j = 0; j < count; j++) { // Check for duplicates
                if (strcmp(variables[j], left_variables[i]) == 0) {
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate) { // If not a duplicate, add to the variables array
                variables = (char**)realloc(variables, (count + 1) * sizeof(char*));
                variables[count] = (char*)malloc(VARIABLE_NAME_SIZE);
                strcpy(variables[count], left_variables[i]);
                count++;
            }
        }
        free(left_variables); // Free the left variables array
        if(node->data.function.right != NULL) { // If the right subtree is not NULL
            char **right_variables = find_variables(node->data.function.right); // Find variables in the right subtree
            for (int i = 0; right_variables[i] != NULL; i++) { // Iterate through the right variables
                int duplicate = 0;
                for (int j = 0; j < count; j++) { // Check for duplicates
                    if (strcmp(variables[j], right_variables[i]) == 0) {
                        duplicate = 1;
                        break;
                    }
                }
                if (!duplicate) { // If not a duplicate, add to the variables array
                    variables = (char**)realloc(variables,  (count + 1) * sizeof(char*));
                    variables[count] = (char*)malloc(VARIABLE_NAME_SIZE);
                    strcpy(variables[count], right_variables[i]);
                    count++;
                }
            }
            free(right_variables); // Free the right variables array
        }
    }
    variables = (char**)realloc(variables, (count + 1) * sizeof(char*)); // Reallocate memory for the variables array
    variables[count] = NULL; // Set the last element to NULL
    return variables;
}

// Function to check if two nodes are equal
int is_equal(Node *node1, Node *node2) {
    if (node1 == NULL && node2 == NULL) return 1; // If both nodes are NULL, they are equal
    if (node1 == NULL || node2 == NULL) return 0; // If one of the nodes is NULL, they are not equal
    if (node1->type != node2->type) return 0; // If the types are different, they are not equal
    if (node1->type == NODE_CONSTANT) { // If the nodes are constants
        return node1->data.constant == node2->data.constant; // Compare the constant values
    }
    else if (node1->type == NODE_VARIABLE) { // If the nodes are variables
        return strcmp(node1->data.variable, node2->data.variable) == 0; // Compare the variable names
    }
    else { // If the nodes are functions
        if (!is_equal(node1->data.function.left, node2->data.function.left)) return 0; // Compare the left subtrees
        if (!is_equal(node1->data.function.right, node2->data.function.right)) return 0; // Compare the right subtrees
        return 1; // If both subtrees are equal, the nodes are equal
    }
}
