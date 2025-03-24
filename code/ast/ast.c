#include "ast.h"
#include "stack.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// PRIORITY is the priority of each operator.
// The higher the value, the higher the precedence.
// For example, multiplication has higher precedence than addition.
int PRIORITY[] = {
    0, // NODE_CONSTANT - Constants have the lowest priority as they are leaf nodes.
    0, // NODE_VARIABLE - Variables also have the lowest priority as they are leaf nodes.
    1, // FUNCTION_ADD - Addition has lower priority than multiplication.
    1, // FUNCTION_SUBSTRACT - Subtraction has the same priority as addition.
    2, // FUNCTION_MULTIPLY - Multiplication has higher priority than addition.
    2, // FUNCTION_DIVIDE - Division has the same priority as multiplication.
    3, // FUNCTION_POWER - Exponentiation has higher priority than multiplication.
    4, // FUNCTION_SIN - Trigonometric functions have the highest priority.
    4, // FUNCTION_COS
    4, // FUNCTION_TAN
    4, // FUNCTION_LN - Logarithmic functions have the highest priority.
    4, // FUNCTION_LOG
    4, // FUNCTION_EXP - Exponential functions have the highest priority.
};

// Associativity of each operator.
// 0: left-to-right (e.g., addition, subtraction, multiplication, division)
// 1: right-to-left (e.g., exponentiation)
int ASSOCIATIVITY[] = {
    0, // NODE_CONSTANT - Constants are leaf nodes, no associativity.
    0, // NODE_VARIABLE - Variables are leaf nodes, no associativity.
    0, // FUNCTION_ADD - Addition is left-associative.
    0, // FUNCTION_SUBSTRACT - Subtraction is left-associative.
    0, // FUNCTION_MULTIPLY - Multiplication is left-associative.
    0, // FUNCTION_DIVIDE - Division is left-associative.
    1, // FUNCTION_POWER - Exponentiation is right-associative.
    0, // FUNCTION_SIN - Trigonometric functions are not associative.
    0, // FUNCTION_COS
    0, // FUNCTION_TAN
    0, // FUNCTION_LN - Logarithmic functions are not associative.
    0, // FUNCTION_LOG
    0, // FUNCTION_EXP - Exponential functions are not associative.
};

// Function to create a new AST node.
// type: The type of the node (e.g., NODE_CONSTANT, NODE_VARIABLE, FUNCTION_ADD, etc.).
// data: The input string containing the data for the node.
// start: The starting index in the data string for the node's value.
// end: The ending index in the data string for the node's value.
Node* create_node(enum NodeType type, const char data[], int start, int end) {
    Node *node = (Node*)malloc(sizeof(Node)); // Allocate memory for the node.
    node->type = type; // Set the node type.
    if (type == NODE_CONSTANT) {
        // If the node is a constant, convert the substring to an integer.
        node->data.constant = atoi(data + start);
    } else if (type == NODE_VARIABLE) {
        // If the node is a variable, copy the variable name from the data string.
        strncpy(node->data.variable, data + start, end - start);
        node->data.variable[end - start] = '\0'; // Null-terminate the string.
    } else {
        // If the node is a function, initialize its left and right children to NULL.
        node->data.function.left = NULL;
        node->data.function.right = NULL;
    }
    return node; // Return the newly created node.
}

// Function to create a node with a constant value.
// constant: The constant value to be stored in the node.
Node* create_node_with_constant(long long constant) {
    Node *node = (Node*)malloc(sizeof(Node)); // Allocate memory for the node.
    node->type = NODE_CONSTANT; // Set the node type to NODE_CONSTANT.
    node->data.constant = constant; // Set the constant value.
    return node; // Return the newly created node.
}

// Function to free the memory allocated for a node and its children.
// node: The node to be freed.
void free_node(Node *node) {
    if (node == NULL) return; // If the node is NULL, return immediately.
    if (node->type >= FUNCTION_ADD && node->type <= FUNCTION_LOG) {
        // If the node is a function, recursively free its left and right children.
        free_node(node->data.function.left);
        free_node(node->data.function.right);
    }
    free(node); // Free the node itself.
}

