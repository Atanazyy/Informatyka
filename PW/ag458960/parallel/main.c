#include <pthread.h>
#include <stdlib.h>
#include <stdatomic.h>

#include "common/io.h"
#include "common/sumset.h"
#include "common/err.h"

#define BUFFER_SIZE 5000
#define MAX_DEPTH 2

typedef struct SharedPointer SharedPointer;

struct SharedPointer {
    Sumset sumset;
    SharedPointer* prev;
    atomic_int ref_count;
};

typedef struct Element {
    SharedPointer* a;
    SharedPointer* b;
    int depth;
} Element;

typedef struct SharedData {
    InputData input_data;
    Solution* best_solution;
    Element buffer[BUFFER_SIZE];
    pthread_mutex_t mutex;
    pthread_cond_t is_not_empty;
    int num_elements;
    int num_of_idle_threads;
    bool is_finished;
} SharedData;


// Initializes shared_data and reads the input data.
void shared_data_init(SharedData* shared_data) {
    input_data_read(&shared_data->input_data);
    shared_data->best_solution = (Solution*)malloc(sizeof(Solution));
    solution_init(shared_data->best_solution);
    pthread_mutex_init(&shared_data->mutex, NULL);
    pthread_cond_init(&shared_data->is_not_empty, NULL);
    shared_data->num_elements = 0;
    shared_data->num_of_idle_threads = 0;
    shared_data->is_finished = false;
}

// Frees the resources used by shared_data.
void shared_data_free(SharedData* shared_data) {
    pthread_mutex_destroy(&shared_data->mutex);
    pthread_cond_destroy(&shared_data->is_not_empty);
    free(shared_data->best_solution);
}

// Adds element to stack
void add_element(SharedData* shared_data, Element element) {
    pthread_mutex_lock(&shared_data->mutex);
    shared_data->buffer[shared_data->num_elements++] = element;
    pthread_cond_signal(&shared_data->is_not_empty);
    pthread_mutex_unlock(&shared_data->mutex);
}

// Gets element from stack
int get_element(SharedData* shared_data, Element* element) {
    pthread_mutex_lock(&shared_data->mutex);
    shared_data->num_of_idle_threads++;
    if(shared_data->num_of_idle_threads == shared_data->input_data.t && shared_data->num_elements == 0) {
        shared_data->is_finished = true;
        pthread_cond_broadcast(&shared_data->is_not_empty);
        pthread_mutex_unlock(&shared_data->mutex);
        return -1;
    }
    while (shared_data->num_elements == 0) {
        pthread_cond_wait(&shared_data->is_not_empty, &shared_data->mutex);
        if(shared_data->is_finished) {
            pthread_mutex_unlock(&shared_data->mutex);
            return -1;
        }
    }
    shared_data->num_of_idle_threads--;
    *element = shared_data->buffer[--shared_data->num_elements];
    pthread_mutex_unlock(&shared_data->mutex);
    return 0;
}

void decrement_ref_count(SharedPointer* a) {
    while (atomic_fetch_sub(&a->ref_count, 1) == 1) {
        SharedPointer* temp = a->prev;
        free(a);
        if (temp == NULL) {
            break;
        }
        a = temp;
    }
}

static void reference_solve(const Sumset* a, const Sumset* b, Solution* best_solution, SharedData* shared_data)
{
    if (a->sum > b->sum)
        return reference_solve(b, a, best_solution, shared_data);

    if (is_sumset_intersection_trivial(a, b)) { // s(a) ∩ s(b) = {0}.
        for (size_t i = a->last; i <= shared_data->input_data.d; ++i) {
            if (!does_sumset_contain(b, i)) {
                Sumset a_with_i;
                sumset_add(&a_with_i, a, i);
                reference_solve(&a_with_i, b, best_solution, shared_data);
            }
        }
    } else if ((a->sum == b->sum) && (get_sumset_intersection_size(a, b) == 2)) { // s(a) ∩ s(b) = {0, ∑b}.
        if (b->sum > best_solution->sum)
            solution_build(best_solution, &shared_data->input_data, a, b);
    }
}

