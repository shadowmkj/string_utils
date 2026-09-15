#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DYNAMIC_ARRAY_IMPLEMENTATION
#include "array_util.h"

typedef struct {
    int id;
    double score;
    char tag[16];
} Item;

void test_push_and_index_ints(void) {
    printf("TEST: push_and_index_ints\t");
    int *numbers = NULL;
    assert(da_count(numbers) == 0);
    assert(da_capacity(numbers) == 0);
    assert(da_empty(numbers));

    for (int i = 0; i < 100; i++) {
        da_push(numbers, i * 10);
    }

    assert(da_count(numbers) == 100);
    assert(da_capacity(numbers) >= 100);
    assert(!da_empty(numbers));

    for (int i = 0; i < 100; i++) {
        assert(numbers[i] == i * 10);
    }

    da_free(numbers);
    assert(numbers == NULL);
    assert(da_count(numbers) == 0);
    printf("STATUS: PASS\n");
}

void test_pop_and_last(void) {
    printf("TEST: pop_and_last\t\t");
    int *arr = NULL;
    da_push(arr, 10);
    da_push(arr, 20);
    da_push(arr, 30);

    assert(da_count(arr) == 3);
    assert(da_last(arr) == 30);

    int popped = da_pop(arr);
    assert(popped == 30);
    assert(da_count(arr) == 2);
    assert(da_last(arr) == 20);

    popped = da_pop(arr);
    assert(popped == 20);
    assert(da_count(arr) == 1);
    assert(da_last(arr) == 10);

    popped = da_pop(arr);
    assert(popped == 10);
    assert(da_count(arr) == 0);
    assert(da_empty(arr));

    da_free(arr);
    printf("STATUS: PASS\n");
}

void test_reserve(void) {
    printf("TEST: reserve\t\t\t");
    int *arr = NULL;
    da_reserve(arr, 50);
    assert(da_count(arr) == 0);
    assert(da_capacity(arr) >= 50);

    size_t cap = da_capacity(arr);
    for (int i = 0; i < 50; i++) {
        da_push(arr, i);
    }
    // Capacity should not have reallocated since we reserved 50
    assert(da_capacity(arr) == cap);
    assert(da_count(arr) == 50);

    da_free(arr);
    printf("STATUS: PASS\n");
}

void test_clear(void) {
    printf("TEST: clear\t\t\t");
    int *arr = NULL;
    da_push(arr, 1);
    da_push(arr, 2);
    da_push(arr, 3);
    assert(da_count(arr) == 3);
    size_t cap_before = da_capacity(arr);

    da_clear(arr);
    assert(da_count(arr) == 0);
    assert(da_empty(arr));
    assert(da_capacity(arr) == cap_before); // capacity preserved

    // Re-push after clear
    da_push(arr, 42);
    assert(da_count(arr) == 1);
    assert(arr[0] == 42);

    da_free(arr);
    printf("STATUS: PASS\n");
}

void test_custom_struct(void) {
    printf("TEST: custom_struct\t\t");
    Item *items = NULL;
    Item a = {.id = 1, .score = 98.5, .tag = "first"};
    Item b = {.id = 2, .score = 88.0, .tag = "second"};

    da_push(items, a);
    da_push(items, b);

    assert(da_count(items) == 2);
    assert(items[0].id == 1);
    assert(items[0].score == 98.5);
    assert(!strcmp(items[0].tag, "first"));

    assert(items[1].id == 2);
    assert(items[1].score == 88.0);
    assert(!strcmp(items[1].tag, "second"));

    Item last = da_last(items);
    assert(last.id == 2);

    Item popped = da_pop(items);
    assert(popped.id == 2);
    assert(da_count(items) == 1);

    da_free(items);
    assert(items == NULL);
    printf("STATUS: PASS\n");
}

void test_string_pointers(void) {
    printf("TEST: string_pointers\t\t");
    const char **words = NULL;
    da_push(words, "apple");
    da_push(words, "banana");
    da_push(words, "cherry");

    assert(da_count(words) == 3);
    assert(!strcmp(words[0], "apple"));
    assert(!strcmp(words[1], "banana"));
    assert(!strcmp(words[2], "cherry"));

    da_free(words);
    assert(words == NULL);
    printf("STATUS: PASS\n");
}

int main(void) {
    test_push_and_index_ints();
    test_pop_and_last();
    test_reserve();
    test_clear();
    test_custom_struct();
    test_string_pointers();
    return 0;
}
