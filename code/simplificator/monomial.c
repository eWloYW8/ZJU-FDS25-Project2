#include "monomial.h"
#include "ast.h"
#include "astmath.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>

// Function to create a new monomial
Monomial *create_monomial() {
    Monomial *monomial = (Monomial *)malloc(sizeof(Monomial)); // Allocate memory for monomial
    monomial->coefficient = 1; // Initialize coefficient to 1
    monomial->coefficient_denominator = 1; // Initialize coefficient denominator to 1
    monomial->count = 0; // Initialize count of variables to 0
    for (int i = 0; i < MAX_VARIABLES; i++) {
        monomial->node[i] = NULL; // Initialize node pointers to NULL
        monomial->exponent[i] = 0; // Initialize exponents to 0
    }
    return monomial; // Return the created monomial
}

// Function to free the memory allocated for a monomial
void free_monomial(Monomial *monomial) {
    for (int i = 0; i < monomial->count; i++) {
        free_node(monomial->node[i]); // Free each node in the monomial
    }
    free(monomial); // Free the monomial itself
}

// Function to find a node in the monomial
int find_monomial(Monomial *monomial, Node *node) {
    for (int i = 0; i < monomial->count; i++) {
        if (is_equal(monomial->node[i], node)) { // Check if nodes are equal
            return i; // Return index if found
        }
    }
    return -1; // Return -1 if not found
}

// Internal function to add a node to the monomial with a given exponent magnification
void _add_monomial(Monomial *monomial, Node *node, long long exponent_magnification) {
    int index;
    if (node == NULL) {
        return; // Return if node is NULL
    }
    switch (node->type) {
        case NODE_VARIABLE:
            index = find_monomial(monomial, node); // Find the node in the monomial
            if (index == -1) {
                if (monomial->count < MAX_VARIABLES) {
                    monomial->node[monomial->count] = deep_copy_node(node); // Deep copy the node
                    monomial->exponent[monomial->count] = exponent_magnification; // Set the exponent
                    monomial->count++; // Increment the count
                } else {
                    fprintf(stderr, "Error: Maximum number of variables exceeded.\n"); // Error if max variables exceeded
                }
            } else {
                monomial->exponent[index] += exponent_magnification; // Add to the existing exponent
            }
            break;
        case NODE_CONSTANT:
            if (exponent_magnification > 0) {
                monomial->coefficient *= fastpow(node->data.constant, exponent_magnification); // Multiply coefficient
            } else {
                monomial->coefficient_denominator *= fastpow(node->data.constant, -exponent_magnification); // Multiply denominator
            }
            break;
        case FUNCTION_MULTIPLY:
            _add_monomial(monomial, node->data.function.left, exponent_magnification); // Recursively add left node
            _add_monomial(monomial, node->data.function.right, exponent_magnification); // Recursively add right node
            break;
        case FUNCTION_DIVIDE:
            _add_monomial(monomial, node->data.function.left, exponent_magnification); // Recursively add left node
            _add_monomial(monomial, node->data.function.right, -exponent_magnification); // Recursively add right node with negative exponent
            break;
        case FUNCTION_POWER:
            if (node->data.function.right->type == NODE_CONSTANT) {
                long long exponent = node->data.function.right->data.constant;
                _add_monomial(monomial, node->data.function.left, exponent * exponent_magnification); // Multiply exponents
                break;
            } else {
                index = find_monomial(monomial, node); // Find the node in the monomial
                if (index == -1) {
                    if (monomial->count < MAX_VARIABLES) {
                        monomial->node[monomial->count] = deep_copy_node(node); // Deep copy the node
                        monomial->exponent[monomial->count] = exponent_magnification; // Set the exponent
                        monomial->count++; // Increment the count
                    } else {
                        fprintf(stderr, "Error: Maximum number of variables exceeded.\n"); // Error if max variables exceeded
                    }
                } else {
                    monomial->exponent[index] += exponent_magnification; // Add to the existing exponent
                }
                break;
            }
        default:
            index = find_monomial(monomial, node); // Find the node in the monomial
            if (index == -1) {
                if (monomial->count < MAX_VARIABLES) {
                    monomial->node[monomial->count] = deep_copy_node(node); // Deep copy the node
                    monomial->exponent[monomial->count] = exponent_magnification; // Set the exponent
                    monomial->count++; // Increment the count
                } else {
                    fprintf(stderr, "Error: Maximum number of variables exceeded.\n"); // Error if max variables exceeded
                }
            } else {
                monomial->exponent[index] += exponent_magnification; // Add to the existing exponent
            }
    }
}

// Function to add a node to the monomial with an exponent of 1
void add_monomial(Monomial *monomial, Node *node) {
    _add_monomial(monomial, node, 1); // Call internal function with exponent 1
}

