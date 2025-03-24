// This file contains the declaration of the Stack data structure and its functions.
// The Stack data structure is a stack of Node pointers.
#pragma once

#include "ast.h"

// Define the Stack structure
typedef struct Stack {
    Node **data;    // Pointer to an array of Node pointers, representing the stack's elements
    int top;        // Index of the top element in the stack
    int capacity;   // Maximum number of elements the stack can hold
} Stack;

// Function to create a new stack with a given capacity
// Parameters:
// - capacity: the maximum number of elements the stack can hold
// Returns:
// - A pointer to the newly created stack
Stack* create_stack(int capacity);

// Function to free the memory allocated for the stack
// Parameters:
// - s: pointer to the stack to be freed
void free_stack(Stack *s);

// Function to push a new node onto the stack
// Parameters:
// - s: pointer to the stack
// - node: pointer to the node to be pushed onto the stack
void push(Stack *s, Node *node);

// Function to pop the top node from the stack
// Parameters:
// - s: pointer to the stack
// Returns:
// - A pointer to the node that was popped from the stack
Node* pop(Stack *s);

// Function to peek at the top node of the stack without removing it
// Parameters:
// - s: pointer to the stack
// Returns:
// - A pointer to the node at the top of the stack
Node* peek(Stack *s);

// Function to check if the stack is empty
// Parameters:
// - s: pointer to the stack
// Returns:
// - An integer value: 1 if the stack is empty, 0 otherwise
int is_empty(Stack *s);