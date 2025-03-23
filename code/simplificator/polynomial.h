#pragma once

#include "./expressionobj.h"
#include "../ast.h"

struct Monomial;

typedef struct PolynomialNode {
    ExpressionObj* key;
    long long value;
    long long value_denominator;
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
} Polynomial;

Polynomial* create_polynomial(unsigned long long size);

void free_polynomial(Polynomial* table);

PolynomialNode* find_polynomial(Polynomial* table, ExpressionObj* key);

void sort_value_polynomialnode(PolynomialNode* node);

void remove_polynomial(Polynomial* table, ExpressionObj* key);

PolynomialNode* insert_polynomial(Polynomial* table, ExpressionObj* key, long long value, long long value_denominator);

unsigned long long count_polynomial(Polynomial* table);

unsigned long long hash_polynomial(Polynomial* table);

int is_equal_polynomial(Polynomial* obj1, Polynomial* obj2);

Polynomial* deep_copy_polynomial(Polynomial* table);

Polynomial* create_polynomial_from_ast(Node* node);

Node* polynomialnode_to_ast(PolynomialNode* node);

Node* polynomial_to_ast(Polynomial* table);

int is_zero_polynomial(Polynomial* table);