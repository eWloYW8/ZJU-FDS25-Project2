#include "monomial.h"
#include "ast.h"
#include "astmath.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>

Monomial *create_monomial() {
    Monomial *monomial = (Monomial *)malloc(sizeof(Monomial));
    monomial->coefficient = 1;
    monomial->coefficient_denominator = 1;
    monomial->count = 0;
    for (int i = 0; i < MAX_VARIABLES; i++) {
        monomial->node[i] = NULL;
        monomial->exponent[i] = 0;
    }
    return monomial;
}

void free_monomial(Monomial *monomial) {
    for (int i = 0; i < monomial->count; i++) {
        free_node(monomial->node[i]);
    }
    free(monomial);
}

int find_monomial(Monomial *monomial, Node *node) {
    for (int i = 0; i < monomial->count; i++) {
        if (is_equal(monomial->node[i], node)) {
            return i;
        }
    }
    return -1;
}

void _add_monomial(Monomial *monomial, Node *node, long long exponent_magnification) {
    int index;
    if (node == NULL) {
        return;
    }
    switch (node->type) {
        case NODE_VARIABLE:
            index = find_monomial(monomial, node);
            if (index == -1) {
                if (monomial->count < MAX_VARIABLES) {
                    monomial->node[monomial->count] = deep_copy_node(node);
                    monomial->exponent[monomial->count] = exponent_magnification;
                    monomial->count++;
                } else {
                    fprintf(stderr, "Error: Maximum number of variables exceeded.\n");
                }
            } else {
                monomial->exponent[index] += exponent_magnification;
            }
            break;
        case NODE_CONSTANT:
            if (exponent_magnification > 0) {
                monomial->coefficient *= fastpow(node->data.constant, exponent_magnification);
            } else {
                monomial->coefficient_denominator *= fastpow(node->data.constant, -exponent_magnification);
            }
            break;
        case FUNCTION_MULTIPLY:
            _add_monomial(monomial, node->data.function.left, exponent_magnification);
            _add_monomial(monomial, node->data.function.right, exponent_magnification);
            break;
        case FUNCTION_DIVIDE:
            _add_monomial(monomial, node->data.function.left, exponent_magnification);
            _add_monomial(monomial, node->data.function.right, -exponent_magnification);
            break;
        case FUNCTION_POWER:
            if (node->data.function.right->type == NODE_CONSTANT) {
                long long exponent = node->data.function.right->data.constant;
                _add_monomial(monomial, node->data.function.left, exponent * exponent_magnification);
                break;
            }
            else {
                index = find_monomial(monomial, node);
                if (index == -1) {
                    if (monomial->count < MAX_VARIABLES) {
                        monomial->node[monomial->count] = deep_copy_node(node);
                        monomial->exponent[monomial->count] = exponent_magnification;
                        monomial->count++;
                    } else {
                        fprintf(stderr, "Error: Maximum number of variables exceeded.\n");
                    }
                } else {
                    monomial->exponent[index] += exponent_magnification;
                }
            }
        default:
            index = find_monomial(monomial, node);
            if (index == -1) {
                if (monomial->count < MAX_VARIABLES) {
                    monomial->node[monomial->count] = deep_copy_node(node);
                    monomial->exponent[monomial->count] = exponent_magnification;
                    monomial->count++;
                } else {
                    fprintf(stderr, "Error: Maximum number of variables exceeded.\n");
                }
            } else {
                monomial->exponent[index] += exponent_magnification;
            }
    }
}

void add_monomial(Monomial *monomial, Node *node) {
    _add_monomial(monomial, node, 1);
}

Monomial *node_to_monomial(Node *node) {
    Monomial *monomial = create_monomial();
    _add_monomial(monomial, node, 1);
    return monomial;
}

Node *monomial_to_node(Monomial *monomial) {
    sort_coefficient_monomial(monomial);
    if (monomial->count == 0) {
        return create_node_with_constant(monomial->coefficient);
    }
    Node *current = create_node(FUNCTION_DIVIDE, NULL, 0, 0);
    current->data.function.left = create_node_with_constant(monomial->coefficient);
    current->data.function.right = create_node_with_constant(monomial->coefficient_denominator);
    for (int i = 0; i < monomial->count; i++) {
        if (monomial->exponent[i] == 0) {
            continue;
        }
        else if (monomial->exponent[i] == 1) {
            Node* temp = create_node(FUNCTION_MULTIPLY, NULL, 0, 0);
            temp->data.function.left = current;
            temp->data.function.right = deep_copy_node(monomial->node[i]);
            current = temp;
        } else if (monomial->exponent[i] == -1) {
            Node* temp = create_node(FUNCTION_DIVIDE, NULL, 0, 0);
            temp->data.function.left = current;
            temp->data.function.right = deep_copy_node(monomial->node[i]);
            current = temp;
        } else {
            Node* temp = create_node(FUNCTION_POWER, NULL, 0, 0);
            temp->data.function.left = deep_copy_node(monomial->node[i]);
            temp->data.function.right = create_node_with_constant(monomial->exponent[i]);
            Node* temp2 = create_node(FUNCTION_MULTIPLY, NULL, 0, 0);
            temp2->data.function.left = current;
            temp2->data.function.right = temp;
            current = temp2;
        }
    }
    return current;
}

void sort_coefficient_monomial(Monomial *monomial) {
    int negative = 0;
    if (monomial->coefficient_denominator < 0) {
        monomial->coefficient_denominator *= -1;
        monomial->coefficient *= -1;
    }
    if (monomial->coefficient < 0) {
        negative = 1;
        monomial->coefficient *= -1;
    }
    long long coefficient_gcd = gcd(monomial->coefficient, monomial->coefficient_denominator);
    monomial->coefficient /= coefficient_gcd;
    monomial->coefficient_denominator /= coefficient_gcd;
    if (negative) {
        monomial->coefficient *= -1;
    }
}

Node *monomial_simplify(Node *node) {
    if (node->type == FUNCTION_MULTIPLY || node->type == FUNCTION_DIVIDE) {
        Monomial *monomial = node_to_monomial(node);
        Node *result = monomial_to_node(monomial);
        free_monomial(monomial);
        return result;
    }
    return node;
}

Node *monomial_simplify_recursive(Node *node) {
    if (node == NULL) {
        return NULL;
    }
if (node->type == FUNCTION_ADD || node->type == FUNCTION_SUBTRACT || node->type == FUNCTION_LOG || node->type == FUNCTION_EXP || node->type == FUNCTION_SIN || node->type == FUNCTION_COS || node->type == FUNCTION_TAN || node->type == FUNCTION_LN || node->type == FUNCTION_POWER) {
        node->data.function.left = monomial_simplify_recursive(node->data.function.left);
        node->data.function.right = monomial_simplify_recursive(node->data.function.right);
    }
    return monomial_simplify(node);
}