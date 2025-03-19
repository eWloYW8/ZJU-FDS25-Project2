#pragma once

// Monomial is a hash table, where the keys are polynomials (base) and the values are polynomials (exponent).

#include "./expressionobj.h"
#include "../ast.h"
#include "astmath.h"

typedef struct Polynomial Polynomial;

typedef struct MonimialNode {
    ExpressionObj* key;
    ExpressionObj* value;
} MonimialNode;

typedef struct MonimialEntry {
    unsigned long long hash;
    MonimialNode* obj;
    struct MonimialEntry* next;
} MonimialEntry;

typedef struct {
    MonimialEntry** buckets;
    unsigned long long size;
    unsigned long long count;
    long long coeffecient;
    long long coeffecient_
} Monimial;

Monimial* create_monomial(unsigned long long size);

void free_monomial(Monimial* table);

MonimialNode* insert_monomial(Monimial* table, ExpressionObj* key, ExpressionObj* value);

MonimialNode* find_monomial(Monimial* table, ExpressionObj* key);

void remove_monomial(Monimial* table, ExpressionObj* key);

unsigned long long count_monomial(Monimial* table);

MonimialNode* getone_in_monomial(Monimial* table);

unsigned long long hash_monomial(ExpressionObj* key);

int is_equal_monomial(ExpressionObj* obj1, ExpressionObj* obj2);

Monimial* deep_copy_monomial(Monimial* table);

Monimial* create_monomial_from_ast(Node* node, Polynomial* parent);