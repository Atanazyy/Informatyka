#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "common/io.h"

typedef struct Element {
    Sumset* a;
    Sumset* b;
    int operation;
} Element;

typedef struct Stack {
    Element *items;
    int top;
    int capacity;
} Stack;

void initStack(Stack *s, int capacity);
bool isFull(Stack *s);
bool isEmpty(Stack *s);
void resize(Stack *s);
void push(Stack *s, Element item);
Element pop(Stack *s);
Element peek(Stack *s);
void freeStack(Stack *s);

#endif // STACK_H