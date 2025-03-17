// This file contains some mathematical operations that can be performed on the AST.
#pragma once

#include "ast.h"

// differentiate computes the derivative of a node with respect to a variable.
Node* differentiate(Node *node, const char *variable);

// simplify simplifies a node.
// This function only performs one-layer simplification.
// It will return the input node instead of creating a new node, but you shouldn't use the input node anymore beacaue it may be freed.
// It simplifies the node in the following rule:
// 1. 1*node = node*1 = node
// 2. 0*node = node*0 = 0
// 3. 0+node = node+0 = node
// 4. node-0 = node
// 5. ln(1) = 0
// 6. pow(1, node) = 1
// 7. pow(node, 0) = 1
// 8. pow(node, 1) = node
// 9. 0/node = 0
// 10. node/1 = node
// 11. 0-node = -node
Node* simple_simplify(Node *node);

// find_variables finds all variables in a node.
// It returns an array of strings.
// The last element of the array is NULL.
char** find_variables(Node *node);

// is_equal checks if two nodes are structurally equal.
// It returns 1 if they are equal, 0 otherwise.
int is_equal(Node *node1, Node *node2);