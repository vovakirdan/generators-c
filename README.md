# Generator Library

This project implements a flexible generator system in `C`, which allows for the creation of both infinite and non-infinite generators. These generators can produce sequences of numbers based on various logic patterns, such as counting, cycling through an array, repeating a value, or accumulating sums.
##  Features

- `Count Generator`: Produces an infinite sequence starting from a specified initial value.
- `Cycle Generator`: Repeatedly cycles through a given array infinitely.
- `Repeat Generator`: Repeats a specified value a given number of times.
- `Accumulate Generator`: Produces a sequence of cumulative sums from an input array.
# Getting Started

## Prerequisites

To compile and run this project, you will need:

- A `C` compiler (e.g., `GCC`)

## Compilation

To compile the project, use the following command:

```bash
gcc -o generator itertools.c
```

## Usage

1. Include the Header

   Ensure to include necessary headers in your source file:

   #include <stdio.h>
   #include <stdlib.h>
   #include <limits.h>
   #include <stdbool.h>


2. Create Generators

   You can create different types of generators using the provided functions:

    - Count Generator
```C
Generator* countGen = count(5);
```


- Cycle Generator
```C
int arr[] = {1, 2, 3, 4};
Generator* cycleGen = cycle(arr, 4);
```

- Repeat Generator
```C
Generator* repeatGen = repeat(10, 3);
```
- Accumulate Generator
```C
int arr[] = {1, 2, 3, 4, 5};
Generator* accumulateGen = accumulate(arr, 5);
```

3. Using Generators

   Retrieve values from the generator using the next function:
```C
int value;
while ((value = next(countGen)) != -1) {
printf("%d ", value);
}
```

4. Freeing Memory

   Once you are done with a generator, free its memory:
```C
free_generator(countGen);
```
## Functionality Overview

- Infinite Generators:
    - `count(int init_value)`: Starts from init_value and counts upwards indefinitely.
    - `cycle(int* array, size_t size)`: Cycles through the elements of array indefinitely.

- Non-Infinite Generators:
    - `repeat(int value, size_t count)`: Repeats value for count times.
    - `accumulate(int* array, size_t size)`: Returns cumulative sums of elements in array.
## Notes

- The function free_generator is used to release memory allocated for a generator. Ensure to call this function to prevent memory leaks.
- Each generator type has a specific logic encapsulated in its next_<generator_name>_function.

## Contributing

Contributions are welcome! Please fork this repository and submit pull requests for any enhancements or bug fixes.
