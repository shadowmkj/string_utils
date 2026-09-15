#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// ==============================================================================
// Configuration & Data Structures
// ==============================================================================

// Default initial capacity when pushing to an unallocated (NULL) array
#define DA_DEFAULT_CAPACITY 8

// Header containing metadata stored immediately before the user array buffer
typedef struct {
    size_t count;
    size_t capacity;
} Da_Header;

// ==============================================================================
// Public Macro API
// ==============================================================================

// Internal helper to get pointer to the header block
#define da_header(a) ((Da_Header *)(a) - 1)

// Returns current number of elements in the array (0 if NULL)
#define da_count(a) ((a) ? da_header(a)->count : 0)

// Returns current capacity of the array (0 if NULL)
#define da_capacity(a) ((a) ? da_header(a)->capacity : 0)

// Returns true if array is empty (or NULL)
#define da_empty(a) (da_count(a) == 0)

// Appends an element to the dynamic array, growing capacity if needed
#define da_push(a, val)                                                        \
    (da_grow(&(a), sizeof(*(a))), (a)[da_header(a)->count++] = (val))

// Pops and returns the last element
#define da_pop(a) ((a)[--da_header(a)->count])

// Returns an lvalue reference to the last element
#define da_last(a) ((a)[da_header(a)->count - 1])

// Pre-allocates memory for at least `n` elements
#define da_reserve(a, n) da_reserve_internal(&(a), (n), sizeof(*(a)))

// Clears all elements by resetting count to 0, retaining allocated capacity
#define da_clear(a)                                                            \
    do {                                                                       \
        if ((a) != NULL) {                                                     \
            da_header(a)->count = 0;                                           \
        }                                                                      \
    } while (0)

// Frees the allocated memory block (header + data) and sets array pointer to
// NULL
#define da_free(a)                                                             \
    do {                                                                       \
        if ((a) != NULL) {                                                     \
            free(da_header(a));                                                \
            (a) = NULL;                                                        \
        }                                                                      \
    } while (0)

// ==============================================================================
// Internal Growth & Reservation Helpers
// ==============================================================================

void *da_grow_internal(void *arr, size_t item_size);
void *da_reserve_internal_impl(void *arr, size_t needed_capacity,
                               size_t item_size);

#define da_grow(arr_ptr, item_size)                                            \
    (*(void **)(arr_ptr) = da_grow_internal(*(arr_ptr), (item_size)))

#define da_reserve_internal(arr_ptr, needed_capacity, item_size)               \
    (*(void **)(arr_ptr) =                                                     \
         da_reserve_internal_impl(*(arr_ptr), (needed_capacity), (item_size)))

#endif // !DYNAMIC_ARRAY_H

#ifdef DYNAMIC_ARRAY_IMPLEMENTATION
#undef DYNAMIC_ARRAY_IMPLEMENTATION

#include <stdio.h>

void *da_grow_internal(void *arr, size_t item_size) {
    if (arr == NULL) {
        size_t initial_cap = DA_DEFAULT_CAPACITY;
        Da_Header *header =
            (Da_Header *)malloc(sizeof(Da_Header) + initial_cap * item_size);
        if (!header) {
            perror("ERROR: Failed to allocate memory in da_grow\n");
            return NULL;
        }
        header->count = 0;
        header->capacity = initial_cap;
        return (void *)(header + 1);
    }

    Da_Header *header = da_header(arr);
    if (header->count < header->capacity) {
        return arr;
    }

    size_t new_cap = header->capacity * 2;
    Da_Header *new_header =
        (Da_Header *)realloc(header, sizeof(Da_Header) + new_cap * item_size);
    if (!new_header) {
        perror("ERROR: Failed to reallocate memory in da_grow\n");
        return arr;
    }

    new_header->capacity = new_cap;
    return (void *)(new_header + 1);
}

void *da_reserve_internal_impl(void *arr, size_t needed_capacity,
                               size_t item_size) {
    if (arr == NULL) {
        size_t cap = needed_capacity < DA_DEFAULT_CAPACITY ? DA_DEFAULT_CAPACITY
                                                           : needed_capacity;
        Da_Header *header =
            (Da_Header *)malloc(sizeof(Da_Header) + cap * item_size);
        if (!header) {
            perror("ERROR: Failed to allocate memory in da_reserve\n");
            return NULL;
        }
        header->count = 0;
        header->capacity = cap;
        return (void *)(header + 1);
    }

    Da_Header *header = da_header(arr);
    if (header->capacity >= needed_capacity) {
        return arr;
    }

    size_t new_cap = header->capacity * 2;
    while (new_cap < needed_capacity) {
        new_cap *= 2;
    }

    Da_Header *new_header =
        (Da_Header *)realloc(header, sizeof(Da_Header) + new_cap * item_size);
    if (!new_header) {
        perror("ERROR: Failed to reallocate memory in da_reserve\n");
        return arr;
    }

    new_header->capacity = new_cap;
    return (void *)(new_header + 1);
}

#endif // !DYNAMIC_ARRAY_IMPLEMENTATION
