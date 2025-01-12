#include <stddef.h>

#include "common/io.h"
#include "common/sumset.h"
#include "stack.h"

static InputData input_data;

static Solution best_solution;

int main()
{
    input_data_read(&input_data);
    //input_data_init(&input_data, 8, 25, (int[]){0}, (int[]){1, 0});

    solution_init(&best_solution);
    
    Stack stack;
    initStack(&stack, 1000);
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
                push(&stack, current);
            } else if (is_sumset_intersection_trivial(current.a, current.b)) {
                for (size_t i = current.a->last; i <= input_data.d; ++i) {
                    if (!does_sumset_contain(current.b, i)) {
                        Element next;
                        next.a = (Sumset *)malloc(sizeof(Sumset));
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
            free(current.a);
        }
    }

    freeStack(&stack);
    solution_print(&best_solution);
    return 0;
}
