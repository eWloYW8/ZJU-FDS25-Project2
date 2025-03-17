#include "astmath.h"

#include <stdlib.h>
#include <string.h>

Node* differentiate(Node *node, const char *variable) {
    Node *temp;
    if (node == NULL) return NULL;
    Node *result = (Node*)malloc(sizeof(Node));
    if (node->type == NODE_CONSTANT) {
        result->type = NODE_CONSTANT;
        result->data.constant = 0;
    } else if (node->type == NODE_VARIABLE) {
        result->type = NODE_CONSTANT;
        if (strcmp(node->data.variable, variable) == 0) {
            result->data.constant = 1;
        } else {
            result->data.constant = 0;
        }
    } else {
        result->type = node->type;
        switch (node->type) {
            case FUNCTION_ADD:
                result->data.function.left = differentiate(node->data.function.left, variable);
                result->data.function.right = differentiate(node->data.function.right, variable);
                break;
            case FUNCTION_SUBTRACT:
                if (node->data.function.left == NULL) {
                    result->type = FUNCTION_SUBTRACT;
                    result->data.function.left = NULL;
                    result->data.function.right = differentiate(node->data.function.right, variable);
                } else {
                    result->data.function.left = differentiate(node->data.function.left, variable);
                    result->data.function.right = differentiate(node->data.function.right, variable);
                }
                break;
            case FUNCTION_MULTIPLY:
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
            case FUNCTION_DIVIDE:
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
            case FUNCTION_POWER:
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
            case FUNCTION_SIN:
                result->type = FUNCTION_MULTIPLY;
                result->data.function.left = create_node(FUNCTION_COS, NULL, 0, 0);
                result->data.function.left->data.function.left = deep_copy_node(node->data.function.left);
                result->data.function.right = differentiate(node->data.function.left, variable);
                break;
            case FUNCTION_COS:
                result->type = FUNCTION_MULTIPLY;
                result->data.function.left = create_node(FUNCTION_SUBTRACT, NULL, 0, 0);
                result->data.function.left->data.function.left = NULL;
                result->data.function.left->data.function.right = differentiate(node->data.function.left, variable);
                result->data.function.right = create_node(FUNCTION_SIN, NULL, 0, 0);
                result->data.function.right->data.function.left = deep_copy_node(node->data.function.left);
                break;
            case FUNCTION_TAN:
                result->type = FUNCTION_DIVIDE;
                result->data.function.left = differentiate(node->data.function.left, variable);
                result->data.function.right = create_node(FUNCTION_POWER, NULL, 0, 0);
                result->data.function.right->data.function.left = create_node(FUNCTION_COS, NULL, 0, 0);
                result->data.function.right->data.function.left->data.function.left = deep_copy_node(node->data.function.left);
                result->data.function.right->data.function.right = create_node(NODE_CONSTANT, "2", 0, 1);
                break;
            case FUNCTION_LN:
                result->type = FUNCTION_DIVIDE;
                result->data.function.left = differentiate(node->data.function.left, variable);
                result->data.function.right = deep_copy_node(node->data.function.left);
                break;
            case FUNCTION_LOG:
                temp = create_node(FUNCTION_DIVIDE, NULL, 0, 0);
                temp->data.function.left = create_node(FUNCTION_LN, NULL, 0, 0);
                temp->data.function.right = create_node(FUNCTION_LN, NULL, 0, 0);
                temp->data.function.left->data.function.left = deep_copy_node(node->data.function.right);
                temp->data.function.right->data.function.left = deep_copy_node(node->data.function.left);
                result = differentiate(temp, variable);
                free_node(temp);
                break;
            case FUNCTION_EXP:
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
            if (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0) {
                Node *temp = node->data.function.right;
                free(node->data.function.left);
                free(node);
                return temp;
            }
            else if (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 0) {
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
                free(node->data.function.right);
                free(node);
                return temp;
            }
            else if (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0) {
                free(node->data.function.left);
                node->data.function.left = NULL;
                return node;
            }
            else {
                return node;
            }
            break;
        case FUNCTION_MULTIPLY:
            if ((node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0) || (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 0)) {
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
            if (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 0) {
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
            if (node->data.function.left->type == NODE_CONSTANT && node->data.function.left->data.constant == 1) {
                free_node(node);
                return create_node(NODE_CONSTANT, "1", 0, 1);
            }
            else if (node->data.function.right->type == NODE_CONSTANT && node->data.function.right->data.constant == 0) {
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

// find_variables finds all variables in a node.
// It returns an array of strings.
// The last element of the array is NULL.
char** find_variables(Node *node) {
    if (node == NULL) return NULL;
    char **variables = (char**)malloc(sizeof(char*));
    int count = 0;
    if (node->type == NODE_VARIABLE) {
        variables[count] = (char*)malloc(VARIABLE_NAME_SIZE);
        strcpy(variables[count], node->data.variable);
        count++;
    }
    else if (node->type >= FUNCTION_ADD && node->type <= FUNCTION_EXP) {
        char **left_variables = find_variables(node->data.function.left);
        char **right_variables = find_variables(node->data.function.right);
        for (int i = 0; left_variables[i] != NULL; i++) {
            int duplicate = 0;
            for (int j = 0; j < count; j++) {
                if (strcmp(variables[j], left_variables[i]) == 0) {
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate) {
                variables = (char**)realloc(variables, (count + 1) * sizeof(char*));
                variables[count] = (char*)malloc(VARIABLE_NAME_SIZE);
                strcpy(variables[count], left_variables[i]);
                count++;
            }
        }
        for (int i = 0; right_variables[i] != NULL; i++) {
            int duplicate = 0;
            for (int j = 0; j < count; j++) {
                if (strcmp(variables[j], right_variables[i]) == 0) {
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate) {
                variables = (char**)realloc(variables, (count + 1) * sizeof(char*));
                variables[count] = (char*)malloc(VARIABLE_NAME_SIZE);
                strcpy(variables[count], right_variables[i]);
                count++;
            }
        }
        free(left_variables);
        free(right_variables);
    }
    variables = (char**)realloc(variables, (count + 1) * sizeof(char*));
    variables[count] = NULL;
    return variables;
}

int is_equal(Node *node1, Node *node2) {
    if (node1 == NULL && node2 == NULL) return 1;
    if (node1 == NULL || node2 == NULL) return 0;
    if (node1->type != node2->type) return 0;
    if (node1->type == NODE_CONSTANT) {
        return node1->data.constant == node2->data.constant;
    }
    else if (node1->type == NODE_VARIABLE) {
        return strcmp(node1->data.variable, node2->data.variable) == 0;
    }
    else {
        if (!is_equal(node1->data.function.left, node2->data.function.left)) return 0;
        if (!is_equal(node1->data.function.right, node2->data.function.right)) return 0;
        return 1;
    }
}