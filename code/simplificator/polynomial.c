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
            table->count--;
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
    if (node == NULL) return NULL;

    Polynomial* poly = create_polynomial(100); // 初始哈希表大小设为100

    switch (node->type) {
        // 处理叶子节点（常数/变量）
        case NODE_CONSTANT: {
            // 创建常数单项式
            ExpressionObj* const_obj = create_expression_obj(EXPR_OBJ_TYPE_Constant);
            const_obj->constant = node->data.constant;
            insert_polynomial(poly, const_obj, 1, 1);
            destroy_expression_obj(const_obj);
            break;
        }
        case NODE_VARIABLE: {
            // 创建变量单项式
            ExpressionObj* var_obj = create_expression_obj(EXPR_OBJ_TYPE_Variable);
            strncpy(var_obj->variable, node->data.variable, VARIABLE_NAME_SIZE);
            insert_polynomial(poly, var_obj, 1, 1);
            destroy_expression_obj(var_obj);
            break;
        }

        // 处理加法：合并左右子树多项式
        case FUNCTION_ADD: {
            Polynomial* left = create_polynomial_from_ast(node->data.function.left);
            Polynomial* right = create_polynomial_from_ast(node->data.function.right);
            
            // 合并左子树项
            for (unsigned i = 0; i < left->size; i++) {
                PolynomialEntry* e = left->buckets[i];
                while (e) {
                    insert_polynomial(poly, e->obj->key, e->obj->value, e->obj->value_denominator);
                    e = e->next;
                }
            }
            
            // 合并右子树项
            for (unsigned i = 0; i < right->size; i++) {
                PolynomialEntry* e = right->buckets[i];
                while (e) {
                    insert_polynomial(poly, e->obj->key, e->obj->value, e->obj->value_denominator);
                    e = e->next;
                }
            }
            
            free_polynomial(left);
            free_polynomial(right);
            break;
        }

        // 处理乘法：生成笛卡尔积组合
        case FUNCTION_MULTIPLY: {
            Polynomial* left = create_polynomial_from_ast(node->data.function.left);
            Polynomial* right = create_polynomial_from_ast(node->data.function.right);

            // 遍历左右所有项进行组合
            for (unsigned i = 0; i < left->size; i++) {
                PolynomialEntry* le = left->buckets[i];
                while (le) {
                    for (unsigned j = 0; j < right->size; j++) {
                        PolynomialEntry* re = right->buckets[j];
                        while (re) {
                            // 合并系数
                            long long new_val = le->obj->value * re->obj->value;
                            long long new_den = le->obj->value_denominator * re->obj->value_denominator;

                            // 合并键（作为Monomial的乘积）
                            ExpressionObj* merged_key = create_expression_obj(EXPR_OBJ_TYPE_Monomial);
                            merged_key->monomial = create_monomial(100);
                            
                            // 添加左项因子
                            if (le->obj->key->type == EXPR_OBJ_TYPE_Monomial) {
                                MonomialEntry** entries = le->obj->key->monomial->buckets;
                                for (unsigned k = 0; k < le->obj->key->monomial->size; k++) {
                                    MonomialEntry* me = entries[k];
                                    while (me) {
                                        insert_monomial(merged_key->monomial, me->obj->key, me->obj->value);
                                        me = me->next;
                                    }
                                }
                            } else {
                                ExpressionObj* constant_obj = create_expression_obj(EXPR_OBJ_TYPE_Constant);
                                constant_obj->constant = 1;
                                insert_monomial(merged_key->monomial, le->obj->key, constant_obj);
                            }

                            // 添加右项因子
                            if (re->obj->key->type == EXPR_OBJ_TYPE_Monomial) {
                                MonomialEntry** entries = re->obj->key->monomial->buckets;
                                for (unsigned k = 0; k < re->obj->key->monomial->size; k++) {
                                    MonomialEntry* me = entries[k];
                                    while (me) {
                                        insert_monomial(merged_key->monomial, me->obj->key, me->obj->value);
                                        me = me->next;
                                    }
                                }
                            } else {
                                ExpressionObj* constant_obj = create_expression_obj(EXPR_OBJ_TYPE_Constant);
                                constant_obj->constant = 1;
                                insert_monomial(merged_key->monomial, re->obj->key, constant_obj);
                            }

                            // 插入合并后的项
                            insert_polynomial(poly, merged_key, new_val, new_den);
                            destroy_expression_obj(merged_key);
                            
                            re = re->next;
                        }
                    }
                    le = le->next;
                }
            }
            free_polynomial(left);
            free_polynomial(right);
            break;
        }

        // 处理函数/复杂结构：保留结构并递归处理参数
        case FUNCTION_SIN:
        case FUNCTION_COS:
        case FUNCTION_TAN:
        case FUNCTION_LN:
        case FUNCTION_EXP:
        case FUNCTION_LOG:
        case FUNCTION_POWER: {
            // 创建函数表达式对象
            ExpressionObj* func_obj = create_expression_obj_from_ast(node);
            insert_polynomial(poly, func_obj, 1, 1);
            destroy_expression_obj(func_obj);
            break;
        }

        // 处理除法：分母作为系数处理
        case FUNCTION_DIVIDE: {
            Polynomial* num_poly = create_polynomial_from_ast(node->data.function.left);
            Node* den_node = node->data.function.right;
            
            // 将分母转换为表达式对象
            ExpressionObj* den_obj = create_expression_obj_from_ast(den_node);
            
            // 遍历分子项，分母作为系数分母
            for (unsigned i = 0; i < num_poly->size; i++) {
                PolynomialEntry* e = num_poly->buckets[i];
                while (e) {
                    insert_polynomial(poly, e->obj->key, e->obj->value, e->obj->value_denominator * hash_expression_obj(den_obj));
                    e = e->next;
                }
            }
            
            destroy_expression_obj(den_obj);
            free_polynomial(num_poly);
            break;
        }

        default:
            // 处理其他未实现的操作符
            break;
    }

    return poly;
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