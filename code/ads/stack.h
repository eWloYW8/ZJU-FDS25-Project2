// This file contains the declaration of the Stack data structure and its functions.
// The Stack data structure is a stack of Node pointers.
#pragma once

#include "../ast.h"

typedef struct Stack {
    Node **data;
    int top;
    int capacity;
} Stack;

Stack* create_stack(int capacity);

void free_stack(Stack *s);

void push(Stack *s, Node *node);

Node* pop(Stack *s);

Node* peek(Stack *s);

int is_empty(Stack *s);