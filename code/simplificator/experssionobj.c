#include "./expressionobj.h"
#include "../ast.h"
#include "./monomial.h"
#include "./polynomial.h"

#include <stdlib.h>
#include <string.h>

ExpressionObj* create_expression_obj(enum ExpressionObjType type) {
    ExpressionObj* obj = malloc(sizeof(ExpressionObj));
    obj->type = type;
    switch (type) {
        case EXPR_OBJ_TYPE_Monomial:
            obj->monomial = create_monomial(100);
            break;
        case EXPR_OBJ_TYPE_Polynomial:
            obj->polynomial = create_polynomial(100);
            break;
        case EXPR_OBJ_TYPE_Sin:
            obj->sin.arg = NULL;
            break;
        case EXPR_OBJ_TYPE_Cos:
            obj->cos.arg = NULL;
            break;
        case EXPR_OBJ_TYPE_Tan:
            obj->tan.arg = NULL;
            break;
        case EXPR_OBJ_TYPE_Ln:
            obj->ln.arg = NULL;
            break;
        case EXPR_OBJ_TYPE_Log:
            obj->log.base = NULL;
            obj->log.arg = NULL;
            break;
        case EXPR_OBJ_TYPE_Exp:
            obj->exp.arg = NULL;
            break;
        case EXPR_OBJ_TYPE_Pow:
            obj->pow.base = NULL;
            obj->pow.exponent = NULL;
            break;
        case EXPR_OBJ_TYPE_Constant:
            obj->constant = 0;
            break;
        case EXPR_OBJ_TYPE_Variable:
            memset(obj->variable, 0, VARIABLE_NAME_SIZE);
            break;
    }
    return obj;
}

void destroy_expression_obj(ExpressionObj* obj) {
    if (obj == NULL) {
        return;
    }
    switch (obj->type) {
        case EXPR_OBJ_TYPE_Monomial:
            free_monomial(obj->monomial);
            break;
        case EXPR_OBJ_TYPE_Polynomial:
            free_polynomial(obj->polynomial);
            break;
        case EXPR_OBJ_TYPE_Sin:
            destroy_expression_obj(obj->sin.arg);
            break;
        case EXPR_OBJ_TYPE_Cos:
            destroy_expression_obj(obj->cos.arg);
            break;
        case EXPR_OBJ_TYPE_Tan:
            destroy_expression_obj(obj->tan.arg);
            break;
        case EXPR_OBJ_TYPE_Ln:
            destroy_expression_obj(obj->ln.arg);
            break;
        case EXPR_OBJ_TYPE_Log:
            destroy_expression_obj(obj->log.base);
            destroy_expression_obj(obj->log.arg);
            break;
        case EXPR_OBJ_TYPE_Exp:
            destroy_expression_obj(obj->exp.arg);
            break;
        case EXPR_OBJ_TYPE_Pow:
            destroy_expression_obj(obj->pow.base);
            destroy_expression_obj(obj->pow.exponent);
            break;
        case EXPR_OBJ_TYPE_Constant:
            break;
        case EXPR_OBJ_TYPE_Variable:
            break;
    }
    free(obj);
}

int is_equal_expression_obj(ExpressionObj *obj1, ExpressionObj *obj2) {
    if (obj1 == NULL || obj2 == NULL) {
        return 0;
    }
    if (obj1->type != obj2->type) {
        return 0;
    }
    switch (obj1->type) {
        case EXPR_OBJ_TYPE_Monomial:
            return is_equal_monomial(obj1->monomial, obj2->monomial);
        case EXPR_OBJ_TYPE_Polynomial:
            return is_equal_polynomial(obj1->polynomial, obj2->polynomial);
        case EXPR_OBJ_TYPE_Sin:
            return is_equal_expression_obj(obj1->sin.arg, obj2->sin.arg);
        case EXPR_OBJ_TYPE_Cos:
            return is_equal_expression_obj(obj1->cos.arg, obj2->cos.arg);
        case EXPR_OBJ_TYPE_Tan:
            return is_equal_expression_obj(obj1->tan.arg, obj2->tan.arg);
        case EXPR_OBJ_TYPE_Ln:
            return is_equal_expression_obj(obj1->ln.arg, obj2->ln.arg);
        case EXPR_OBJ_TYPE_Log:
            return is_equal_expression_obj(obj1->log.base, obj2->log.base) && is_equal_expression_obj(obj1->log.arg, obj2->log.arg);
        case EXPR_OBJ_TYPE_Exp:
            return is_equal_expression_obj(obj1->exp.arg, obj2->exp.arg);
        case EXPR_OBJ_TYPE_Pow:
            return is_equal_expression_obj(obj1->pow.base, obj2->pow.base) && is_equal_expression_obj(obj1->pow.exponent, obj2->pow.exponent);
        case EXPR_OBJ_TYPE_Constant:
            return obj1->constant == obj2->constant;
        case EXPR_OBJ_TYPE_Variable:
            return strcmp(obj1->variable, obj2->variable) == 0;
    }
    return 0;
}

