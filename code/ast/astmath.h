// This file contains some mathematical operations that can be performed on the AST.
#pragma once

#include "ast.h"

// differentiate computes the derivative of a node with respect to a variable.
Node* differentiate(Node *node, const char *variable);

// find_variables finds all variables in a node.
// It returns an array of strings.
// The last element of the array is NULL.
char** find_variables(Node *node);

// is_equal checks if two nodes are structurally equal.
// It returns 1 if they are equal, 0 otherwise.
int is_equal(Node *node1, Node *node2);

