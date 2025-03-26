#include "stack.h" // Include the header file for stack definitions
#include <stdlib.h> // Include standard library for memory allocation functions

// Define a structure for a node in the stack
typedef struct StackNode {
    Node *data; // Pointer to the data stored in the stack node
    struct StackNode *next; // Pointer to the next node in the stack
} StackNode;

// Define the structure for the stack itself
struct Stack {
    StackNode *top; // Pointer to the top node of the stack
};

// Function to create a new stack
Stack* create_stack() {
    Stack *s = (Stack*)malloc(sizeof(Stack)); // Allocate memory for the stack
    s->top = NULL; // Initialize the top pointer to NULL (empty stack)
    return s; // Return the created stack
}

// Function to free the memory used by the stack
void free_stack(Stack *s) {
    StackNode *current = s->top; // Start from the top of the stack
    while (current != NULL) { // Traverse through all nodes
        StackNode *temp = current; // Store the current node
        current = current->next; // Move to the next node
        free(temp); // Free the current node
    }
    free(s); // Free the stack structure itself
}

// Function to push a new node onto the stack
void push(Stack *s, Node *node) {
    StackNode *new_node = (StackNode*)malloc(sizeof(StackNode)); // Allocate memory for a new stack node
    new_node->data = node; // Set the data of the new node
    new_node->next = s->top; // Link the new node to the current top
    s->top = new_node; // Update the top pointer to the new node
}

// Function to pop the top node from the stack
Node* pop(Stack *s) {
    if (s->top == NULL) return NULL; // If the stack is empty, return NULL
    
    StackNode *temp = s->top; // Store the current top node
    Node *data = temp->data; // Retrieve the data from the top node
    s->top = temp->next; // Update the top pointer to the next node
    free(temp); // Free the memory of the popped node
    return data; // Return the data of the popped node
}

// Function to peek at the top node of the stack without removing it
Node* peek(Stack *s) {
    return (s->top != NULL) ? s->top->data : NULL; // Return the data of the top node, or NULL if the stack is empty
}

// Function to check if the stack is empty
int is_empty(Stack *s) {
    return s->top == NULL; // Return 1 (true) if the top pointer is NULL, otherwise 0 (false)
}