long long hash_expression_obj(ExpressionObj *obj) {
    if (obj == NULL) {
        return 0;
    }
    long long hash = 0;
    switch (obj->type) {
        case EXPR_OBJ_TYPE_Monomial:
            hash = hash_monomial(obj->monomial);
            break;
        case EXPR_OBJ_TYPE_Polynomial:
            hash = hash_polynomial(obj->polynomial);
            break;
        case EXPR_OBJ_TYPE_Sin:
            hash = hash_expression_obj(obj->sin.arg);
            break;
        case EXPR_OBJ_TYPE_Cos:
            hash = hash_expression_obj(obj->cos.arg);
            break;
        case EXPR_OBJ_TYPE_Tan:
            hash = hash_expression_obj(obj->tan.arg);
            break;
        case EXPR_OBJ_TYPE_Ln:
            hash = hash_expression_obj(obj->ln.arg);
            break;
        case EXPR_OBJ_TYPE_Log:
            hash = hash_expression_obj(obj->log.base) ^ hash_expression_obj(obj->log.arg);
            break;
        case EXPR_OBJ_TYPE_Exp:
            hash = hash_expression_obj(obj->exp.arg);
            break;
        case EXPR_OBJ_TYPE_Pow:
            hash = hash_expression_obj(obj->pow.base) ^ hash_expression_obj(obj->pow.exponent);
            break;
        case EXPR_OBJ_TYPE_Constant:
            hash = obj->constant;
            break;
        case EXPR_OBJ_TYPE_Variable:
            for (int i = 0; i < VARIABLE_NAME_SIZE; i++) {
                hash += obj->variable[i];
                hash *= 31;
            }
            break;
    }
    return hash;
}

ExpressionObj* deep_copy_expression_obj(ExpressionObj* obj) {
    if (obj == NULL) {
        return NULL;
    }
    ExpressionObj* new_obj = create_expression_obj(obj->type);
    switch (obj->type) {
        case EXPR_OBJ_TYPE_Monomial:
            new_obj->monomial = deep_copy_monomial(obj->monomial);
            break;
        case EXPR_OBJ_TYPE_Polynomial:
            new_obj->polynomial = deep_copy_polynomial(obj->polynomial);
            break;
        case EXPR_OBJ_TYPE_Sin:
            new_obj->sin.arg = deep_copy_expression_obj(obj->sin.arg);
            break;
        case EXPR_OBJ_TYPE_Cos:
            new_obj->cos.arg = deep_copy_expression_obj(obj->cos.arg);
            break;
        case EXPR_OBJ_TYPE_Tan:
            new_obj->tan.arg = deep_copy_expression_obj(obj->tan.arg);
            break;
        case EXPR_OBJ_TYPE_Ln:
            new_obj->ln.arg = deep_copy_expression_obj(obj->ln.arg);
            break;
        case EXPR_OBJ_TYPE_Log:
            new_obj->log.base = deep_copy_expression_obj(obj->log.base);
            new_obj->log.arg = deep_copy_expression_obj(obj->log.arg);
            break;
        case EXPR_OBJ_TYPE_Exp:
            new_obj->exp.arg = deep_copy_expression_obj(obj->exp.arg);
            break;
        case EXPR_OBJ_TYPE_Pow:
            new_obj->pow.base = deep_copy_expression_obj(obj->pow.base);
            new_obj->pow.exponent = deep_copy_expression_obj(obj->pow.exponent);
            break;
        case EXPR_OBJ_TYPE_Constant:
            new_obj->constant = obj->constant;
            break;
        case EXPR_OBJ_TYPE_Variable:
            strcpy(new_obj->variable, obj->variable);
            break;
    }
    return new_obj;
}

ExpressionObj* create_expression_obj_from_ast(Node* node) {
    if (node == NULL) {
        return NULL;
    }
    ExpressionObj* obj;
    switch (node->type) {
        case NODE_CONSTANT:
            obj = create_expression_obj(EXPR_OBJ_TYPE_Constant);
            obj->constant = node->data.constant;
            break;
        case NODE_VARIABLE:
            obj = create_expression_obj(EXPR_OBJ_TYPE_Variable);
            strcpy(obj->variable, node->data.variable);
            break;
        case FUNCTION_ADD:
        case FUNCTION_SUBTRACT:
        case FUNCTION_MULTIPLY:
        case FUNCTION_DIVIDE:
            obj = create_expression_obj(EXPR_OBJ_TYPE_Polynomial);
            obj->polynomial = create_polynomial_from_ast(node);
            break;
        case FUNCTION_POWER:
            obj = create_expression_obj(EXPR_OBJ_TYPE_Pow);
            obj->pow.base = create_expression_obj_from_ast(node->data.function.left);
            obj->pow.exponent = create_expression_obj_from_ast(node->data.function.right);
            break;
        case FUNCTION_SIN:
            obj = create_expression_obj(EXPR_OBJ_TYPE_Sin);
            obj->sin.arg = create_expression_obj_from_ast(node->data.function.left);
            break;
        case FUNCTION_COS:
            obj = create_expression_obj(EXPR_OBJ_TYPE_Cos);
            obj->cos.arg = create_expression_obj_from_ast(node->data.function.left);
            break;
        case FUNCTION_TAN:
            obj = create_expression_obj(EXPR_OBJ_TYPE_Tan);
            obj->tan.arg = create_expression_obj_from_ast(node->data.function.left);
            break;
        case FUNCTION_LN:
            obj = create_expression_obj(EXPR_OBJ_TYPE_Ln);
            obj->ln.arg = create_expression_obj_from_ast(node->data.function.left);
            break;
        case FUNCTION_LOG:
            obj = create_expression_obj(EXPR_OBJ_TYPE_Log);
            obj->log.base = create_expression_obj_from_ast(node->data.function.left);
            obj->log.arg = create_expression_obj_from_ast(node->data.function.right);
            break;
        case FUNCTION_EXP:
            obj = create_expression_obj(EXPR_OBJ_TYPE_Exp);
            obj->exp.arg = create_expression_obj_from_ast(node->data.function.left);
            break;
    }
    return obj;
}

