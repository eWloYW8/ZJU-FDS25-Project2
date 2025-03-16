#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "./ast.h"
#include "./stack.h"


int precedence[] = {
    [FUNCTION_NEGATIVE] = 6,
    [FUNCTION_POWER] = 5,
    [FUNCTION_MULTIPLY] = 4,
    [FUNCTION_DIVIDE] = 4,
    [FUNCTION_ADD] = 3,
    [FUNCTION_SUBSTRACT] = 3,
    [FUNCTION_SIN] = 7,
    [FUNCTION_COS] = 7,
    [FUNCTION_TAN] = 7,
    [FUNCTION_LN] = 7,
    [FUNCTION_EXP] = 7,
    [FUNCTION_LOG] = 7
};

int get_precedence(enum NodeType type) {
    return precedence[type];
}

int is_left_associative(enum NodeType type) {
    switch (type) {
        case FUNCTION_ADD:
        case FUNCTION_SUBSTRACT:
        case FUNCTION_MULTIPLY:
        case FUNCTION_DIVIDE:
        case FUNCTION_LOG: return 1;
        case FUNCTION_POWER:
        case FUNCTION_NEGATIVE: return 0;
        default: return 1;
    }
}

// 创建新节点
Node* create_node(enum NodeType type) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->type = type;
    return node;
}

// 转换函数
Node* parse_expression(const char *expr) {
    Stack *op_stack = create_stack(100);
    Stack *value_stack = create_stack(100);
    const char *p = expr;
    while (*p) {
        if (isspace(*p)) { p++; continue; }
        Node *a = create_node(NODE_VARIABLE);
        strcpy(a->data.variable, "a");
        push(value_stack, a);
        p++;
        Node *op = create_node(FUNCTION_ADD);
    }

    while (!is_empty(op_stack)) {
        Node *op = pop(op_stack);
    }

    Node *result = pop(value_stack);
    free(op_stack->data);
    free(op_stack);
    free(value_stack->data);
    free(value_stack);
    return result;
}

int main() {
    const char *expr = "a+b^c*d";
    Node *ast = parse_expression(expr);
    return 0;
}