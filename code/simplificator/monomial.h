#pragma once

#include "../ast.h"
#include "../ads/hashtable.h"

struct ExpressionObj;

typedef struct MonomialNode {
    struct ExpressionObj *coefficient;
    long long exponent;
} MonomialNode;

typedef struct Monomial {
    long long coefficient;
    long long coefficient_denominator;
    HashTable *expressionobjects;
} Monomial;

// create_monomial creates a new monomial.
Monomial* create_monomial();

// free_monomial frees the memory of a monomial.
void free_monomial(Monomial *monomial);

// deep_copy_monomial creates a deep copy of a monomial.
Monomial* deep_copy_monomial(Monomial *monomial);

// monomial_is_equal checks if two monomials are equal.
int monomial_is_equal(Monomial *monomial1, Monomial *monomial2);

// monomial_addnode adds a node to a monomial.
void monomial_addnode(Monomial *monomial, struct ExpressionObj *coefficient, long long exponent);

// monomial_to_ast converts a monomial to an AST node.
Node* monomial_to_ast(Monomial *monomial);

// monomial_get_hash returns the hash of a monomial.
unsigned long long monomial_get_hash(Monomial *monomial);

// monomial_from_ast creates a monomial from an AST node.
Monomial* monomial_from_ast(Node *ast);

// monomial_append_from_ast appends a monomial from an AST node.
void monomial_append_from_ast(Monomial *monomial, Node *ast, long long current_exponent_magnification);

// monomial_get_length returns the length of a monomial.
int monomial_get_length(Monomial *monomial);