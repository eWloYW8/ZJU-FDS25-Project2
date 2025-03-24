#include "one_layer.h"
#include "monomial.h"

#include <stdlib.h>

Node* simple_simplify(Node *node) {
    if (node == NULL) return NULL;
    if (node->type == NODE_CONSTANT || node->type == NODE_VARIABLE) {
        return node;
    }
    else {
        node->data.function.left = simple_simplify(node->data.function.left);
        if (node->data.function.right != NULL) {
            node->data.function.right = simple_simplify(node->data.function.right);
        }
    }
    switch (node->type) {
        case FUNCTION_ADD:
            if ((node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0) || node->data.function.left == NULL) {
                Node *temp = node->data.function.right;
                free(node->data.function.left);
                free(node);
                return temp;
            }
            else if (node->data.function.right == NULL || (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 0)) {
                Node *temp = node->data.function.left;
                free(node->data.function.right);
                free(node);
                return temp;
            }
            else {
                return node;
            }
            break;
        case FUNCTION_SUBTRACT:
            if (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 0) {
                Node *temp = node->data.function.left;
                if (temp == NULL) temp = create_node(NODE_CONSTANT, "0", 0, 1);
                free(node->data.function.right);
                free(node);
                return temp;
            }
            else if (node->data.function.left != NULL && node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0) {
                free(node->data.function.left);
                node->data.function.left = NULL;
                return node;
            }
            else {
                return node;
            }
            break;
        case FUNCTION_MULTIPLY:
            if (node->data.function.left == NULL || node->data.function.right == NULL || (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0) || (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 0)) {
                free_node(node);
                return create_node(NODE_CONSTANT, "0", 0, 1);
            }
            else if (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 1) {
                Node *temp = node->data.function.right;
                free(node->data.function.left);
                free(node);
                return temp;
            }
            else if (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 1) {
                Node *temp = node->data.function.left;
                free(node->data.function.right);
                free(node);
                return temp;
            }
            else {
                return node;
            }
            break;
        case FUNCTION_DIVIDE:
            if (node->data.function.left == NULL || (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0)) {
                free_node(node);
                return create_node(NODE_CONSTANT, "0", 0, 1);
            }
            else if (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 1) {
                Node *temp = node->data.function.left;
                free(node->data.function.right);
                free(node);
                return temp;
            }
            else {
                return node;
            }
            break;
        case FUNCTION_POWER:
            if (node->data.function.left == NULL || (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0)) {
                free_node(node);
                return create_node(NODE_CONSTANT, "0", 0, 1);
            }
            else if (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 1) {
                free_node(node);
                return create_node(NODE_CONSTANT, "1", 0, 1);
            }
            else if (node->data.function.left == NULL || (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 0)) {
                free_node(node);
                return create_node(NODE_CONSTANT, "1", 0, 1);
            }
            else if (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 1) {
                Node *temp = node->data.function.left;
                free(node->data.function.right);
                free(node);
                return temp;
            }
            else {
                return node;
            }
            break;
        case FUNCTION_LN:
            if (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 1) {
                free_node(node);
                return create_node(NODE_CONSTANT, "0", 0, 1);
            }
            else {
                return node;
            }
            break;
        case FUNCTION_SIN:
        case FUNCTION_COS:
        case FUNCTION_TAN:
        case FUNCTION_LOG:
        case FUNCTION_EXP:
        case NODE_VARIABLE:
        case NODE_CONSTANT:
            return node;
            break;
    }
}