// Function to create a deep copy of a node.
// node: The node to be copied.
Node* deep_copy_node(Node *node) {
    if (node == NULL) return NULL; // If the node is NULL, return NULL.
    Node *copy = (Node*)malloc(sizeof(Node)); // Allocate memory for the copy.
    copy->type = node->type; // Copy the node type.
    if (node->type == NODE_CONSTANT) {
        // If the node is a constant, copy the constant value.
        copy->data.constant = node->data.constant;
    } else if (node->type == NODE_VARIABLE) {
        // If the node is a variable, copy the variable name.
        strcpy(copy->data.variable, node->data.variable);
    } else {
        // If the node is a function, recursively copy its left and right children.
        copy->data.function.left = deep_copy_node(node->data.function.left);
        copy->data.function.right = deep_copy_node(node->data.function.right);
    }
    return copy; // Return the deep copy of the node.
}

// Function to build an Abstract Syntax Tree (AST) from an expression string.
// expression: The input expression string.
// current_position: A pointer to the current position in the expression string.
Node* build_ast(const char *expression, int *current_position) {
    Stack *operators = create_stack(100); // Stack to hold operators.
    Stack *operands = create_stack(100); // Stack to hold operands.
    while (expression[*current_position] != '\0') {
        if (isdigit(expression[*current_position])) {
            // If the current character is a digit, parse the constant.
            int start = *current_position;
            while (isdigit(expression[*current_position])) {
                (*current_position)++;
            }
            Node *node = create_node(NODE_CONSTANT, expression, start, *current_position);
            push(operands, node); // Push the constant node onto the operands stack.
        } else if (isalpha(expression[*current_position])) {
            // If the current character is a letter, parse the variable or function.
            int start = *current_position;
            while (isalpha(expression[*current_position])) {
                (*current_position)++;
            }
            char str[VARIABLE_NAME_SIZE];
            strncpy(str, expression + start, *current_position - start);
            str[*current_position - start] = '\0';
            if (strcmp(str, "sin") == 0) {
                // If the string is "sin", create a sine function node.
                Node* node = create_node(FUNCTION_SIN, NULL, 0, 0);
                (*current_position)++;
                node->data.function.left = build_ast(expression, current_position); // Parse the argument.
                push(operands, node); // Push the sine node onto the operands stack.
            } else if (strcmp(str, "cos") == 0) {
                // If the string is "cos", create a cosine function node.
                Node* node = create_node(FUNCTION_COS, NULL, 0, 0);
                (*current_position)++;
                node->data.function.left = build_ast(expression, current_position); // Parse the argument.
                push(operands, node); // Push the cosine node onto the operands stack.
            } else if (strcmp(str, "tan") == 0) {
                // If the string is "tan", create a tangent function node.
                Node* node = create_node(FUNCTION_TAN, NULL, 0, 0);
                (*current_position)++;
                node->data.function.left = build_ast(expression, current_position); // Parse the argument.
                push(operands, node); // Push the tangent node onto the operands stack.
            } else if (strcmp(str, "ln") == 0) {
                // If the string is "ln", create a natural logarithm function node.
                Node* node = create_node(FUNCTION_LN, NULL, 0, 0);
                (*current_position)++;
                node->data.function.left = build_ast(expression, current_position); // Parse the argument.
                push(operands, node); // Push the ln node onto the operands stack.
            } else if (strcmp(str, "log") == 0) {
                // If the string is "log", create a logarithm function node.
                Node* node = create_node(FUNCTION_LOG, NULL, 0, 0);
                (*current_position)++;
                node->data.function.left = build_ast(expression, current_position); // Parse the first argument.
                node->data.function.right = build_ast(expression, current_position); // Parse the second argument.
                push(operands, node); // Push the log node onto the operands stack.
            } else if (strcmp(str, "exp") == 0) {
                // If the string is "exp", create an exponential function node.
                Node* node = create_node(FUNCTION_EXP, NULL, 0, 0);
                (*current_position)++;
                node->data.function.left = build_ast(expression, current_position); // Parse the argument.
                push(operands, node); // Push the exp node onto the operands stack.
            } else {
                // If the string is a variable, create a variable node.
                Node *node = create_node(NODE_VARIABLE, expression, start, *current_position);
                push(operands, node); // Push the variable node onto the operands stack.
            }
        } else if (expression[*current_position] == '(') {
            // If the current character is '(', it indicates the start of a sub-expression.
            (*current_position)++;
            Node *node = build_ast(expression, current_position); // Recursively build the AST for the sub-expression.
            push(operands, node); // Push the sub-expression node onto the operands stack.
        } else if (expression[*current_position] == ')' || expression[*current_position] == ',') {
            // If the current character is ')' or ',', it indicates the end of a sub-expression or argument list.
            (*current_position)++;
            break; // Exit the loop to process the sub-expression.
        } else {
            // If the current character is an operator, determine its type.
            enum NodeType type;
            switch (expression[*current_position]) {
                case '+':
                    type = FUNCTION_ADD;
                    break;
                case '-':
                    type = FUNCTION_SUBTRACT;
                    break;
                case '*':
                    type = FUNCTION_MULTIPLY;
                    break;
                case '/':
                    type = FUNCTION_DIVIDE;
                    break;
                case '^':
                    type = FUNCTION_POWER;
                    break;
            };
            // Process operators based on their priority and associativity.
            while (!is_empty(operators) && PRIORITY[peek(operators)->type] >= PRIORITY[type] && ASSOCIATIVITY[type] == 0) {
                Node *op = pop(operators); // Pop the operator from the stack.
                Node *right = pop(operands); // Pop the right operand.
                Node *left = pop(operands); // Pop the left operand.
                op->data.function.left = left; // Set the left child of the operator.
                op->data.function.right = right; // Set the right child of the operator.
                push(operands, op); // Push the operator node back onto the operands stack.
            }
            Node *node = create_node(type, NULL, 0, 0); // Create a new operator node.
            push(operators, node); // Push the operator node onto the operators stack.
            (*current_position)++; // Move to the next character in the expression.
        }
    }
    // After processing the entire expression, pop any remaining operators and build the final AST.
    while (!is_empty(operators)) {
        Node *op = pop(operators); // Pop the operator from the stack.
        Node *right = pop(operands); // Pop the right operand.
        Node *left = pop(operands); // Pop the left operand.
        op->data.function.left = left; // Set the left child of the operator.
        op->data.function.right = right; // Set the right child of the operator.
        push(operands, op); // Push the operator node back onto the operands stack.
    }
    Node *result = pop(operands); // The final AST is the last node on the operands stack.
    free_stack(operators); // Free the operators stack.
    free_stack(operands); // Free the operands stack.
    return result; // Return the root of the AST.
}

