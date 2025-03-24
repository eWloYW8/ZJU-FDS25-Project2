#include "stack.h"
#include <stdlib.h>

// Function to create a stack with a given capacity
// Allocates memory for the stack structure and its data array
Stack* create_stack(int capacity) {
    Stack *s = (Stack*)malloc(sizeof(Stack)); // Allocate memory for the stack
    s->data = (Node**)malloc(sizeof(Node*) * capacity); // Allocate memory for the stack's data array
    s->top = -1; // Initialize the top index to -1, indicating the stack is empty
    s->capacity = capacity; // Set the stack's capacity
    return s; // Return the created stack
}

// Function to free the memory allocated for the stack
void free_stack(Stack *s) {
    free(s->data); // Free the memory allocated for the stack's data array
    free(s); // Free the memory allocated for the stack structure
}

// Function to push a node onto the stack
// Adds the node to the top of the stack if there is space
void push(Stack *s, Node *node) {
    if (s->top < s->capacity - 1) { // Check if there is space in the stack
        s->data[++s->top] = node; // Increment the top index and add the node to the stack
    }
}

// Function to pop a node from the stack
// Removes and returns the node from the top of the stack if the stack is not empty
Node* pop(Stack *s) {
    if (s->top >= 0) { // Check if the stack is not empty
        return s->data[s->top--]; // Return the top node and decrement the top index
    }
    return NULL; // Return NULL if the stack is empty
}

// Function to peek at the top node of the stack without removing it
// Returns the node at the top of the stack if the stack is not empty
Node* peek(Stack *s) {
    if (s->top >= 0) { // Check if the stack is not empty
        return s->data[s->top]; // Return the top node
    }
    return NULL; // Return NULL if the stack is empty
}

// Function to check if the stack is empty
// Returns 1 if the stack is empty, 0 otherwise
int is_empty(Stack *s) {
    return s->top == -1; // Return 1 if the top index is -1, indicating the stack is empty
}