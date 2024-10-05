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
    DOUBLE_TYPE,
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
            printf("Iterable[");
            for (size_t i = 0; i < iterable.length; i++) {
                printf("%d", *(int*)iterable.get_element(iterable.array, i));
                if (i != iterable.length - 1) {
                    printf(", ");
                }
            }
            printf("] int(%llu\n)", iterable.length);
            break;
        case FLOAT_TYPE:
            printf("Iterable[");
            for (size_t i = 0; i < iterable.length; i++) {
                printf("%f", *(float*)iterable.get_element(iterable.array, i));
                if (i != iterable.length - 1) {
                    printf(", ");
                }
            }
            printf("] float(%llu\n)", iterable.length);
            break;
        case DOUBLE_TYPE:
            printf("Iterable[");
            for (size_t i = 0; i < iterable.length; i++) {
                printf("%lf", *(double*)iterable.get_element(iterable.array, i));
                if (i != iterable.length - 1) {
                    printf(", ");
                }
            }
            printf("] double(%llu\n)", iterable.length);
            break;
        case CHAR_TYPE:
            printf("Iterable[");
            for (size_t i = 0; i < iterable.length; i++) {
                printf("%c", *(char*)iterable.get_element(iterable.array, i));
                if (i != iterable.length - 1) {
                    printf(", ");
                }
            }
            printf("] char(%llu\n)", iterable.length);
            break;
        default: 
            printf("Iterable[] any(%llu\n)", iterable.length);
            break;
    }
}

// ----------iterable getters----------

Iterable get_int_iterable(int* array, size_t size) {
    int* temp_array = malloc(size * sizeof(int));
    if (temp_array != NULL) {
        temp_array = memcpy(temp_array, array, size * sizeof(int));
    } else {
        fprintf(stderr, "Failed to allocate memory for iterable array\n");
        exit(EXIT_FAILURE);
    }
    Iterable iterable = {
        .array = temp_array,
        .element_size = sizeof(int),
        .length = size,
        .get_element = get_int_element,
        .data_type = INT_TYPE
    };
    return iterable;
}

Iterable get_float_iterable(float* array, size_t size) {
    float* temp_array = malloc(size * sizeof(float));
    if (temp_array != NULL) {
        temp_array = memcpy(temp_array, array, size * sizeof(float));
    } else {
        fprintf(stderr, "Failed to allocate memory for iterable array\n");
        exit(EXIT_FAILURE);
    }
    Iterable iterable = {
        .array = temp_array,
        .element_size = sizeof(float),
        .length = size,
        .get_element = get_float_element
    ,   .data_type = FLOAT_TYPE
    };
    return iterable;
}

Iterable get_double_iterable(double* array, size_t size) {
    double* temp_array = malloc(size * sizeof(double));
    if (temp_array != NULL) {
        temp_array = memcpy(temp_array, array, size * sizeof(double));
    } else {
        fprintf(stderr, "Failed to allocate memory for iterable array\n");
        exit(EXIT_FAILURE);
    }
    Iterable iterable = {
        .array = temp_array,
        .element_size = sizeof(double),
        .length = size,
        .get_element = get_double_element,
        .data_type = DOUBLE_TYPE
    };
    return iterable;
}

Iterable get_char_iterable(char* array) {
    size_t size = 1;
    for (char* ch = array; *ch != '\0'; ch++, size++);
    char* temp_array = malloc(size * sizeof(char));
    if (temp_array != NULL) {
        temp_array = memcpy(temp_array, array, size * sizeof(char));
    } else {
        fprintf(stderr, "Failed to allocate memory for iterable array\n");
        exit(EXIT_FAILURE);
    }
    Iterable iterable = {
        .array = temp_array,
        .element_size = sizeof(char),
        .length = size,
        .get_element = get_char_element,
        .data_type = CHAR_TYPE
    };
    return iterable;
}

Iterable get_T_iterable(void* array, size_t element_size, size_t length, void*(*function_get_element)(void*, size_t)) {
    void* temp_array = malloc(length * element_size);
    if (temp_array != NULL) {
        temp_array = memcpy(temp_array, array, length * element_size);
    } else {
        fprintf(stderr, "Failed to allocate memory for iterable array\n");
        exit(EXIT_FAILURE);
    }
    Iterable iterable = {
        .array = temp_array,
        .element_size = element_size,
        .length = length,
        .get_element = function_get_element,
        .data_type = ANY_TYPE
    };
    return iterable;
}

