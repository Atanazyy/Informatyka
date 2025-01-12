#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "common/io.h"
#include "common/sumset.h"

static InputData input_data;
static Solution best_solution;

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

void initStack(Stack *s, int capacity) {
    s->items = (Element *)malloc(capacity * sizeof(Element));
    s->top = -1;
    s->capacity = capacity;
}

bool isEmpty(Stack *s) {
    return s->top == -1;
}

void push(Stack *s, Element item) {
    s->items[++(s->top)] = item;
}

Element pop(Stack *s) {
    assert(!isEmpty(s));
    return s->items[(s->top)--];
}

void freeStack(Stack *s) {
    free(s->items);
}

int main()
{
    input_data_read(&input_data);
    solution_init(&best_solution);
    Sumset Pool[5000];
    int pointer = 0;
    
    Stack stack;
    initStack(&stack, 5000);
    Element start;
    start.a = &input_data.a_start;
    start.b = &input_data.b_start;
    start.operation = 0;
    push(&stack, start);

    while (!isEmpty(&stack)) {
        Element current = pop(&stack);
        if (current.operation == 0) {
            if (current.a->sum > current.b->sum) {
                Sumset* temp = current.a;
                current.a = current.b;
                current.b = temp;
            }
            if (is_sumset_intersection_trivial(current.a, current.b)) {
                for (size_t i = current.a->last; i <= input_data.d; ++i) {
                    if (!does_sumset_contain(current.b, i)) {
                        Element next;
                        next.a = &Pool[pointer++];
                        next.b = current.b;
                        next.operation = 0;
                        sumset_add(next.a, current.a, i);
                        Element free_element;
                        free_element.a = next.a;
                        free_element.operation = 1;
                        push(&stack, free_element);
                        push(&stack, next);
                    }
                }
                continue;
            } else if ((current.a->sum == current.b->sum) && (get_sumset_intersection_size(current.a, current.b) == 2)) {
                if (current.b->sum > best_solution.sum) {
                    solution_build(&best_solution, &input_data, current.a, current.b);
                }
            }
        }
        else {
            pointer--;
        }
    }

    freeStack(&stack);
    solution_print(&best_solution);
    return 0;
}
