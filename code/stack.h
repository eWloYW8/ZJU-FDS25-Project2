#include <stdlib.h>
#include "./ast.h"

typedef struct Stack {
    Node **data;
    int top;
    int capacity;
} Stack;

Stack* create_stack(int capacity);

void push(Stack *s, Node *node);

Node* pop(Stack *s);

Node* peek(Stack *s);

int is_empty(Stack *s);