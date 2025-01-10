#include "stack.h"

void initStack(Stack *s, int capacity) {
    s->items = (Element *)malloc(capacity * sizeof(Element));
    s->top = -1;
    s->capacity = capacity;
}

bool isFull(Stack *s) {
    return s->top == (s->capacity - 1);
}

bool isEmpty(Stack *s) {
    return s->top == -1;
}

void resize(Stack *s) {
    s->capacity *= 2;
    s->items = (Element *)realloc(s->items, s->capacity * sizeof(Element));
}

void push(Stack *s, Element item) {
    if (isFull(s)) {
        resize(s);
    }
    s->items[++(s->top)] = item;
}

Element pop(Stack *s) {
    assert(!isEmpty(s));
    return s->items[(s->top)--];
}

Element peek(Stack *s) {
    assert(!isEmpty(s));
    return s->items[s->top];
}

void freeStack(Stack *s) {
    free(s->items);
}