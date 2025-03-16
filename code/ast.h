// AST (Abstract Syntax Tree) is a tree representation of a mathematical expression.
#pragma once

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// NodeType is the type of a node in the AST.
enum NodeType {
    NODE_CONSTANT, // 1, 2, 3, 4, 5, 6, 7, 8, 9, 0
    NODE_VARIABLE, // any variable such as x, y, z, pos, etc.
    FUNCTION_ADD, // + (binary)
    FUNCTION_SUBSTRACT, // - (binary)
    FUNCTION_MULTIPLY, // * (binary)
    FUNCTION_DIVIDE, // / (binary)
    FUNCTION_POWER, // ^ (binary)
    FUNCTION_NEGATIVE, // - (unary)
    FUNCTION_SIN, // sin (unary)
    FUNCTION_COS, // cos (unary)
    FUNCTION_TAN, // tan (unary)
    FUNCTION_LN, // ln (unary)
    FUNCTION_LOG, // log (binary)
    FUNCTION_EXP, // exp (unary)
};


// Node is a single node in the AST.
// It can be a constant, a variable, or a function.
typedef struct Node {
    enum NodeType type;
    union {
        double constant;
        char variable[10];
        struct {
            struct Node *left;
            struct Node *right;
        } function;
    } data;
} Node;
