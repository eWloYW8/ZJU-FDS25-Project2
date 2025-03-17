#include "stack.h"
#include <stdlib.h>

Stack* create_stack(int capacity) {
    Stack *s = (Stack*)malloc(sizeof(Stack));
    s->data = (Node**)malloc(sizeof(Node*) * capacity);
    s->top = -1;
    s->capacity = capacity;
    return s;
}

void free_stack(Stack *s) {
    free(s->data);
    free(s);
}

void push(Stack *s, Node *node) {
    if (s->top < s->capacity -1) {
        s->data[++s->top] = node;
    }
}

Node* pop(Stack *s) {
    if (s->top >= 0) {
        return s->data[s->top--];
    }
    return NULL;
}

Node* peek(Stack *s) {
    if (s->top >=0) {
        return s->data[s->top];
    }
    return NULL;
}

int is_empty(Stack *s) {
    return s->top == -1;
}