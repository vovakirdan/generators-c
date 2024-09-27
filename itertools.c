#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

// struncture for generators that use arifmetic on types
typedef enum {
    ANY_TYPE,
    INT_TYPE,
    FLOAT_TYPE,
    DOUBLR_TYPE,
    CHAR_TYPE
} DataType;

// Generic Iterable structure
typedef struct {
    // pointer to the array of any type
    void *array;
    // size of each element in the array
    size_t element_size;
    // NUmber of elements in the array
    size_t length;
    // function to get element by index
    void* (*get_element)(void* array, size_t index);
    // tells what type it is
    DataType data_type;
} Iterable;

// Generator structure
typedef struct Generator {
    size_t index;
    Iterable iterable;
    bool done;
    //for iterators
    size_t* repeat_values;
    size_t repeat_size;
    void* (*next_function)(struct Generator*);
} Generator;

// init a Generator
Generator* gmalloc() {
    Generator* gen = (Generator*) malloc(sizeof(Generator));
    if (gen == NULL) {
        fprintf(stderr, "Failed to allocate memory for Generator\n");
        exit(EXIT_FAILURE);
    }
    return gen;
}

// free a Generator
void gfree(Generator* gen) {
    if (gen != NULL) {
        free(gen);
        fprintf(stderr, "Freed Generator");
    }
}

// global `next` function for extracting values. Do not forget to cast to the type you excpected
void* next(Generator* gen) {
    return gen->next_function(gen);
}

#define END_CHAIN ((Iterable){.length = 0, .array = NULL, .data_type = ANY_TYPE})

// ----------ELEMENT GETTERS----------

// function to get an element from array of ints
void* get_int_element(void* array, size_t index) {
    return (int*)array + index;
}

// function to get an element from array of floats
void* get_float_element(void* array, size_t index) {
    return (float*)array + index;
}

// function to get an element from array of doubles
void* get_double_element(void* array, size_t index) {
    return (double*)array + index;
}

// function to get an element from array of chars
void* get_char_element(void* array, size_t index) {
    return (char*)array + index;
}

// todo void free generator

// print iterable
void print_iterable(Iterable iterable) {
    switch (iterable.data_type) 
    {
        case INT_TYPE:
            printf("[");
            for (size_t i = 0; i < iterable.length; i++) {
                printf("%d", *(int*)iterable.get_element(iterable.array, i));
                if (i != iterable.length - 1) {
                    printf(", ");
                }
            }
            printf("]");
            break;
        case FLOAT_TYPE:
            printf("[");
            for (size_t i = 0; i < iterable.length; i++) {
                printf("%f", *(float*)iterable.get_element(iterable.array, i));
                if (i != iterable.length - 1) {
                    printf(", ");
                }
            }
            printf("]");
            break;
    }
}