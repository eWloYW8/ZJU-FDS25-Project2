#include "ast.h"

#define MAX_VARIABLES 1000

typedef struct {
    long long coefficient;
    long long coefficient_denominator;
    Node* node[MAX_VARIABLES];
    long long exponent[MAX_VARIABLES];
    int count;
} Monomial;

Monomial *create_monomial();

void free_monomial(Monomial *monomial);

int find_monomial(Monomial *monomial, Node *node);

void _add_monomial(Monomial *monomial, Node *node, long long exponent_magnification);

void add_monomial(Monomial *monomial, Node *node);

Monomial *node_to_monomial(Node *node);

Node *monomial_to_node(Monomial *monomial);

void sort_coefficient_monomial(Monomial *monomial);

Node *monomial_simplify(Node *node);

Node *monomial_simplify_recursive(Node *node);