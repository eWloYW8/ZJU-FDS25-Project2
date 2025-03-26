// This file provides the definition of monomial and functions to manipulate monomials, which are used in the simplification process of mathematical expressions.

#pragma once

#include "ast.h"

#define MAX_VARIABLES 10000

typedef struct {
    long long coefficient; // Coefficient of the monomial
    long long coefficient_denominator; // Denominator of the coefficient
    Node* node[MAX_VARIABLES]; // Array of pointers to nodes representing variables
    long long exponent[MAX_VARIABLES]; // Array of exponents corresponding to the variables
    int count; // Number of variables in the monomial
} Monomial;

// Function to create a new monomial
Monomial *create_monomial();

// Function to free the memory allocated for a monomial
void free_monomial(Monomial *monomial);

// Function to find a variable in the monomial
int find_monomial(Monomial *monomial, Node *node);

// Internal function to add a variable to the monomial with a given exponent magnification
void _add_monomial(Monomial *monomial, Node *node, long long exponent_magnification);

// Function to add a variable to the monomial
void add_monomial(Monomial *monomial, Node *node);

// Function to convert a node to a monomial
Monomial *node_to_monomial(Node *node);

// Function to convert a monomial to a node
Node *monomial_to_node(Monomial *monomial);

// Function to sort the monomial by its coefficient
void sort_coefficient_monomial(Monomial *monomial);

// Function to simplify a node using monomials
Node *monomial_simplify(Node *node);

// Recursive function to simplify a node using monomials
Node *monomial_simplify_recursive(Node *node);