// Helper function to convert a node to a string representation.
// node: The node to be converted.
// parent_priority: The priority of the parent node to determine if parentheses are needed.
char* _node_to_string(Node *node, int parent_priority){
    if (node == NULL) return NULL; // If the node is NULL, return NULL.
    char *str = (char*)malloc(100); // Allocate memory for the string.
    if (node->type == NODE_CONSTANT) {
        // If the node is a constant, convert it to a string.
        if (node->data.constant < 0 && parent_priority > 1) {
            // If the constant is negative and the parent has high priority, add parentheses.
            sprintf(str, "(%lld)", node->data.constant);
        } else {
            sprintf(str, "%lld", node->data.constant);
        }
    } else if (node->type == NODE_VARIABLE) {
        // If the node is a variable, copy the variable name to the string.
        sprintf(str, "%s", node->data.variable);
    } else {
        // If the node is a function, recursively convert its children to strings.
        int current_priority = PRIORITY[node->type];
        char *left = NULL;
        char *right = NULL;
        if (current_priority > parent_priority) {
            // If the current node has higher priority than the parent, no parentheses are needed.
            switch (node->type) {
                case FUNCTION_ADD:
                    left = _node_to_string(node->data.function.left, current_priority);
                    right = _node_to_string(node->data.function.right, current_priority);
                    sprintf(str, "%s+%s", left, right);
                    break;
                case FUNCTION_SUBTRACT:
                    left = _node_to_string(node->data.function.left, current_priority);
                    right = _node_to_string(node->data.function.right, current_priority);
                    if (left == NULL) {
                        sprintf(str, "-%s", right); // Handle unary minus.
                    } else {
                        sprintf(str, "%s-%s", left, right);
                    }
                    break;
                case FUNCTION_MULTIPLY:
                    left = _node_to_string(node->data.function.left, current_priority);
                    right = _node_to_string(node->data.function.right, current_priority);
                    sprintf(str, "%s*%s", left, right);
                    break;
                case FUNCTION_DIVIDE:
                    left = _node_to_string(node->data.function.left, current_priority);
                    right = _node_to_string(node->data.function.right, current_priority);
                    sprintf(str, "%s/%s", left, right);
                    break;
                case FUNCTION_POWER:
                    left = _node_to_string(node->data.function.left, current_priority);
                    right = _node_to_string(node->data.function.right, current_priority);
                    sprintf(str, "%s^%s", left, right);
                    break;
                case FUNCTION_SIN:
                    left = _node_to_string(node->data.function.left, -1);
                    sprintf(str, "sin(%s)", left);
                    break;
                case FUNCTION_COS:
                    left = _node_to_string(node->data.function.left, -1);
                    sprintf(str, "cos(%s)", left);
                    break;
                case FUNCTION_TAN:
                    left = _node_to_string(node->data.function.left, -1);
                    sprintf(str, "tan(%s)", left);
                    break;
                case FUNCTION_LN:
                    left = _node_to_string(node->data.function.left, -1);
                    sprintf(str, "ln(%s)", left);
                    break;
                case FUNCTION_LOG:
                    left = _node_to_string(node->data.function.left, -1);
                    right = _node_to_string(node->data.function.right, -1);
                    sprintf(str, "log(%s,%s)", left, right);
                    break;
                case FUNCTION_EXP:
                    left = _node_to_string(node->data.function.left, -1);
                    sprintf(str, "exp(%s)", left);
                    break;
                default:
                    sprintf(str, "ERROR"); // Handle unknown node types.
            }
        } else {
            // If the current node has lower or equal priority to the parent, add parentheses.
            switch (node->type) {
                case FUNCTION_ADD:
                    left = _node_to_string(node->data.function.left, current_priority);
                    right = _node_to_string(node->data.function.right, current_priority);
                    sprintf(str, "(%s+%s)", left, right);
                    break;
                case FUNCTION_SUBTRACT:
                    left = _node_to_string(node->data.function.left, current_priority);
                    right = _node_to_string(node->data.function.right, current_priority);
                    if (left == NULL) {
                        sprintf(str, "(-%s)", right); // Handle unary minus.
                    } else {
                        sprintf(str, "(%s-%s)", left, right);
                    }
                    break;
                case FUNCTION_MULTIPLY:
                    left = _node_to_string(node->data.function.left, current_priority);
                    right = _node_to_string(node->data.function.right, current_priority);
                    sprintf(str, "(%s*%s)", left, right);
                    break;
                case FUNCTION_DIVIDE:
                    left = _node_to_string(node->data.function.left, current_priority);
                    right = _node_to_string(node->data.function.right, current_priority);
                    sprintf(str, "(%s/%s)", left, right);
                    break;
                case FUNCTION_POWER:
                    left = _node_to_string(node->data.function.left, current_priority);
                    right = _node_to_string(node->data.function.right, current_priority);
                    sprintf(str, "(%s^%s)", left, right);
                    break;
                case FUNCTION_SIN:
                    left = _node_to_string(node->data.function.left, -1);
                    sprintf(str, "sin(%s)", left);
                    break;
                case FUNCTION_COS:
                    left = _node_to_string(node->data.function.left, -1);
                    sprintf(str, "cos(%s)", left);
                    break;
                case FUNCTION_TAN:
                    left = _node_to_string(node->data.function.left, -1);
                    sprintf(str, "tan(%s)", left);
                    break;
                case FUNCTION_LN:
                    left = _node_to_string(node->data.function.left, -1);
                    sprintf(str, "ln(%s)", left);
                    break;
                case FUNCTION_LOG:
                    left = _node_to_string(node->data.function.left, -1);
                    right = _node_to_string(node->data.function.right, -1);
                    sprintf(str, "log(%s,%s)", left, right);
                    break;
                case FUNCTION_EXP:
                    left = _node_to_string(node->data.function.left, -1);
                    sprintf(str, "exp(%s)", left);
                    break;
                default:
                    sprintf(str, "ERROR"); // Handle unknown node types.
            }
        }
        free(left); // Free the memory allocated for the left child string.
        free(right); // Free the memory allocated for the right child string.
    }
    return str; // Return the string representation of the node.
}

// Function to convert a node to a string representation.
// node: The node to be converted.
char* node_to_string(Node *node) {
    return _node_to_string(node, -1); // Call the helper function with no parent priority.
}

// Function to rebuild an AST from a node.
// node: The node to be rebuilt.
Node* rebuild_ast(Node *node) {
    char *str = node_to_string(node); // Convert the node to a string.
    int current_position = 0;
    Node *new_node = build_ast(str, &current_position); // Build a new AST from the string.
    free(str); // Free the memory allocated for the string.
    return new_node; // Return the new AST.
}