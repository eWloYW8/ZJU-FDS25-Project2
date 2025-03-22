#pragma once

#include "./expressionobj.h"
#include "../ast.h"

struct Monomial;

typedef struct PolynomialNode {
    ExpressionObj* key;
    ExpressionObj* value;
} PolynomialNode;

typedef struct PolynomialEntry {
    unsigned long long hash;
    PolynomialNode* obj;
    struct PolynomialEntry* next;
} PolynomialEntry;

typedef struct Polynomial {
    PolynomialEntry** buckets;
    unsigned long long size;
    unsigned long long count;
    long long constant;
    long long constant_denominator;
} Polynomial;

Polynomial* create_polynomial(unsigned long long size);

void free_polynomial(Polynomial* table);

PolynomialNode* insert_polynomial(Polynomial* table, ExpressionObj* key, ExpressionObj* value);

PolynomialNode* find_polynomial(Polynomial* table, ExpressionObj* key);

void remove_polynomial(Polynomial* table, ExpressionObj* key);

unsigned long long count_polynomial(Polynomial* table);

unsigned long long hash_polynomial(Polynomial* table);

int is_equal_polynomial(Polynomial* obj1, Polynomial* obj2);

Polynomial* deep_copy_polynomial(Polynomial* table);

Polynomial* create_polynomial_from_ast(Node* node);

void sort_coefficient_polynomial(Polynomial* table);

Node* polynomial_to_ast(Polynomial* table);