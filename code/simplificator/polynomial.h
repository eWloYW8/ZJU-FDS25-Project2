#pragma once

#include "../ast.h"
#include "../ads/hashtable.h"

struct ExpressionObj;

typedef struct PolynomialNode {
    long long coefficient;
    struct ExpressionObj *data;
} PolynomialNode;

typedef struct Polynomial {
    long long constant;
    HashTable *expressionobjects;
} Polynomial;

// create_polynomial creates a new polynomial.
Polynomial* create_polynomial();

// free_polynomial frees the memory of a polynomial.
void free_polynomial(Polynomial *polynomial);

// deep_copy_polynomial creates a deep copy of a polynomial.
Polynomial* deep_copy_polynomial(Polynomial *polynomial);

// polynomial_is_equal checks if two polynomials are equal.
int polynomial_is_equal(Polynomial *polynomial1, Polynomial *polynomial2);

// polynomial_addnode adds a node to a polynomial.
void polynomial_addnode(Polynomial *polynomial, long long coefficient, struct ExpressionObj *data);

// polynomial_to_ast converts a polynomial to an AST node.
Node* polynomial_to_ast(Polynomial *polynomial);

// polynomial_get_hash returns the hash of a polynomial.
unsigned long long polynomial_get_hash(Polynomial *polynomial);

// polynomial_from_ast creates a polynomial from an AST node.
Polynomial* polynomial_from_ast(Node *ast);

// polynomial_get_length returns the length of a polynomial.
int polynomial_get_length(Polynomial *polynomial);