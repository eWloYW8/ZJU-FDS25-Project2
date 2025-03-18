#include "./monomial.h"
#include "../ast.h"
#include "../ads/hashtable.h"
#include "./ExpressionObj.h"
#include "../utils.h"

#include <stdlib.h>

Monomial* create_monomial() {
    Monomial *monomial = malloc(sizeof(Monomial));
    monomial->coefficient = 1;
    monomial->coefficient_denominator = 1;
    monomial->expressionobjects = create_hash_table(MONOMIAL_SIZE);
    return monomial;
}

void free_monomial(Monomial *monomial) {
    if (monomial == NULL) {
        return;
    }
    free_hash_table(monomial->expressionobjects);
    free(monomial);
}

Monomial* deep_copy_monomial(Monomial *monomial) {
    if (monomial == NULL) {
        return NULL;
    }
    Monomial *new_monomial = create_monomial();
    new_monomial->coefficient = monomial->coefficient;
    new_monomial->coefficient_denominator = monomial->coefficient_denominator;
    for (unsigned long long i = 0; i < monomial->expressionobjects->size; i++) {
        HashTableEntry* entry = monomial->expressionobjects->buckets[i];
        while (entry != NULL) {
            struct ExpressionObj* obj = deep_copy_expression_obj(entry->obj->key);
            
            entry = entry->next;
        }
    }
    return new_monomial;
}

int monomial_is_equal(Monomial *monomial1, Monomial *monomial2) {
    if (monomial1 == NULL || monomial2 == NULL) {
        return 0;
    }
    if (monomial1->coefficient != monomial2->coefficient) {
        return 0;
    }
    if (monomial1->coefficient_denominator != monomial2->coefficient_denominator) {
        return 0;
    }
    if (count_hash_table(monomial1->expressionobjects) != count_hash_table(monomial2->expressionobjects)) {
        return 0;
    }
    for (unsigned long long i = 0; i < monomial1->expressionobjects->size; i++) {
        HashTableEntry* entry = monomial1->expressionobjects->buckets[i];
        while (entry != NULL) {
            struct ExpressionObj* obj = entry->obj->key;
            HashNode* node = find_hash_table(monomial2->expressionobjects, obj);
            if (node == NULL) {
                return 0;
            }
            entry = entry->next;
        }
    }
    return 1;
}

void monomial_sortexponent(Monomial *monomial) {
    int negative = 0;
    if (monomial->coefficient_denominator < 0) {
        monomial->coefficient = -monomial->coefficient;
        monomial->coefficient_denominator = -monomial->coefficient_denominator;
    }
    if (monomial->coefficient < 0) {
        negative = 1;
        monomial->coefficient = -monomial->coefficient;
    }
    long long effecient_gcd = gcd(monomial->coefficient, monomial->coefficient_denominator);
    monomial->coefficient /= effecient_gcd;
    monomial->coefficient_denominator /= effecient_gcd;
    if (monomial->coefficient_denominator < 0) {
        monomial->coefficient = -monomial->coefficient;
    }
}

void monomial_addobject(Monomial *monomial, struct ExpressionObj *object, long long current_exponent_magnification) {
    if (object->type == EXPRESSION_OBJ_AST) {
        if (object->data.ast->type == NODE_CONSTANT) {
            if (current_exponent_magnification > 0) {
                monomial->coefficient *= longlong_pow(object->data.ast->data.constant, current_exponent_magnification);
            }
            else if (current_exponent_magnification < 0) {
                monomial->coefficient_denominator *= longlong_pow(object->data.ast->data.constant, -current_exponent_magnification);
            }
            monomial_sortexponent(monomial);
        }
        else if (object->data.ast->type == NODE_VARIABLE) {
            HashNode* node = find_hash_table(monomial->expressionobjects, object);
            if (node == NULL) {
                HashNode* new_node = insert_hash_table(monomial->expressionobjects, object);
                new_node->value = current_exponent_magnification;
            }
            else {
                node->value += current_exponent_magnification;
                if (node->value == 0) {
                    remove_hash_table(monomial->expressionobjects, object);
                }
            }
        }
    }
    else if (object->type == EXPRESSION_OBJ_MONOMIAL) {
        monomial->coefficient *= object->data.monomial->coefficient;
        monomial->coefficient_denominator *= object->data.monomial->coefficient_denominator;
        for (unsigned long long i = 0; i < object->data.monomial->expressionobjects->size; i++) {
            HashTableEntry* entry = object->data.monomial->expressionobjects->buckets[i];
            while (entry != NULL) {
                struct ExpressionObj* obj = entry->obj->key;
                HashNode* node = find_hash_table(monomial->expressionobjects, obj);
                if (node == NULL) {
                    HashNode* new_node = insert_hash_table(monomial->expressionobjects, obj);
                    new_node->value = entry->obj->value * current_exponent_magnification;
                }
                else {
                    node->value += entry->obj->value * current_exponent_magnification;
                    if (node->value == 0) {
                        remove_hash_table(monomial->expressionobjects, obj);
                    }
                }
                entry = entry->next;
            }
        }
    }
    else if (object->type == EXPRESSION_OBJ_POLYNOMIAL) {
        // count the gcd of coefficient
        long long polymomial_coefficient_gcd = 0;
        int negative = 0;
        for (unsigned long long i = 0; i < object->data.polynomial->expressionobjects->size; i++) {
            HashTableEntry* entry = object->data.polynomial->expressionobjects->buckets[i];
            while (entry != NULL) {
                if (polymomial_coefficient_gcd == 0) {
                    polymomial_coefficient_gcd = entry->obj->value;
                    if (polymomial_coefficient_gcd < 0) {
                        negative = 1;
                    }
                }
                else {
                    polymomial_coefficient_gcd = gcd(polymomial_coefficient_gcd, entry->obj->value);
                }
                entry = entry->next;
            }
        }
        if (negative) {
            polymomial_coefficient_gcd = -polymomial_coefficient_gcd;
        }
        for (unsigned long long i = 0; i < object->data.polynomial->expressionobjects->size; i++) {
            HashTableEntry* entry = object->data.polynomial->expressionobjects->buckets[i];
            while (entry != NULL) {
                entry->obj->value /= polymomial_coefficient_gcd;
                entry = entry->next;
            }
        }
        if (current_exponent_magnification > 0) {
            monomial->coefficient *= longlong_pow(polymomial_coefficient_gcd, current_exponent_magnification);
        }
        else if (current_exponent_magnification < 0) {
            monomial->coefficient_denominator *= longlong_pow(polymomial_coefficient_gcd, -current_exponent_magnification);
        }
        monomial_sortexponent(monomial);

        HashNode* hashnode = find_hash_table(monomial->expressionobjects, object);
        if (hashnode == NULL) {
            HashNode* new_node = insert_hash_table(monomial->expressionobjects, object);
            new_node->value = 1;
        }
        else {
            hashnode->value += 1;
            if (hashnode->value == 0) {
                remove_hash_table(monomial->expressionobjects, object);
            }
        }
    }
    else {
        HashNode* hashnode = find_hash_table(monomial->expressionobjects, object);
        if (hashnode == NULL) {
            HashNode* new_node = insert_hash_table(monomial->expressionobjects, object);
            new_node->value = current_exponent_magnification;
        }
        else {
            hashnode->value += current_exponent_magnification;
            if (hashnode->value == 0) {
                remove_hash_table(monomial->expressionobjects, object);
            }
        }
    }
}

