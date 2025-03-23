#include "ast.h"
#include "stack.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// PRIORITY is the priority of each operator.
int PRIORITY[] = {
    0, // NODE_CONSTANT
    0, // NODE_VARIABLE
    1, // FUNCTION_ADD
    1, // FUNCTION_SUBSTRACT
    2, // FUNCTION_MULTIPLY
    2, // FUNCTION_DIVIDE
    3, // FUNCTION_POWER
    4, // FUNCTION_SIN
    4, // FUNCTION_COS
    4, // FUNCTION_TAN
    4, // FUNCTION_LN
    4, // FUNCTION_LOG
    4, // FUNCTION_EXP
};

// Associativity of each operator.
// 0: left-to-right
// 1: right-to-left
int ASSOCIATIVITY[] = {
    0, // NODE_CONSTANT
    0, // NODE_VARIABLE
    0, // FUNCTION_ADD
    0, // FUNCTION_SUBSTRACT
    0, // FUNCTION_MULTIPLY
    0, // FUNCTION_DIVIDE
    1, // FUNCTION_POWER
    0, // FUNCTION_SIN
    0, // FUNCTION_COS
    0, // FUNCTION_TAN
    0, // FUNCTION_LN
    0, // FUNCTION_LOG
    0, // FUNCTION_EXP
};

Node* create_node(enum NodeType type, const char data[], int start, int end) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->type = type;
    if (type == NODE_CONSTANT) {
        node->data.constant = atoi(data + start);
    } else if (type == NODE_VARIABLE) {
        strncpy(node->data.variable, data + start, end - start);
        node->data.variable[end - start] = '\0';
    } else {
        node->data.function.left = NULL;
        node->data.function.right = NULL;
    }
    return node;
}

void free_node(Node *node) {
    if (node == NULL) return;
    if (node->type >= FUNCTION_ADD && node->type <= FUNCTION_LOG) {
        free_node(node->data.function.left);
        free_node(node->data.function.right);
    }
    free(node);
}

Node* deep_copy_node(Node *node) {
    if (node == NULL) return NULL;
    Node *copy = (Node*)malloc(sizeof(Node));
    copy->type = node->type;
    if (node->type == NODE_CONSTANT) {
        copy->data.constant = node->data.constant;
    } else if (node->type == NODE_VARIABLE) {
        strcpy(copy->data.variable, node->data.variable);
    } else {
        copy->data.function.left = deep_copy_node(node->data.function.left);
        copy->data.function.right = deep_copy_node(node->data.function.right);
    }
    return copy;
}

Node* build_ast(const char *expression, int *current_position) {
    Stack *operators = create_stack(100);
    Stack *operands = create_stack(100);
    while (expression[*current_position] != '\0') {
        if (isdigit(expression[*current_position])) {
            int start = *current_position;
            while (isdigit(expression[*current_position])) {
                (*current_position)++;
            }
            Node *node = create_node(NODE_CONSTANT, expression, start, *current_position);
            push(operands, node);
        } else if (isalpha(expression[*current_position])) {
            int start = *current_position;
            while (isalpha(expression[*current_position])) {
                (*current_position)++;
            }
            char str[VARIABLE_NAME_SIZE];
            strncpy(str, expression + start, *current_position - start);
            str[*current_position - start] = '\0';
            if (strcmp(str, "sin") == 0) {
                Node* node = create_node(FUNCTION_SIN, NULL, 0, 0);
                (*current_position)++;
                node->data.function.left = build_ast(expression, current_position);
                push(operands, node);
            } else if (strcmp(str, "cos") == 0) {
                Node* node = create_node(FUNCTION_COS, NULL, 0, 0);
                (*current_position)++;
                node->data.function.left = build_ast(expression, current_position);
                push(operands, node);
            } else if (strcmp(str, "tan") == 0) {
                Node* node = create_node(FUNCTION_TAN, NULL, 0, 0);
                (*current_position)++;
                node->data.function.left = build_ast(expression, current_position);
                push(operands, node);
            } else if (strcmp(str, "ln") == 0) {
                Node* node = create_node(FUNCTION_LN, NULL, 0, 0);
                (*current_position)++;
                node->data.function.left = build_ast(expression, current_position);
                push(operands, node);
            } else if (strcmp(str, "log") == 0) {
                Node* node = create_node(FUNCTION_LOG, NULL, 0, 0);
                (*current_position)++;
                node->data.function.left = build_ast(expression, current_position);
                node->data.function.right = build_ast(expression, current_position);
                push(operands, node);
            } else if (strcmp(str, "exp") == 0) {
                Node* node = create_node(FUNCTION_EXP, NULL, 0, 0);
                (*current_position)++;
                node->data.function.left = build_ast(expression, current_position);
                push(operands, node);
            } else {
                Node *node = create_node(NODE_VARIABLE, expression, start, *current_position);
                push(operands, node);
            }
        } else if (expression[*current_position] == '(') {
            (*current_position)++;
            Node *node = build_ast(expression, current_position);
            push(operands, node);
        } else if (expression[*current_position] == ')' || expression[*current_position] == ',') {
            (*current_position)++;
            break;
        } else {
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
            while (!is_empty(operators) && PRIORITY[peek(operators)->type] >= PRIORITY[type] && ASSOCIATIVITY[type] == 0) {
                Node *op = pop(operators);
                Node *right = pop(operands);
                Node *left = pop(operands);
                op->data.function.left = left;
                op->data.function.right = right;
                push(operands, op);
            }
            Node *node = create_node(type, NULL, 0, 0);
            push(operators, node);
            (*current_position)++;
        }
    }
    while (!is_empty(operators)) {
        Node *op = pop(operators);
        Node *right = pop(operands);
        Node *left = pop(operands);
        op->data.function.left = left;
        op->data.function.right = right;
        push(operands, op);
    }
    Node *result = pop(operands);
    free_stack(operators);
    free_stack(operands);
    return result;
}

char* _node_to_string(Node *node, int parent_priority){
    if (node == NULL) return NULL;
    char *str = (char*)malloc(100);
    if (node->type == NODE_CONSTANT) {
        if (node->data.constant < 0 && parent_priority > 1) {
            sprintf(str, "(%lld)", node->data.constant);
        } else {
            sprintf(str, "%lld", node->data.constant);
        }
    } else if (node->type == NODE_VARIABLE) {
        sprintf(str, "%s", node->data.variable);
    } else {
        int current_priority = PRIORITY[node->type];
        char *left = NULL;
        char *right = NULL;
        if (current_priority >= parent_priority) {
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
                        sprintf(str, "-%s", right); // This happens because -(Node) is considered as NULL-(Node)
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
                    sprintf(str, "ERROR");
            }
        } else {
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
                        sprintf(str, "(-%s)", right); // This happens because -(Node) is considered as NULL-(Node)
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
                    sprintf(str, "ERROR");
            }
        }
        free(left);
        free(right);
    }
    return str;
}

char* node_to_string(Node *node) {
    return _node_to_string(node, -1);
}

Node* rebuild_ast(Node *node) {
    char *str = node_to_string(node);
    int current_position = 0;
    Node *new_node = build_ast(str, &current_position);
    free(str);
    return new_node;
}