// Do not use with dynamically created arrays! Use get_T_iterable instead!
#define GET_ITERABLE(array, get_element_function) get_T_iterable(array, sizeof(array[0]), sizeof(array) / sizeof(array[0]), get_element_function)

// combine all passed Iterables untill faced with zero length iterable
Iterable combine_iterables(Iterable iterable, ...) {
    va_list args;
    va_start(args, iterable);
    // start with first iterable
    Iterable current_iterable = iterable;
    size_t total_length = 1;
    // combine with all other iterables
    while (current_iterable.length != 0) {
        total_length *= current_iterable.length;
        current_iterable = va_arg(args, Iterable);
    }
    // allocate memory for the new combined array
    void* combined_array = malloc(total_length * current_iterable.element_size);
    if (combined_array != NULL) {
        combined_array = memcpy(combined_array, current_iterable.array, total_length * current_iterable.element_size);
    } else {
        fprintf(stderr, "Failed to allocate memory for combined array\n");
        exit(EXIT_FAILURE);
    }
    // copy contents
    va_start(args, iterable);
    current_iterable = iterable;
    if (current_iterable.data_type == CHAR_TYPE) {
        while (current_iterable.length != 0) {
            strcat((char*)combined_array, (char*)current_iterable.array);
            current_iterable = va_arg(args, Iterable);
        }
    } else {
        size_t offset = 0;
        while (current_iterable.length != 0) {
            for (size_t i = 0; i < current_iterable.length; i++) {
                memcpy((char*)combined_array + offset * iterable.element_size, current_iterable.array, current_iterable.length * iterable.element_size);
                offset += current_iterable.length;
                current_iterable = va_arg(args, Iterable);
            }
        }
    }
    va_end(args);
}
// ----------Iterable functions----------

// return a sliced iterable. Equivalent of it[n:m] in python
Iterable slice(Iterable iterable, int from, int till) {
    size_t length = iterable.length;
    int start, end;
    start = from;
    end = till;
    start = start < 0 ? length + start : start;
    end = end < 0 ? length + end : end;
    if (start < 0) start = 0;
    if (end > length) end = length;
    if (start > end) start = end;
    Iterable sliced = {
        .element_size = iterable.element_size,
        .length = end - start,
        .array = get_element_slice(iterable.array, start, end, iterable.element_size),
        .get_element = iterable.get_element,
        .data_type = iterable.data_type,
    };
    return sliced;
}

// returns an element from an iterable
void* get_element(Iterable iterable, long int i) {
    size_t i_ = i < 0 ? i + iterable.length : i;
    return iterable.get_element(iterable.array, i_);
}

// ----------Infinite Iterators----------

void* next_count_function(Generator* gen) {
    int* value = (int*)malloc(sizeof(int));
    if (value != NULL) {
        *value = gen->iterable.length;
        gen->iterable.length++;
    } else {
        fprintf(stderr, "Failed to allocate memory for count value\n");
        exit(EXIT_FAILURE);
    }
    return value;
}

// count(5) -> 5 6 7 8 ...
Generator* count(int start) {
    Generator* gen = gmalloc();
    if (gen != NULL) {
        gen->done = false;
        gen->index = 0;
        gen->iterable.array = NULL;
        gen->iterable.element_size = sizeof(int);
        gen->iterable.length = start;
        gen->next_function = next_count_function;
    } else {
        fprintf(stderr, "Failed to allocate memory for count iterator\n");
        exit(EXIT_FAILURE);
    }
    return gen;
}

void* next_range_function(Generator* gen) {
    int* value = (int*)malloc(sizeof(int));
    if (value != NULL) {
        *value = gen->index;
        gen->index++;
    } else {
        fprintf(stderr, "Failed to allocate memory for range value\n");
        exit(EXIT_FAILURE);
    }
    return value;
}

// range(5) -> 0 1 2 3 4
Generator* range(int start) {
    Generator* gen = gmalloc();
    if (gen != NULL) {
        gen->done = false;
        gen->index = start;
        gen->iterable.array = NULL;
        gen->iterable.element_size = sizeof(int);
        gen->next_function = next_range_function;
    } else {
        fprintf(stderr, "Failed to allocate memory for range iterator\n");
        exit(EXIT_FAILURE);
    }
    return gen;
}