Node* monomial_to_ast(Monomial *monomial) {
    Node *ast = create_node_from_constant(monomial->coefficient);
    if (monomial->coefficient_denominator != 1) {
        Node *denominator = create_node_from_constant(monomial->coefficient_denominator);
        Node *temp = create_node(FUNCTION_DIVIDE, NULL, 0, 0);
        temp->data.function.left = ast;
        temp->data.function.right = denominator;
        ast = temp;
    }
    for (unsigned long long i = 0; i < monomial->expressionobjects->size; i++) {
        HashTableEntry* entry = monomial->expressionobjects->buckets[i];
        while (entry != NULL) {
            struct ExpressionObj* obj = entry->obj->key;
            if (obj->type == EXPRESSION_OBJ_AST) {
                Node *temp = create_node(FUNCTION_MULTIPLY, NULL, 0, 0);
                temp->data.function.left = ast;
                temp->data.function.right = obj->data.ast;
                ast = temp;
            }
            entry = entry->next;
        }
    }
    return ast;
}

unsigned long long monomial_get_hash(Monomial *monomial) {
    unsigned long long hash = 0;
    hash += monomial->coefficient;
    hash += monomial->coefficient_denominator;
    for (unsigned long long i = 0; i < monomial->expressionobjects->size; i++) {
        HashTableEntry* entry = monomial->expressionobjects->buckets[i];
        while (entry != NULL) {
            hash += expression_obj_get_hash(entry->obj->key);
            entry = entry->next;
        }
    }
    return hash;
}

void monomial_append_from_ast(Monomial *monomial, Node *ast, long long current_exponent_magnification) {
    if (ast->type == NODE_CONSTANT) {
        if (current_exponent_magnification > 0) {
            monomial->coefficient *= longlong_pow(ast->data.constant, current_exponent_magnification);
        }
        else if (current_exponent_magnification < 0) {
            monomial->coefficient_denominator *= longlong_pow(ast->data.constant, -current_exponent_magnification);
        }
        monomial_sortexponent(monomial);
    }
    else if (ast->type == NODE_VARIABLE) {
        ExpressionObj *obj = create_expression_obj(EXPRESSION_OBJ_AST);
        obj->data.ast = deep_copy_node(ast);
        monomial_addobject(monomial, obj, current_exponent_magnification);
        free_expression_obj(obj);
    }
    else if (ast->type == FUNCTION_MULTIPLY) {
        monomial_append_from_ast(monomial, ast->data.function.left, current_exponent_magnification);
        monomial_append_from_ast(monomial, ast->data.function.right, current_exponent_magnification);
    }
    else if (ast->type == FUNCTION_DIVIDE) {
        monomial_append_from_ast(monomial, ast->data.function.left, current_exponent_magnification);
        monomial_append_from_ast(monomial, ast->data.function.right, -current_exponent_magnification);
    }
    else if (ast->type == FUNCTION_POWER && ast->data.function.right->type == NODE_CONSTANT) {
        long long exponent = ast->data.function.right->data.constant;
        if (exponent > 0) {
            monomial_append_from_ast(monomial, ast->data.function.left, exponent * current_exponent_magnification);
        }
        else if (exponent < 0) {
            monomial_append_from_ast(monomial, ast->data.function.left, -exponent * current_exponent_magnification);
        }
    }
    else {
        ExpressionObj *obj = expression_obj_from_ast(ast);
        monomial_addobject(monomial, obj, current_exponent_magnification);
        free_expression_obj(obj);
    }
}

int monomial_get_length(Monomial *monomial) {
    return monomial->expressionobjects->count;
}