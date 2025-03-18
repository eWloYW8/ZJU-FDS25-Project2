#include "./expressionobj.h"
#include "./monomial.h"
#include "./polynomial.h"
#include "../ast.h"
#include "../astmath.h"

#include <stdlib.h>

// create_expression_obj creates a new expression object.
ExpressionObj* create_expression_obj(enum ExpressionObjType type) {
    ExpressionObj *obj = malloc(sizeof(ExpressionObj));
    obj->type = type;
    switch (type) {
        case EXPRESSION_OBJ_AST:
            obj->data.ast = NULL;
            break;
        case EXPRESSION_OBJ_MONOMIAL:
            obj->data.monomial = NULL;
            break;
        case EXPRESSION_OBJ_POLYNOMIAL:
            obj->data.polynomial = NULL;
            break;
    }
    return obj;
}

// free_expression_obj frees the memory of an expression object.
void free_expression_obj(ExpressionObj *obj) {
    switch (obj->type) {
        case EXPRESSION_OBJ_AST:
            free_node(obj->data.ast);
            break;
        case EXPRESSION_OBJ_MONOMIAL:
            free_monomial(obj->data.monomial);
            break;
        case EXPRESSION_OBJ_POLYNOMIAL:
            free_polynomial(obj->data.polynomial);
            break;
    }
    free(obj);
}

// deep_copy_expression_obj creates a deep copy of an expression object.
ExpressionObj* deep_copy_expression_obj(ExpressionObj *obj) {
    ExpressionObj *new_obj = create_expression_obj(obj->type);
    switch (obj->type) {
        case EXPRESSION_OBJ_AST:
            new_obj->data.ast = deep_copy_node(obj->data.ast);
            break;
        case EXPRESSION_OBJ_MONOMIAL:
            new_obj->data.monomial = deep_copy_monomial(obj->data.monomial);
            break;
        case EXPRESSION_OBJ_POLYNOMIAL:
            new_obj->data.polynomial = deep_copy_polynomial(obj->data.polynomial);
            break;
    }
    return new_obj;
}

// expression_obj_is_equal checks if two expression objects are equal.
int expression_obj_is_equal(ExpressionObj *obj1, ExpressionObj *obj2) {
    if (obj1->type != obj2->type) {
        return 0;
    }
    switch (obj1->type) {
        case EXPRESSION_OBJ_AST:
            return ast_is_equal(obj1->data.ast, obj2->data.ast);
        case EXPRESSION_OBJ_MONOMIAL:
            return monomial_is_equal(obj1->data.monomial, obj2->data.monomial);
        case EXPRESSION_OBJ_POLYNOMIAL:
            return polynomial_is_equal(obj1->data.polynomial, obj2->data.polynomial);
    }
    return 0;
}

// expression_obj_to_ast converts an expression object to an AST node.
Node* expression_obj_to_ast(ExpressionObj *obj) {
    switch (obj->type) {
        case EXPRESSION_OBJ_AST:
            return deep_copy_node(obj->data.ast);
        case EXPRESSION_OBJ_MONOMIAL:
            return monomial_to_ast(obj->data.monomial);
        case EXPRESSION_OBJ_POLYNOMIAL:
            return polynomial_to_ast(obj->data.polynomial);
    }
    return NULL;
}

// expression_obj_get_hash returns the hash of an expression object.
unsigned long long expression_obj_get_hash(ExpressionObj *obj) {
    switch (obj->type) {
        case EXPRESSION_OBJ_AST:
            return ast_get_hash(obj->data.ast);
        case EXPRESSION_OBJ_MONOMIAL:
            return monomial_get_hash(obj->data.monomial);
        case EXPRESSION_OBJ_POLYNOMIAL:
            return polynomial_get_hash(obj->data.polynomial);
    }
    return 0;
}

// expression_obj_from_ast creates an expression object from an AST node.
ExpressionObj* expression_obj_from_ast(Node *ast) {
    if (ast->type == NODE_CONSTANT || ast->type == NODE_VARIABLE) {
        ExpressionObj *obj = create_expression_obj(EXPRESSION_OBJ_AST);
        obj->data.ast = deep_copy_node(ast);
        return obj;
    }
    else if(ast->type >= FUNCTION_ADD && ast->type <= FUNCTION_SUBTRACT) {
        Polynomial* polynomial = polynomial_from_ast(ast);
        if (polynomial_get_length(polynomial) == 0) {
            free_polynomial(polynomial);
            ExpressionObj *obj = create_expression_obj(EXPRESSION_OBJ_AST);
            obj->data.ast = create_node_from_constant(polynomial->constant);
            free_polynomial(polynomial);
            return obj;
        }
        else if (polynomial_get_length(polynomial) == 1 && polynomial->constant == 0) {
            ExpressionObj *obj = create_expression_obj(EXPRESSION_OBJ_MONOMIAL);
            obj->data.monomial = monomial_from_ast(polynomial_to_ast(polynomial));
            free_polynomial(polynomial);
            return obj;
        }
        else {
            ExpressionObj *obj = create_expression_obj(EXPRESSION_OBJ_POLYNOMIAL);
            obj->data.polynomial = polynomial;
            return obj;
        }
    }
    else if(ast->type >= FUNCTION_MULTIPLY && ast->type <= FUNCTION_DIVIDE) {
        Monomial* monomial = monomial_from_ast(ast);
        if (monomial->coefficient == 0) {
            free_monomial(monomial);
            ExpressionObj *obj = create_expression_obj(EXPRESSION_OBJ_AST);
            obj->data.ast = create_node_from_constant(0);
            free_monomial(monomial);
            return obj;
        }
        else if (monomial_get_length(monomial) == 0) {
            free_monomial(monomial);
            ExpressionObj *obj = create_expression_obj(EXPRESSION_OBJ_AST);
            obj->data.ast = create_node_from_constant(monomial->coefficient);
            free_monomial(monomial);
            return obj;
        }
        else {
            ExpressionObj *obj = create_expression_obj(EXPRESSION_OBJ_MONOMIAL);
            obj->data.monomial = monomial;
            return obj;
        }
    }
    else {
        ExpressionObj *obj = create_expression_obj(EXPRESSION_OBJ_AST);
        obj->data.ast = create_node(ast->type, NULL, 0, 0);
        if(ast->data.function.left != NULL) {
            ExpressionObj *left = expression_obj_from_ast(ast->data.function.left);
            obj->data.ast->data.function.left = expression_obj_to_ast(left);
            free_expression_obj(left);
        }
        if(ast->data.function.right != NULL) {
            ExpressionObj *right = expression_obj_from_ast(ast->data.function.right);
            obj->data.ast->data.function.right = expression_obj_to_ast(right);
            free_expression_obj(right);
        }
        return obj;
    }
}