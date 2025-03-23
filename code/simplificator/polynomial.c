#include "./expressionobj.h"
#include "../ast.h"
#include "./monomial.h"
#include "./polynomial.h"
#include "../utils.h"

#include <stdlib.h>
#include <string.h>

Polynomial* create_polynomial(unsigned long long size) {
    Polynomial* table = malloc(sizeof(Polynomial));
    table->buckets = malloc(sizeof(PolynomialEntry*) * size);
    for (unsigned long long i = 0; i < size; i++) {
        table->buckets[i] = NULL;
    }
    table->size = size;
    table->count = 0;
    return table;
}

void free_polynomial(Polynomial* table) {
    if (table == NULL) return;
    for (unsigned long long i = 0; i < table->size; i++) {
        PolynomialEntry* entry = table->buckets[i];
        while (entry != NULL) {
            PolynomialEntry* temp = entry;
            entry = entry->next;
            free(temp->obj);
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

PolynomialNode* find_polynomial(Polynomial* table, ExpressionObj* key) {
    unsigned long long hash = hash_expression_obj(key);
    unsigned long long index = hash % table->size;
    PolynomialEntry* entry = table->buckets[index];
    while (entry != NULL) {
        if (entry->hash == hash && is_equal_expression_obj(entry->obj->key, key)) {
            return entry->obj;
        }
        entry = entry->next;
    }
    return NULL;
}

void sort_value_polynomialnode(PolynomialNode* node) {
    int negative = 0;
    if (node == NULL) return;
    if (node->value_denominator < 0) {
        node->value = -node->value;
        node->value_denominator = -node->value_denominator;
    }
    if (node->value == 0) {
        node->value_denominator = 1;
    } else {
        if (node->value < 0) {
            negative = 1;
            node->value = -node->value;
        }
        long long value_gcd = gcd(node->value, node->value_denominator);
        node->value /= value_gcd;
        node->value_denominator /= value_gcd;
        if (negative) {
            node->value = -node->value;
        }
    }
}

void remove_polynomial(Polynomial* table, ExpressionObj* key) {
    unsigned long long hash = hash_expression_obj(key);
    unsigned long long index = hash % table->size;
    PolynomialEntry* entry = table->buckets[index];
    PolynomialEntry* prev = NULL;
    while (entry != NULL) {
        if (entry->hash == hash && is_equal_expression_obj(entry->obj->key, key)) {
            if (prev == NULL) {
                table->buckets[index] = entry->next;
            } else {
                prev->next = entry->next;
            }
            free(entry->obj);
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

PolynomialNode* insert_polynomial(Polynomial* table, ExpressionObj* key, long long value, long long value_denominator) {
    if (is_zero_expression_obj(key)) {
        return NULL;
    }
    if (key->type == EXPR_OBJ_TYPE_Monomial) {
        value *= key->monomial->coeffecient;
        value_denominator *= key->monomial->coeffecient_denominator;
        key->monomial->coeffecient = 1;
        key->monomial->coeffecient_denominator = 1;
    }
    unsigned long long hash = hash_expression_obj(key);
    unsigned long long index = hash % table->size;
    PolynomialEntry* entry = table->buckets[index];
    while (entry != NULL) {
        if (entry->hash == hash && is_equal_expression_obj(entry->obj->key, key)) {
            long long new_value_denominator = entry->obj->value_denominator * value_denominator;
            long long new_value = entry->obj->value * value_denominator + value * entry->obj->value_denominator;
            entry->obj->value = new_value;
            entry->obj->value_denominator = new_value_denominator;
            sort_value_polynomialnode(entry->obj);
            if (entry->obj->value == 0) {
                remove_polynomial(table, key);
                table->count--;
                return NULL;
            }
            return entry->obj;
        }
        entry = entry->next;
    }
    PolynomialNode* new_node = malloc(sizeof(PolynomialNode));
    new_node->key = deep_copy_expression_obj(key);
    new_node->value = value;
    PolynomialEntry* new_entry = malloc(sizeof(PolynomialEntry));
    new_entry->hash = hash;
    new_entry->obj = new_node;
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    table->count++;
    return new_node;
}

unsigned long long count_polynomial(Polynomial* table) {
    return table->count;
}

unsigned long long hash_polynomial(Polynomial* table) {
    unsigned long long hash = 0;
    for (unsigned long long i = 0; i < table->size; i++) {
        PolynomialEntry* entry = table->buckets[i];
        while (entry != NULL) {
            hash += entry->hash;
            entry = entry->next;
        }
    }
    return hash;
}

int is_equal_polynomial(Polynomial* obj1, Polynomial* obj2) {
    if (obj1 == NULL || obj2 == NULL) {
        return 0;
    }
    if (obj1->count != obj2->count) {
        return 0;
    }
    for (unsigned long long i = 0; i < obj1->size; i++) {
        PolynomialEntry* entry1 = obj1->buckets[i];
        while (entry1 != NULL) {
            PolynomialNode* node2 = find_polynomial(obj2, entry1->obj->key);
            if (node2 == NULL || node2->value != entry1->obj->value) {
                return 0;
            }
            entry1 = entry1->next;
        }
    }
    return 1;
}

Polynomial* deep_copy_polynomial(Polynomial* table) {
    if (table == NULL) {
        return NULL;
    }
    Polynomial* new_table = create_polynomial(table->size);
    for (unsigned long long i = 0; i < table->size; i++) {
        PolynomialEntry* entry = table->buckets[i];
        while (entry != NULL) {
            insert_polynomial(new_table, deep_copy_expression_obj(entry->obj->key), entry->obj->value, entry->obj->value_denominator);
            entry = entry->next;
        }
    }
    return new_table;
}

Polynomial* create_polynomial_from_ast(Node* node) {
    if (node == NULL) {
        return NULL;
    }
    Polynomial* polynomial = create_polynomial(100);
    Monomial* current_monomial = create_monomial(100);
    create_monomial_from_ast(node, polynomial, current_monomial);
    return polynomial;
}

Node* polynomialnode_to_ast(PolynomialNode* node) {
    if (node == NULL) {
        return NULL;
    }
    Node* ast = create_node(FUNCTION_MULTIPLY, NULL, 0, 0);
    ast->data.function.right = expression_obj_to_ast(node->key);
    Node* ast_left = create_node_from_constant(node->value);
    if (node->value_denominator != 1) {
        Node* ast_left_temp = create_node(FUNCTION_DIVIDE, NULL, 0, 0);
        ast_left_temp->data.function.left = ast_left;
        ast_left_temp->data.function.right = create_node_from_constant(node->value_denominator);
        ast_left = ast_left_temp;
    }
    ast->data.function.left = ast_left;
    return ast;
}

Node* polynomial_to_ast(Polynomial* table) {
    if (is_zero_polynomial(table)) {
        return create_node_from_constant(0);
    }
    if (table == NULL) {
        return NULL;
    }
    Node* ast = create_node(FUNCTION_ADD, NULL, 0, 0);
    Node* current = ast;
    for (unsigned long long i = 0; i < table->size; i++) {
        PolynomialEntry* entry = table->buckets[i];
        while (entry != NULL) {
            Node* node = polynomialnode_to_ast(entry->obj);
            current->data.function.left = node;
            current->data.function.right = create_node(FUNCTION_ADD, NULL, 0, 0);
            current = current->data.function.right;
            entry = entry->next;
        }
    }
    current->data.function.right = create_node_from_constant(0);
    return ast;
}

int is_zero_polynomial(Polynomial* table) {
    if (table == NULL) {
        return 1;
    }
    if (table->count == 0) {
        return 1;
    }
    return 0;
}