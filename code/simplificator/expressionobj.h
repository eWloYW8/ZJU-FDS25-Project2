#pragma once

#include "../ast.h"

typedef struct Monomial Monomial;

typedef struct Polynomial Polynomial;

typedef struct sinObj {
    struct ExpressionObj* arg;
} sinObj;

typedef struct cosObj {
    struct ExpressionObj* arg;
} cosObj;

typedef struct tanObj {
    struct ExpressionObj* arg;
} tanObj;

typedef struct lnObj {
    struct ExpressionObj* arg;
} lnObj;

typedef struct logObj {
    struct ExpressionObj* base;
    struct ExpressionObj* arg;
} logObj;

typedef struct expObj {
    struct ExpressionObj* arg;
} expObj;

enum ExpressionObjType {
    EXPR_OBJ_TYPE_Monomial,
    EXPR_OBJ_TYPE_Polynomial,
    EXPR_OBJ_TYPE_Sin,
    EXPR_OBJ_TYPE_Cos,
    EXPR_OBJ_TYPE_Tan,
    EXPR_OBJ_TYPE_Ln,
    EXPR_OBJ_TYPE_Log,
    EXPR_OBJ_TYPE_Exp
} type;

typedef struct ExpressionObj {
    enum ExpressionObjType type;
    union {
        struct Monomial* monomial;
        struct Polynomial* polynomial;
        struct sinObj sin;
        struct cosObj cos;
        struct tanObj tan;
        struct lnObj ln;
        struct logObj log;
        struct expObj exp;
    };
} ExpressionObj;

ExpressionObj* create_expression_obj(enum ExpressionObjType type);

void destroy_expression_obj(ExpressionObj* obj);

int is_equal_expression_obj(ExpressionObj* obj1, ExpressionObj* obj2);

long long hash_expression_obj(ExpressionObj* obj);

ExpressionObj create_expression_obj_from_ast(Node* node);

ExpressionObj* deep_copy_expression_obj(ExpressionObj* obj);

Node* expression_obj_to_ast(ExpressionObj* obj);

