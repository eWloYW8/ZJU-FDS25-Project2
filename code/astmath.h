// This file contains some mathematical operations that can be performed on the AST.
#pragma once

#include "ast.h"

// differentiate computes the derivative of a node with respect to a variable.
Node* differentiate(Node *node, const char *variable);

// find_variables finds all variables in a node.
// It returns an array of strings.
// The last element of the array is NULL.
char** find_variables(Node *node);

// ast_is_equal checks if two nodes are structurally equal.
// It returns 1 if they are equal, 0 otherwise.
int ast_is_equal(Node *node1, Node *node2);

// monomial_simplify simplifies a monomial node.
// A monomial node is a node that is a product of constants and variables.
// It will record all nodes in the monomial node and the exponents of the equal node.
// It will return a new monomial node that is simplified.
// For example, if the input node is 2*x^2*3/x^3, the output node will be 6/x; if the input node is 2*x^2*3*x^3, the output node will be 6*x^5.
// And it will extract pow(node,[constant]), for example, pow(sin(x),2) will be extracted as sin(x)^2.
Node* monomial_simplify(Node *node);