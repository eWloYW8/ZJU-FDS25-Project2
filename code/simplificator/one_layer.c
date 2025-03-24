#include "one_layer.h"
#include "monomial.h"

#include <stdlib.h>

// Simplify the given node
Node* simple_simplify(Node *node) {
    if (node == NULL) return NULL; // If node is NULL, return NULL
    if (node->type == NODE_CONSTANT || node->type == NODE_VARIABLE) {
        return node; // If node is a constant or variable, return it as is
    }
    else {
        // Recursively simplify left and right children
        node->data.function.left = simple_simplify(node->data.function.left);
        if (node->data.function.right != NULL) {
            node->data.function.right = simple_simplify(node->data.function.right);
        }
    }
    switch (node->type) {
        case FUNCTION_ADD:
            // Simplify addition
            if ((node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0) || node->data.function.left == NULL) {
                // If left child is 0 or NULL, return right child
                Node *temp = node->data.function.right;
                free(node->data.function.left);
                free(node);
                return temp;
            }
            else if (node->data.function.right == NULL || (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 0)) {
                // If right child is 0 or NULL, return left child
                Node *temp = node->data.function.left;
                free(node->data.function.right);
                free(node);
                return temp;
            }
            else {
                return node; // Return node if no simplification is possible
            }
            break;
        case FUNCTION_SUBTRACT:
            // Simplify subtraction
            if (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 0) {
                // If right child is 0, return left child
                Node *temp = node->data.function.left;
                if (temp == NULL) temp = create_node(NODE_CONSTANT, "0", 0, 1);
                free(node->data.function.right);
                free(node);
                return temp;
            }
            else if (node->data.function.left != NULL && node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0) {
                // If left child is 0, return node
                free(node->data.function.left);
                node->data.function.left = NULL;
                return node;
            }
            else {
                return node; // Return node if no simplification is possible
            }
            break;
        case FUNCTION_MULTIPLY:
            // Simplify multiplication
            if (node->data.function.left == NULL || node->data.function.right == NULL || (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0) || (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 0)) {
                // If any child is 0 or NULL, return 0
                free_node(node);
                return create_node(NODE_CONSTANT, "0", 0, 1);
            }
            else if (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 1) {
                // If left child is 1, return right child
                Node *temp = node->data.function.right;
                free(node->data.function.left);
                free(node);
                return temp;
            }
            else if (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 1) {
                // If right child is 1, return left child
                Node *temp = node->data.function.left;
                free(node->data.function.right);
                free(node);
                return temp;
            }
            else {
                return node; // Return node if no simplification is possible
            }
            break;
        case FUNCTION_DIVIDE:
            // Simplify division
            if (node->data.function.left == NULL || (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0)) {
                // If left child is 0 or NULL, return 0
                free_node(node);
                return create_node(NODE_CONSTANT, "0", 0, 1);
            }
            else if (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 1) {
                // If right child is 1, return left child
                Node *temp = node->data.function.left;
                free(node->data.function.right);
                free(node);
                return temp;
            }
            else {
                return node; // Return node if no simplification is possible
            }
            break;
        case FUNCTION_POWER:
            // Simplify power
            if (node->data.function.left == NULL || (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0)) {
                // If left child is 0 or NULL, return 0
                free_node(node);
                return create_node(NODE_CONSTANT, "0", 0, 1);
            }
            else if (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 1) {
                // If left child is 1, return 1
                free_node(node);
                return create_node(NODE_CONSTANT, "1", 0, 1);
            }
            else if (node->data.function.left == NULL || (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 0)) {
                // If right child is 0 or NULL, return 1
                free_node(node);
                return create_node(NODE_CONSTANT, "1", 0, 1);
            }
            else if (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 1) {
                // If right child is 1, return left child
                Node *temp = node->data.function.left;
                free(node->data.function.right);
                free(node);
                return temp;
            }
            else {
                return node; // Return node if no simplification is possible
            }
            break;
        case FUNCTION_LN:
            // Simplify natural logarithm
            if (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 1) {
                // If left child is 1, return 0
                free_node(node);
                return create_node(NODE_CONSTANT, "0", 0, 1);
            }
            else {
                return node; // Return node if no simplification is possible
            }
            break;
        case FUNCTION_SIN:
        case FUNCTION_COS:
        case FUNCTION_TAN:
        case FUNCTION_LOG:
        case FUNCTION_EXP:
        case NODE_VARIABLE:
        case NODE_CONSTANT:
            return node; // Return node for trigonometric, logarithmic, exponential functions, variables, and constants
            break;
    }
}