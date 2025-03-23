#include "ast.h"
#include "utils.h"
#include "astmath.h"
#include "../simplificator/one_layer.h"
#include "../simplificator/expressionobj.h"

#include <stdlib.h>
#include <stdio.h>

int main() {
    char expression[] = "(a+b)*(c+d)";
    int current_position = 0;
    ExpressionObj* obj = create_expression_obj_from_ast(build_ast(expression, &current_position));
    Node* node = expression_obj_to_ast(obj);
    printf("%s\n", node_to_string(node));
}