Node* expression_obj_to_ast(ExpressionObj* obj) {
    if (obj == NULL) {
        return NULL;
    }
    Node* node;
    switch (obj->type) {
        case EXPR_OBJ_TYPE_Monomial:
            node = monomial_to_ast(obj->monomial);
            break;
        case EXPR_OBJ_TYPE_Polynomial:
            node = polynomial_to_ast(obj->polynomial);
            break;
        case EXPR_OBJ_TYPE_Sin:
            node = create_node(FUNCTION_SIN, "", 0, 0);
            node->data.function.left = expression_obj_to_ast(obj->sin.arg);
            break;
        case EXPR_OBJ_TYPE_Cos:
            node = create_node(FUNCTION_COS, "", 0, 0);
            node->data.function.left = expression_obj_to_ast(obj->cos.arg);
            break;
        case EXPR_OBJ_TYPE_Tan:
            node = create_node(FUNCTION_TAN, "", 0, 0);
            node->data.function.left = expression_obj_to_ast(obj->tan.arg);
            break;
        case EXPR_OBJ_TYPE_Ln:
            node = create_node(FUNCTION_LN, "", 0, 0);
            node->data.function.left = expression_obj_to_ast(obj->ln.arg);
            break;
        case EXPR_OBJ_TYPE_Log:
            node = create_node(FUNCTION_LOG, "", 0, 0);
            node->data.function.left = expression_obj_to_ast(obj->log.base);
            node->data.function.right = expression_obj_to_ast(obj->log.arg);
            break;
        case EXPR_OBJ_TYPE_Exp:
            node = create_node(FUNCTION_EXP, "", 0, 0);
            node->data.function.left = expression_obj_to_ast(obj->exp.arg);
            break;
        case EXPR_OBJ_TYPE_Pow:
            node = create_node(FUNCTION_POWER, "", 0, 0);
            node->data.function.left = expression_obj_to_ast(obj->pow.base);
            node->data.function.right = expression_obj_to_ast(obj->pow.exponent);
            break;
        case EXPR_OBJ_TYPE_Constant:
            node = create_node(NODE_CONSTANT, "", 0, 0);
            node->data.constant = obj->constant;
            break;
        case EXPR_OBJ_TYPE_Variable:
            node = create_node(NODE_VARIABLE, obj->variable, 0, VARIABLE_NAME_SIZE - 1);
            break;
    }
    return node;
}

int is_zero_expression_obj(ExpressionObj* obj) {
    if (obj == NULL) {
        return 0;
    }
    switch (obj->type) {
        case EXPR_OBJ_TYPE_Monomial:
            return is_zero_monomial(obj->monomial);
        case EXPR_OBJ_TYPE_Polynomial:
            return is_zero_polynomial(obj->polynomial);
        case EXPR_OBJ_TYPE_Sin:
            return is_zero_expression_obj(obj->sin.arg);
        case EXPR_OBJ_TYPE_Cos:
            return is_zero_expression_obj(obj->cos.arg);
        case EXPR_OBJ_TYPE_Tan:
            return is_zero_expression_obj(obj->tan.arg);
        case EXPR_OBJ_TYPE_Ln:
            return is_zero_expression_obj(obj->ln.arg);
        case EXPR_OBJ_TYPE_Log:
            return is_zero_expression_obj(obj->log.base) && is_zero_expression_obj(obj->log.arg);
        case EXPR_OBJ_TYPE_Exp:
            return is_zero_expression_obj(obj->exp.arg);
        case EXPR_OBJ_TYPE_Pow:
            return is_zero_expression_obj(obj->pow.base) && is_zero_expression_obj(obj->pow.exponent);
        case EXPR_OBJ_TYPE_Constant:
            return obj->constant == 0;
        case EXPR_OBJ_TYPE_Variable:
            return 0;
    }
    return 0;
}