void* next_cycle_function(Generator* gen) {
    if (gen->iterable.length == 0) {
        gen->done = true;
        return NULL;
    }
    if (gen->index >= gen->iterable.length) {
        gen->index = 0;
    }
    void* element = gen->iterable.get_element(gen->iterable.array, gen->index);
    gen->index++;
    return element;
}

// cycle(iterable(1, 2, 3, 4)) -> 1 2 3 4 1 2 3 ...
Generator* cycle(Iterable iterable) {
    Generator* gen = gmalloc();
    if (gen != NULL) {
        gen->done = false;
        gen->index = 0;
        gen->iterable = iterable;
        gen->next_function = next_cycle_function;
    } else {
        fprintf(stderr, "Failed to allocate memory for cycle iterator\n");
        exit(EXIT_FAILURE);
    }
    return gen;
}

void* next_repeat_function(Generator* gen) {
    if (gen->iterable.length != 0 && gen->index >= gen->iterable.length) {
        gen->done = true;
        return NULL;
    }
    void* element = gen->iterable.get_element(gen->iterable.array, gen->index);
    gen->index++;
    return element;
}

// repeat passed value n or endless times
Generator* repeat(void* element, size_t element_size, ...) {
    va_list args;
    va_start(args, element_size);
    Generator* gen = gmalloc();
    if (gen != NULL) {
        gen->done = false;
        gen->index = 0;
        gen->iterable.array = malloc(element_size);
        if (gen->iterable.array != NULL) {
            memcpy(gen->iterable.array, element, element_size);
        }
        gen->iterable.element_size = element_size;
        size_t n = va_arg(args, size_t);
        if (n > 0) {
            gen->iterable.length = n;
        } else {
            gen->iterable.length = 0;
        }
        gen->next_function = next_repeat_function;
    } else {
        fprintf(stderr, "Failed to allocate memory for repeat iterator\n");
        exit(EXIT_FAILURE);
    }
    va_end(args);
    return gen;
}

// ----------Non-Infinite Iterators----------

void* next_accumulate_function(Generator* gen) {
    if (gen->index >= gen->iterable.length) {
        gen->done = true;
        return NULL;
    }
    void* current_element = gen->iterable.get_element(gen->iterable.array, gen->index);
    void* accumulated_value = malloc(sizeof(int));
    if (accumulated_value == NULL) {
        fprintf(stderr, "Failed to allocate memory for accumulate iterator\n");
        exit(EXIT_FAILURE);
    }
    if (gen->index == 0) {
        memcpy(accumulated_value, current_element, sizeof(int));   
    } else {
        if (gen->iterable.data_type == INT_TYPE) {
            *(int*)accumulated_value = *(int*)gen->iterable.array + *(int*)current_element;
            *(int*)gen->iterable.array = *(int*)accumulated_value;
        } else if (gen->iterable.data_type == FLOAT_TYPE) {
            *(float*)accumulated_value = *(float*)gen->iterable.array + *(float*)current_element;
            *(float*)gen->iterable.array = *(float*)accumulated_value;
        } else if (gen->iterable.data_type == DOUBLE_TYPE) {
            *(double*)accumulated_value = *(double*)gen->iterable.array + *(double*)current_element;
        } else {
            fprintf(stderr, "Invalid data type for accumulate iterator\n");
            exit(EXIT_FAILURE);            
        }
    }
    gen->index++;
    return accumulated_value;
}

// generate cumulative sum
Generator* accumulate(Iterable iterable) {
    Generator* gen = gmalloc();
    gen->done = false;
    gen->index = 0;
    gen->iterable = iterable;
    gen->next_function = next_accumulate_function;
    return gen;
}

// todo write it
void* next_batched_function(Generator* gen);

// batched("ABCDEFG", 3) -> ABC DEF G
Generator* batched(Iterable iterable, int batch_size) {
    Generator* gen = gmalloc();
    gen->done = false;
    gen->index = 0;
    gen->iterable = iterable;
    gen->next_function = next_batched_function;
    return gen;
}

void* next_chain_function(Generator* gen) {
    if (gen->index >= gen->iterable.length) {
        gen->done = true;
        return NULL;
    }
    void* current_element = gen->iterable.get_element(gen->iterable.array, gen->index);
    gen->index++;
    return current_element;
}

