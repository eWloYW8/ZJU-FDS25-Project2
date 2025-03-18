#pragma once

#include "monomial.h"
#include "polynomial.h"
#include "../ast.h"
#include "../ads/hashtable.h"



enum ExpressionObjType {
    EXPRESSION_OBJ_AST,
    EXPRESSION_OBJ_MONOMIAL,
    EXPRESSION_OBJ_POLYNOMIAL,
};

typedef struct ExpressionObj {
    enum ExpressionObjType type;
    union {
        Node *ast;
        Monomial *monomial;
        Polynomial *polynomial;
    } data;
} ExpressionObj;

// create_expression_obj creates a new expression object.
ExpressionObj* create_expression_obj(enum ExpressionObjType type);

// free_expression_obj frees the memory of an expression object.
void free_expression_obj(ExpressionObj *obj);

// deep_copy_expression_obj creates a deep copy of an expression object.
ExpressionObj* deep_copy_expression_obj(ExpressionObj *obj);

// expression_obj_is_equal checks if two expression objects are equal.
int expression_obj_is_equal(ExpressionObj *obj1, ExpressionObj *obj2);

// expression_obj_to_ast converts an expression object to an AST node.
Node* expression_obj_to_ast(ExpressionObj *obj);

// expression_obj_get_hash returns the hash of an expression object.
unsigned long long expression_obj_get_hash(ExpressionObj *obj);

// expression_obj_from_ast creates an expression object from an AST node.
ExpressionObj* expression_obj_from_ast(Node *ast);