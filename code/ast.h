// AST (Abstract Syntax Tree) is a tree representation of a mathematical expression.
#pragma once

// NodeType is the type of a node in the AST.
enum NodeType {
    NODE_CONSTANT, // 1, 2, 3, 4, 5, 6, 7, 8, 9, 0
    NODE_VARIABLE, // any variable such as x, y, z, pos, etc.
    FUNCTION_ADD, // + (binary)
    FUNCTION_SUBTRACT, // - (binary)
    FUNCTION_MULTIPLY, // * (binary)
    FUNCTION_DIVIDE, // / (binary)
    FUNCTION_POWER, // ^ (binary)
    FUNCTION_SIN, // sin (unary)
    FUNCTION_COS, // cos (unary)
    FUNCTION_TAN, // tan (unary)
    FUNCTION_LN, // ln (unary)
    FUNCTION_LOG, // log (binary)
    FUNCTION_EXP, // exp (unary)
};

#define VARIABLE_NAME_SIZE 10

extern int PRIORITY[];

// Node is a single node in the AST.
// It can be a constant, a variable, or a function.
typedef struct Node {
    enum NodeType type;
    union {
        long long constant;
        char variable[VARIABLE_NAME_SIZE];
        struct {
            struct Node *left;
            struct Node *right;
        } function;
    } data;
} Node;

// create_node creates a new node with the given data.
Node* create_node(enum NodeType type, const char data[], int start, int end);

Node* create_node_from_constant(long long constant);

// free_node frees the memory of a node and its children.
void free_node(Node *node);

// deep_copy_node creates a deep copy of a node.
Node* deep_copy_node(Node *node);

// build_ast builds an AST from an expression.
Node* build_ast(const char *expression, int *current_position);


char* _node_to_string(Node *node, int parent_priority);

// node_to_string converts a node to a string.
char* node_to_string(Node *node);

// rebuild_ast rebuilds an AST from a existing AST (through string).
Node* rebuild_ast(Node *node);