// chain is a cycle but generate finite chain
Generator* chain(Iterable iterable, ...) {
    va_list args;
    va_start(args, iterable);
    // start with the first iterable
    Iterable current_iterable = iterable;
    size_t total_length = 1;
    while (current_iterable.length > 0) {
        total_length += current_iterable.length;
        current_iterable = va_arg(args, Iterable);
    }
    // allocate memory for the new combined array
    void* combined_array = malloc(total_length * iterable.element_size);
    if (combined_array == NULL) {
        fprintf(stderr, "Failed to allocate memory for chain\n");
        exit(EXIT_FAILURE);
    }
    // copy contents
    va_start(args, iterable);
    current_iterable = iterable;
    if (current_iterable.data_type == CHAR_TYPE) {
        while (current_iterable.length > 0) {
            strcat((char*)combined_array, (char*)current_iterable.array);
            current_iterable = va_arg(args, Iterable);
        }
    } else {
        size_t offset = 0;
        while (current_iterable.length > 0) {
            memcpy((char*)combined_array + offset, current_iterable.array, current_iterable.element_size * current_iterable.length);
            offset += current_iterable.length;
            current_iterable = va_arg(args, Iterable);
        }
    }
    va_end(args);
    Iterable combined_iterable = {
        .array = combined_array,
        .element_size = iterable.element_size,
        .length = total_length,
        .get_element = iterable.get_element
    };
    Generator* gen = gmalloc();
    gen->done = false;
    gen->index = 0;
    gen->iterable = combined_iterable;
    gen->next_function = next_chain_function;
    return gen;
}

void* next_product_function(Generator* gen) {
    if (gen->done) {return NULL;}
    void* product_array = malloc(gen->repeat_size * gen->iterable.element_size);
    if (product_array == NULL) {
        fprintf(stderr, "Failed to allocate memory for product\n");
        exit(EXIT_FAILURE);
    }
    // fill product_array with the current element
    for (size_t i = 0; i < gen->repeat_size; i++) {
        void* current_element = gen->iterable.get_element(gen->iterable.array, gen->repeat_values[i]);
        memcpy((char*)product_array + i * gen->iterable.element_size, current_element, gen->iterable.element_size);
    }
    // update the indices
    for (int i = gen->repeat_size - 1; i >= 0; i--) {
        if (++gen->repeat_values[i] == gen->iterable.length) {break;}
        gen->repeat_values[i] = 0;
        if (i == 0) {gen->done = true;}
    }
    return product_array;
}

// product
Generator* product(Iterable iterable, size_t repeat) {
    Generator* gen = gmalloc();
    gen->done = false;
    gen->index = 0;
    gen->iterable = iterable;
    gen->repeat_size = repeat;
    gen->repeat_values = (size_t*)calloc(repeat, sizeof(size_t));
    gen->next_function = next_product_function;
    return gen;
}

int main() {
    // some examples of usage
    Iterable char_it1 = get_char_iterable("ABC");
    Iterable char_it2 = get_char_iterable("DEF");
    Generator* chain_gen = chain(char_it1, char_it2, END_CHAIN);
    char* chain_element;
    printf("Chain example with while:\n");
    while ((chain_element = (char*)next(chain_gen)) != NULL) {
        printf("%c\n", chain_element);
    }
    printf("\n");

    int start_count = 5;
    Generator* counter = count(start_count);
    printf("Counter example:\n");
    for (int i = 0; i < 10; i++) {
        int* value = (int*)next(counter);
        printf("%d ", *value);
        free(value);  // don't forget to free dynamically allocated memory
    }
    printf("\n");

    int int_arr[] = {1, 2, 3, 4, 5};
    Iterable int_iterable = GET_ITERABLE(int_arr, get_int_element);
    Generator* int_cycle = cycle(int_iterable);
    printf("Cycle throught array:\n");
    for (int i = 0; i < 10; i++) {
        int* value = (int*)next(int_cycle);
        printf("%d ", *value);
        free(value);  // don't forget to free dynamically allocated memory
    }
    printf("\n");

    int repeat_value = 42;
    //infinite repetition (with only element and size)
    Generator* infinite_repeater = repeat(&repeat_value, sizeof(int), false);

    // limited repetition
    Generator* limited_repeater = repeat(&repeat_value, sizeof(int), 10);
    printf("Infinite repetition:\n");
    for (int i = 0; i < 10; i++) {
        int* value = (int*)next(infinite_repeater);
        printf("%d ", *value);
    }
    printf("\n");
}
