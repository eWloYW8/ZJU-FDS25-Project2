#pragma once

// Monomial is a hash table, where the keys are ExpressionObj (excluding Monomial) (base) and the values are ExpressionObj (excluding Monomial) (exponent).
// Monomial must be a element of Polynomial.

#include "./expressionobj.h"
#include "../ast.h"

struct Polynomial;

typedef struct MonomialNode {
    ExpressionObj* key;
    ExpressionObj* value;
} MonomialNode;

typedef struct MonomialEntry {
    unsigned long long hash;
    MonomialNode* obj;
    struct MonomialEntry* next;
} MonomialEntry;

typedef struct Monomial {
    MonomialEntry** buckets;
    unsigned long long size;
    unsigned long long count;
    long long coeffecient;
    long long coeffecient_denominator;
} Monomial;

Monomial* create_monomial(unsigned long long size);

void free_monomial(Monomial* table);

MonomialNode* find_monomial(Monomial* table, ExpressionObj* key);

void remove_monomial(Monomial* table, ExpressionObj* key);

MonomialNode* insert_monomial(Monomial* table, ExpressionObj* key, ExpressionObj* value);

unsigned long long count_monomial(Monomial* table);

unsigned long long hash_monomial(Monomial* table);

int is_equal_monomial(Monomial* obj1, Monomial* obj2);

Monomial* deep_copy_monomial(Monomial* table);

void sort_coefficient_monomial(Monomial* table);

Node* monomialnode_to_ast(MonomialNode* node);

Node* monomial_to_ast(Monomial* table);

int is_zero_monomial(Monomial* table);

void multiply_monomial(Monomial* table, Monomial* monomial);