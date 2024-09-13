#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// Define the generator logic
typedef struct Generator {
    int current_value;
    size_t limit;
    bool done;
    size_t i;
    int* int_array;
    // function pointer to the custom next logic
    int (*next_function)(struct Generator*);
} Generator;

int next(Generator *gen) {
    if (gen->done) return -1;
    return gen->next_function(gen);
}

void free_generator(Generator* gen) {free(gen);}  // todo check if it possible

/*Any generator should have
next_<generator_name>function - that provides custom next element
Generator* <generator_name> {}*/

// ----------Infinite generators----------

// Count

int next_count_function(Generator* counter) {
    int value = counter->current_value;
    counter->current_value++;
    if (counter->current_value >= counter->limit) {counter->done = 1;}
    return value;
}

// count(5) -> 5 6 7 8 9 ...
Generator* count(int init_value) {
    Generator* gen = (Generator*)malloc(sizeof(Generator));
    if (gen != NULL) {
        gen->done = false;
        gen->current_value = init_value;
        gen->limit = INT_MAX;
        gen->next_function = next_count_function;
    } else {
        printf("Failed to allocate memory for Generator\n");
        exit(EXIT_FAILURE);
    }
    return gen;
}

// Cycle
// Cycle gets an array and repeat it infinity times

int next_cycle_function(Generator* cycler) {
    int* arr = cycler->int_array;
    if (cycler->i == cycler->limit) {cycler->i = 0;}
    int value = arr[cycler->i];
    cycler->i++;
    return value;
}

// cycle({1, 2, 3, 4}, 4) -> 1 2 3 4 1 2 3 4 1 2 ...
Generator* cycle(int* array, size_t size) {
    Generator* gen = (Generator*)malloc(sizeof(Generator));
    if (gen != NULL) {
        gen->current_value = array[0];
        gen->done = false;
        gen->i = 0;
        gen->int_array = array;
        gen->limit = size;
        gen->next_function = next_cycle_function;
    } else {
        printf("Failed to allocate memory for Generator\n");
        exit(EXIT_FAILURE);
    }
}

// Repeat
// Repeat repeats a passed element n times

int next_repeat_function(Generator* repeater) {
    int value;
    if (repeater->i < repeater->limit) {
        value = repeater->current_value;
    } else {
        repeater->done = true;
        return -1;
    }
    return value;
}

// repeat(10, 3) -> 10 10 10
Generator *repeat(int value, size_t count) {
    Generator *gen = (Generator *) malloc(sizeof(Generator));
    if (gen != NULL) {
        gen->current_value = value;
        gen->done = false;
        gen->i = 0;
        gen->limit = count;
        gen->next_function = next_repeat_function;
    } else {
        printf("Failed to allocate memory for Generator\n");
        exit(EXIT_FAILURE);
    }
    return gen;
}

// ----------Non-Infinite generators----------

// Accumulate
// Accumulate return contigous sum of each element in given array

int next_accumulate_function(Generator* accumulator) {
    int* arr = (int*)accumulator->int_array;
    int index = accumulator->i;
    if (index == 0) {
        accumulator->current_value = arr[index];
    } else {
        accumulator->current_value += arr[index];
    }
    if (accumulator->limit == accumulator->i + 1) {accumulator->done = true;}
    accumulator->i++;
    return accumulator->current_value;
}

// accumulate({1, 2, 3, 4, 5}, 5) -> 1 3 6 10 15
Generator* accumulate(int* array, size_t size) {
    Generator *gen = (Generator *) malloc(sizeof(Generator));
    if (gen != NULL) {
        gen->current_value = 0;
        gen->done = false;
        gen->i = 0;
        gen->int_array = array;
        gen->limit = size;
        gen->next_function = next_accumulate_function;
    } else {
        printf("Failed to allocate memory for Generator\n");
        exit(EXIT_FAILURE);
    }
    return gen;
}

int main() {
    int n = 5;
    int arr[] = {1, 2, 3, 4, 5};
    Generator* counter = count(n);
    printf("Counter generator example\nStarts from %i: ", n);
    for (int i = 0; i < 10; i++) {
        printf("%i ", next(counter));
    }
    printf("\n");

    Generator *cycler = cycle(arr, sizeof(arr) / sizeof(arr[0]));
    printf("Cycler generator example\nCycle through array: ");
    for (int i = 0; i < 15; i++) {
        printf("%i ", next(cycler));
    }
    printf("\n");

    Generator *repeater = repeat(10, 3);
    printf("Repeater generator example\nRepeat value 10, 3 times: ");
    for (int i = 0; i < 5; i++) {
        printf("%i ", next(repeater));
    }
    printf("\n");

    Generator *accumulator = accumulate(arr, sizeof(arr) / sizeof(arr[0]));
    printf("Accumulator generator example\nAccumulate array: ");
    for (int i = 0; i < 5; i++) {
        printf("%i ", next(accumulator));
    }
    printf("\n");

    free_generator(counter);
    free_generator(cycler);
    free_generator(repeater);
    free_generator(accumulator);
}