// Function to convert a node to a monomial
Monomial *node_to_monomial(Node *node) {
    Monomial *monomial = create_monomial(); // Create a new monomial
    _add_monomial(monomial, node, 1); // Add the node to the monomial with exponent 1
    return monomial; // Return the created monomial
}

// Function to convert a monomial to a node
Node *monomial_to_node(Monomial *monomial) {
    sort_coefficient_monomial(monomial); // Sort the coefficients
    if (monomial->count == 0) {
        Node *current = create_node(FUNCTION_DIVIDE, NULL, 0, 0); // Create a divide node
        current->data.function.left = create_node_with_constant(monomial->coefficient); // Set left to coefficient
        current->data.function.right = create_node_with_constant(monomial->coefficient_denominator); // Set right to denominator
        return current; // Return the constructed node
    }
    Node *current = create_node(FUNCTION_DIVIDE, NULL, 0, 0); // Create a divide node
    current->data.function.left = create_node_with_constant(monomial->coefficient); // Set left to coefficient
    current->data.function.right = create_node_with_constant(monomial->coefficient_denominator); // Set right to denominator
    for (int i = 0; i < monomial->count; i++) {
        if (monomial->exponent[i] == 0) {
            continue; // Skip if exponent is 0
        } else if (monomial->exponent[i] == 1) {
            Node* temp = create_node(FUNCTION_MULTIPLY, NULL, 0, 0); // Create multiply node
            temp->data.function.left = current; // Set left to current node
            temp->data.function.right = deep_copy_node(monomial->node[i]); // Set right to deep copy of node
            current = temp; // Update current node
        } else if (monomial->exponent[i] == -1) {
            Node* temp = create_node(FUNCTION_DIVIDE, NULL, 0, 0); // Create divide node
            temp->data.function.left = current; // Set left to current node
            temp->data.function.right = deep_copy_node(monomial->node[i]); // Set right to deep copy of node
            current = temp; // Update current node
        } else {
            Node* temp = create_node(FUNCTION_POWER, NULL, 0, 0); // Create power node
            temp->data.function.left = deep_copy_node(monomial->node[i]); // Set left to deep copy of node
            temp->data.function.right = create_node_with_constant(monomial->exponent[i]); // Set right to exponent
            Node* temp2 = create_node(FUNCTION_MULTIPLY, NULL, 0, 0); // Create multiply node
            temp2->data.function.left = current; // Set left to current node
            temp2->data.function.right = temp; // Set right to power node
            current = temp2; // Update current node
        }
    }
    return current; // Return the constructed node
}

// Function to sort the coefficients of a monomial
void sort_coefficient_monomial(Monomial *monomial) {
    int negative = 0;
    if (monomial->coefficient_denominator < 0) {
        monomial->coefficient_denominator *= -1; // Make denominator positive
        monomial->coefficient *= -1; // Make coefficient negative
    }
    if (monomial->coefficient < 0) {
        negative = 1; // Mark as negative
        monomial->coefficient *= -1; // Make coefficient positive
    }
    long long coefficient_gcd = gcd(monomial->coefficient, monomial->coefficient_denominator); // Calculate GCD
    monomial->coefficient /= coefficient_gcd; // Divide coefficient by GCD
    monomial->coefficient_denominator /= coefficient_gcd; // Divide denominator by GCD
    if (negative) {
        monomial->coefficient *= -1; // Restore negative sign
    }
}

// Function to simplify a node using monomials
Node *monomial_simplify(Node *node) {
    if (node->type == FUNCTION_MULTIPLY || node->type == FUNCTION_DIVIDE) {
        Monomial *monomial = node_to_monomial(node); // Convert node to monomial
        Node *result = monomial_to_node(monomial); // Convert monomial back to node
        free_monomial(monomial); // Free the monomial
        return result; // Return the simplified node
    }
    return node; // Return the original node if not multiply or divide
}

// Function to recursively simplify a node using monomials
Node *monomial_simplify_recursive(Node *node) {
    if (node == NULL) {
        return NULL; // Return NULL if node is NULL
    }
    if (node->type == FUNCTION_ADD || node->type == FUNCTION_SUBTRACT || node->type == FUNCTION_LOG || node->type == FUNCTION_EXP || node->type == FUNCTION_SIN || node->type == FUNCTION_COS || node->type == FUNCTION_TAN || node->type == FUNCTION_LN || node->type == FUNCTION_POWER || node->type == FUNCTION_DIVIDE || node->type == FUNCTION_MULTIPLY) {
        node->data.function.left = monomial_simplify_recursive(node->data.function.left); // Recursively simplify left node
        node->data.function.right = monomial_simplify_recursive(node->data.function.right); // Recursively simplify right node
    }
    return monomial_simplify(node); // Simplify the current node
}