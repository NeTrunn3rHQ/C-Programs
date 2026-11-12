/*
 * Dynamic array of int values.
 *
 * Build: gcc -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -O2 dynamic_array.c -o dynamic_array
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntArray;

static int array_init(IntArray *array, size_t initial_capacity)
{
    if (array == NULL) return 1;
    array->data = NULL;
    array->size = 0U;
    array->capacity = 0U;

    if (initial_capacity == 0U) return 0;
    if (initial_capacity > SIZE_MAX / sizeof(*array->data)) return 1;

    array->data = malloc(initial_capacity * sizeof(*array->data));
    if (array->data == NULL) return 1;
    array->capacity = initial_capacity;
    return 0;
}

static int array_reserve(IntArray *array, size_t new_capacity)
{
    if (array == NULL || new_capacity < array->size) return 1;
    if (new_capacity == array->capacity) return 0;
    if (new_capacity > SIZE_MAX / sizeof(*array->data)) return 1;

    if (new_capacity == 0U) {
        free(array->data);
        array->data = NULL;
        array->capacity = 0U;
        return 0;
    }

    int *new_data = realloc(array->data, new_capacity * sizeof(*array->data));
    if (new_data == NULL) return 1;
    array->data = new_data;
    array->capacity = new_capacity;
    return 0;
}

static int array_push(IntArray *array, int value)
{
    if (array == NULL) return 1;
    if (array->size == array->capacity) {
        size_t next_capacity = array->capacity == 0U ? 4U : array->capacity * 2U;
        if (next_capacity < array->capacity ||
            next_capacity > SIZE_MAX / sizeof(*array->data)) return 1;
        if (array_reserve(array, next_capacity) != 0) return 1;
    }
    array->data[array->size++] = value;
    return 0;
}

static int array_remove_first(IntArray *array, int value)
{
    if (array == NULL) return 1;
    for (size_t i = 0U; i < array->size; ++i) {
        if (array->data[i] == value) {
            for (size_t j = i; j + 1U < array->size; ++j)
                array->data[j] = array->data[j + 1U];
            --array->size;
            return 0;
        }
    }
    return 1;
}

static void array_destroy(IntArray *array)
{
    if (array == NULL) return;
    free(array->data);
    array->data = NULL;
    array->size = 0U;
    array->capacity = 0U;
}

static void array_print(const IntArray *array)
{
    printf("[");
    for (size_t i = 0U; i < array->size; ++i)
        printf("%s%d", i == 0U ? "" : ", ", array->data[i]);
    puts("]");
}

int main(void)
{
    IntArray values;
    if (array_init(&values, 0U) != 0) return EXIT_FAILURE;

    for (int i = 1; i <= 5; ++i)
        if (array_push(&values, i * 10) != 0) {
            array_destroy(&values);
            return EXIT_FAILURE;
        }

    printf("Initial array: ");
    array_print(&values);

    (void)array_remove_first(&values, 30);
    (void)array_push(&values, 60);

    printf("After removal and append: ");
    array_print(&values);
    printf("size=%zu, capacity=%zu\n", values.size, values.capacity);

    array_destroy(&values);
    return EXIT_SUCCESS;
}