static void solve(SharedPointer* a, SharedPointer* b, Solution* best_solution, SharedData* shared_data, int depth) {
    if (a->sumset.sum > b->sumset.sum) {
        SharedPointer* temp = a;
        a = b;
        b = temp;
    }
    if (is_sumset_intersection_trivial(&a->sumset, &b->sumset)) { // s(a) ∩ s(b) = {0}.
        for (size_t i = a->sumset.last; i <= shared_data->input_data.d; ++i) {
            if (!does_sumset_contain(&b->sumset, i)) {
                if(depth < MAX_DEPTH) {
                    SharedPointer* a_with_i = (SharedPointer*)malloc(sizeof(SharedPointer));
                    a_with_i->ref_count = 0;
                    sumset_add(&a_with_i->sumset, &a->sumset, i);
                    a_with_i->prev = a;
                    atomic_fetch_add(&a->ref_count, 1);
                    Element element;
                    element.a = a_with_i;
                    element.b = b;
                    element.depth = depth + 1;
                    atomic_fetch_add(&a_with_i->ref_count, 1);
                    atomic_fetch_add(&b->ref_count, 1);
                    add_element(shared_data, element);
                } else {
                    Sumset a_with_i_sumset;
                    sumset_add(&a_with_i_sumset, &a->sumset, i);
                    reference_solve(&a_with_i_sumset, &b->sumset, best_solution, shared_data);
                }
            }
        }
    } else if ((a->sumset.sum == b->sumset.sum) && (get_sumset_intersection_size(&a->sumset, &b->sumset) == 2)) { // s(a) ∩ s(b) = {0, ∑b}.
        if (b->sumset.sum > best_solution->sum)
            solution_build(best_solution, &shared_data->input_data, &a->sumset, &b->sumset);
    }
}

void* calculate(void* data) {
    struct SharedData* shared_data = (struct SharedData*)data;

    Solution* best_solution_local = (Solution*)malloc(sizeof(Solution));
    solution_init(best_solution_local);
    while(true) {
        Element element;
        if(get_element(shared_data, &element) == -1) {
            pthread_mutex_lock(&shared_data->mutex);
            if(best_solution_local->sum > shared_data->best_solution->sum) {
                free(shared_data->best_solution);
                shared_data->best_solution = best_solution_local;
            }
            else {
                free(best_solution_local);
            }
            pthread_mutex_unlock(&shared_data->mutex);
            return 0;
        }

        solve(element.a, element.b, best_solution_local, shared_data, element.depth);

        decrement_ref_count(element.a);
        decrement_ref_count(element.b);
    }
}

int main() {
    srand((unsigned)time(0));
    SharedData shared_data;
    shared_data_init(&shared_data);

    if(shared_data.input_data.t == 1) {
        reference_solve(&shared_data.input_data.a_start, &shared_data.input_data.b_start, shared_data.best_solution, &shared_data);
        solution_print(shared_data.best_solution);
        shared_data_free(&shared_data);
        return 0;
    }

    Element start;
    start.a = (SharedPointer*)malloc(sizeof(SharedPointer));
    start.b = (SharedPointer*)malloc(sizeof(SharedPointer));
    start.a->sumset = shared_data.input_data.a_start;
    start.b->sumset = shared_data.input_data.b_start;
    start.depth = 0;
    atomic_init(&start.a->ref_count, 1);
    atomic_init(&start.b->ref_count, 1);
    start.a->prev = NULL;
    start.b->prev = NULL;
    add_element(&shared_data, start);  

    pthread_t threads[shared_data.input_data.t];
    for (int i = 0; i < shared_data.input_data.t; i++) {
        ASSERT_ZERO(pthread_create(&threads[i], NULL, calculate, &shared_data));
    }

    for (int i = 0; i < shared_data.input_data.t; i++) {
        ASSERT_ZERO(pthread_join(threads[i], NULL));
    }

    solution_print(shared_data.best_solution);
    shared_data_free(&shared_data);
    return 0;
}