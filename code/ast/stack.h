// This file contains the declaration of the Stack data structure and its functions.
// The Stack data structure is a stack of Node pointers.
#pragma once

#include "ast.h"

// 前置声明Stack结构体
typedef struct Stack Stack;

// Function to create a new stack (capacity parameter remains for compatibility)
Stack* create_stack();

// Function to free the memory allocated for the stack
void free_stack(Stack *s);

// Function to push a new node onto the stack
void push(Stack *s, Node *node);

// Function to pop the top node from the stack
Node* pop(Stack *s);

// Function to peek at the top node of the stack
Node* peek(Stack *s);

// Function to check if the stack is empty
int is_empty(Stack *s);