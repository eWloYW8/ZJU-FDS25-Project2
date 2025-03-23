#include "./expressionobj.h"
#include "../ast.h"
#include "./monomial.h"
#include "./polynomial.h"
#include "../utils.h"

#include <stdlib.h>
#include <string.h>

Monomial* create_monomial(unsigned long long size) {
    Monomial* table = malloc(sizeof(Monomial));
    table->buckets = malloc(sizeof(MonomialEntry*) * size);
    for (unsigned long long i = 0; i < size; i++) {
        table->buckets[i] = NULL;
    }
    table->size = size;
    table->count = 0;
    table->coeffecient = 1;
    table->coeffecient_denominator = 1;
    return table;
}

void free_monomial(Monomial* table) {
    if (table == NULL) return;
    for (unsigned long long i = 0; i < table->size; i++) {
        MonomialEntry* entry = table->buckets[i];
        while (entry != NULL) {
            MonomialEntry* temp = entry;
            entry = entry->next;
            free(temp->obj);
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

MonomialNode* find_monomial(Monomial* table, ExpressionObj* key) {
    unsigned long long hash = hash_expression_obj(key);
    unsigned long long index = hash % table->size;
    MonomialEntry* entry = table->buckets[index];
    while (entry != NULL) {
        if (entry->hash == hash && is_equal_expression_obj(entry->obj->key, key)) {
            return entry->obj;
        }
        entry = entry->next;
    }
    return NULL;
}

void remove_monomial(Monomial* table, ExpressionObj* key) {
    unsigned long long hash = hash_expression_obj(key);
    unsigned long long index = hash % table->size;
    MonomialEntry* entry = table->buckets[index];
    MonomialEntry* prev = NULL;
    while (entry != NULL) {
        if (entry->hash == hash && is_equal_expression_obj(entry->obj->key, key)) {
            if (prev == NULL) {
                table->buckets[index] = entry->next;
            } else {
                prev->next = entry->next;
            }
            table->count--;
            free(entry->obj);
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

MonomialNode* insert_monomial(Monomial* table, ExpressionObj* key, ExpressionObj* value) {
    if (is_zero_expression_obj(key)) {
        return NULL;
    }
    if (key->type == EXPR_OBJ_TYPE_Constant) {
        if (key->constant == 0) {
            table->coeffecient = 0;
            table->coeffecient_denominator = 1;
            return NULL;
        }
        if (value->type == EXPR_OBJ_TYPE_Constant) {
            if (value->constant < 0) {
                table->coeffecient_denominator *= fastpow(key->constant, -value->constant);
            }
            else if (value->constant > 0) {
                table->coeffecient *= fastpow(key->constant, value->constant);
            }
            return NULL;
        }
    }
    unsigned long long hash = hash_expression_obj(key);
    unsigned long long index = hash % table->size;
    MonomialEntry* entry = table->buckets[index];
    while (entry != NULL) {
        if (entry->hash == hash && is_equal_expression_obj(entry->obj->key, key)) {
            insert_polynomial(entry->obj->value->polynomial, key, 1, 1);
            if (is_zero_polynomial(entry->obj->value->polynomial)) {
                remove_monomial(table, key);
                return NULL;
            }
            return entry->obj;
        }
        entry = entry->next;
    }
    MonomialNode* new_node = malloc(sizeof(MonomialNode));
    new_node->key = deep_copy_expression_obj(key);
    new_node->value = create_expression_obj(EXPR_OBJ_TYPE_Polynomial);
    insert_polynomial(new_node->value->polynomial, value, 1, 1);
    MonomialEntry* new_entry = malloc(sizeof(MonomialEntry));
    new_entry->hash = hash;
    new_entry->obj = new_node;
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    table->count++;
    return new_node;
}

unsigned long long count_monomial(Monomial* table) {
    return table->count;
}

unsigned long long hash_monomial(Monomial* table) {
    unsigned long long hash = 0;
    for (unsigned long long i = 0; i < table->size; i++) {
        MonomialEntry* entry = table->buckets[i];
        while (entry != NULL) {
            hash += entry->hash;
            entry = entry->next;
        }
    }
    return hash;
}

int is_equal_monomial(Monomial* obj1, Monomial* obj2) {
    if (obj1 == NULL || obj2 == NULL) {
        return 0;
    }
    if (obj1->count != obj2->count) {
        return 0;
    }
    for (unsigned long long i = 0; i < obj1->size; i++) {
        MonomialEntry* entry1 = obj1->buckets[i];
        while (entry1 != NULL) {
            MonomialNode* node2 = find_monomial(obj2, entry1->obj->key);
            if (node2 == NULL || !is_equal_expression_obj(entry1->obj->value, node2->value)) {
                return 0;
            }
            entry1 = entry1->next;
        }
    }
    return 1;
}

Monomial* deep_copy_monomial(Monomial* table) {
    if (table == NULL) {
        return NULL;
    }
    Monomial* new_table = create_monomial(table->size);
    new_table->coeffecient = table->coeffecient;
    new_table->coeffecient_denominator = table->coeffecient_denominator;
    for (unsigned long long i = 0; i < table->size; i++) {
        MonomialEntry* entry = table->buckets[i];
        while (entry != NULL) {
            insert_monomial(new_table, deep_copy_expression_obj(entry->obj->key), deep_copy_expression_obj(entry->obj->value));
            entry = entry->next;
        }
    }
    return new_table;
}

void sort_coefficient_monomial(Monomial* table) {
    int negative = 0;
    if (table == NULL) return;
    if (table->coeffecient_denominator < 0) {
        table->coeffecient = -table->coeffecient;
        table->coeffecient_denominator = -table->coeffecient_denominator;
    }
    if (table->coeffecient == 0) {
        table->coeffecient_denominator = 1;
    } else {
        if (table->coeffecient < 0) {
            negative = 1;
            table->coeffecient = -table->coeffecient;
        }
        long long gcd_value = gcd(table->coeffecient, table->coeffecient_denominator);
        table->coeffecient /= gcd_value;
        table->coeffecient_denominator /= gcd_value;
    }
    if (negative) {
        table->coeffecient = -table->coeffecient;
    }
}

Node* monomialnode_to_ast(MonomialNode* node) {
    if (node == NULL) {
        return NULL;
    }
    Node* ast = create_node(FUNCTION_POWER, NULL, 0, 0);
    ast->data.function.left = expression_obj_to_ast(node->key);
    ast->data.function.right = expression_obj_to_ast(node->value);
    return ast;
}


Node* monomial_to_ast(Monomial* table) {
    if (table == NULL) {
        return NULL;
    }
    Node* ast = create_node(FUNCTION_MULTIPLY, NULL, 0, 0);
    ast->data.function.left = create_node_from_constant(table->coeffecient);
    Node* current = ast;
    for (unsigned long long i = 0; i < table->size; i++) {
        MonomialEntry* entry = table->buckets[i];
        while (entry != NULL) {
            Node* node = monomialnode_to_ast(entry->obj);
            current->data.function.right = node;
            current = node;
            entry = entry->next;
        }
    }
    if (table->coeffecient_denominator != 1) {
        Node* ast_temp = create_node(FUNCTION_DIVIDE, NULL, 0, 0);
        ast_temp->data.function.left = ast;
        ast_temp->data.function.right = create_node_from_constant(table->coeffecient_denominator);
        ast = ast_temp;
    }
    return ast;
}

int is_zero_monomial(Monomial* table) {
    if (table == NULL) {
        return 0;
    }
    return table->coeffecient == 0;
}

void multiply_monomial(Monomial *table, Monomial *monomial) {
    if (table == NULL || monomial == NULL) {
        return;
    }
    table->coeffecient *= monomial->coeffecient;
    table->coeffecient_denominator *= monomial->coeffecient_denominator;
    sort_coefficient_monomial(table);
    for (unsigned long long i = 0; i < monomial->size; i++) {
        MonomialEntry* entry = monomial->buckets[i];
        while (entry != NULL) {
            insert_monomial(table, entry->obj->key, entry->obj->value);
            entry = entry->next;
        }
    }
}