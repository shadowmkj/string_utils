#include <assert.h>
#include <stdio.h>
#include <string.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "string_util.h"

void from_str(void) {
    printf("TEST: from_str\t");
    char *pattern = "Hello World";
    char *other_pattern = "Something Else";
    String_View view = sv_from_cstr(pattern);
    assert(!strcmp(view.string, pattern));      // Positive Case
    assert(strcmp(view.string, other_pattern)); // Negative Case
    printf("STATUS: PASS\n");
}

void to_str(void) {
    printf("TEST: to_str\t");
    char *pattern = "Hello";
    String_View view = sv_from_cstr(pattern);
    assert(!strcmp(sv_to_cstr(&view), pattern));
    printf("STATUS: PASS\n");
}

void append_char(void) {
    printf("TEST: append_char\t");
    char *pattern = "Hell";
    String_View view = sv_from_cstr(pattern);
    char c = 'o';
    sv_append_char(&view, c);
    assert(!strcmp("Hello", view.string));
    printf("STATUS: PASS\n");
}

void append_view(void) {
    printf("TEST: append_view\t");
    char *pattern = "Hello";
    String_View view = sv_from_cstr(pattern);
    char *other = " World";
    String_View other_view = sv_from_cstr(other);
    sv_append_view(&view, &other_view);
    assert(!strcmp("Hello World", view.string));
    printf("STATUS: PASS\n");
}

void append_cstr(void) {
    printf("TEST: append_cstr\t");
    char *pattern = "Hello";
    String_View view = sv_from_cstr(pattern);
    char *other = " World";
    sv_append_cstr(&view, other);
    assert(!strcmp("Hello World", view.string));
    printf("STATUS: PASS\n");
}

void starts_with(void) {
    printf("TEST: starts_with\t");
    char *pattern = "hello";
    String_View view = sv_from_cstr("hello world");
    assert(sv_starts_with(&view, pattern));
    assert(!sv_starts_with(&view, "Something Else"));
    printf("STATUS: PASS\n");
}

void ends_with(void) {
    printf("TEST: ends_with\t");
    char *pattern = "world";
    String_View view = sv_from_cstr("hello world");
    assert(sv_ends_with(&view, pattern));
    assert(sv_ends_with(&view, "hello world")); // full match
    assert(sv_ends_with(&view, "d"));           // single char match
    assert(sv_ends_with(&view, ""));            // empty pattern
    assert(!sv_ends_with(&view, "hello"));      // prefix but not suffix
    assert(!sv_ends_with(&view, "Something Else"));
    assert(!sv_ends_with(&view, "longer than hello world"));
    printf("STATUS: PASS\n");
}

void reset(void) {
    printf("TEST: reset\t");
    String_View view = sv_from_cstr("hello world");
    assert(view.size == 11);
    assert(!strcmp(view.string, "hello world"));
    sv_reset(&view);
    assert(view.size == 0);
    assert(!strcmp(view.string, ""));
    sv_append_cstr(&view, "new string");
    assert(view.size == 10);
    assert(!strcmp(view.string, "new string"));
    printf("STATUS: PASS\n");
}

void free_view(void) {
    printf("TEST: free\t");
    String_View view = sv_from_cstr("hello world");
    assert(view.string != NULL);
    assert(view.capacity > 0);
    sv_free(&view);
    assert(view.string == NULL);
    assert(view.size == 0);
    assert(view.capacity == 0);
    printf("STATUS: PASS\n");
}

void contains(void) {
    printf("TEST: contains\t");
    String_View view = sv_from_cstr("the quick brown fox");
    assert(sv_contains(&view, "quick"));
    assert(sv_contains(&view, "the"));
    assert(sv_contains(&view, "fox"));
    assert(sv_contains(&view, ""));
    assert(!sv_contains(&view, "lazy"));
    assert(!sv_contains(&view, "the quick brown fox jumps"));
    sv_free(&view);
    printf("STATUS: PASS\n");
}

void equals(void) {
    printf("TEST: equals\t");
    String_View v1 = sv_from_cstr("hello");
    String_View v2 = sv_from_cstr("hello");
    String_View v3 = sv_from_cstr("world");
    String_View v4 = sv_from_cstr("hello world");
    assert(sv_equals(&v1, &v2));
    assert(!sv_equals(&v1, &v3));
    assert(!sv_equals(&v1, &v4));
    sv_free(&v1);
    sv_free(&v2);
    sv_free(&v3);
    sv_free(&v4);
    printf("STATUS: PASS\n");
}

void equals_cstr(void) {
    printf("TEST: equals_cstr\t");
    String_View view = sv_from_cstr("hello");
    assert(sv_equals_cstr(&view, "hello"));
    assert(!sv_equals_cstr(&view, "world"));
    assert(!sv_equals_cstr(&view, "hello world"));
    assert(!sv_equals_cstr(&view, "hell"));
    sv_free(&view);
    printf("STATUS: PASS\n");
}

void trim(void) {
    printf("TEST: trim\t");
    String_View view = sv_from_cstr("   \t  hello world \n \r  ");
    sv_trim(&view);
    assert(!strcmp(view.string, "hello world"));
    assert(view.size == 11);

    // All whitespace
    String_View empty_view = sv_from_cstr("   \t\n  ");
    sv_trim(&empty_view);
    assert(!strcmp(empty_view.string, ""));
    assert(empty_view.size == 0);

    // No whitespace
    String_View no_ws = sv_from_cstr("hello");
    sv_trim(&no_ws);
    assert(!strcmp(no_ws.string, "hello"));
    assert(no_ws.size == 5);

    sv_free(&view);
    sv_free(&empty_view);
    sv_free(&no_ws);
    printf("STATUS: PASS\n");
}

void clone_view(void) {
    printf("TEST: clone\t");
    String_View original = sv_from_cstr("clone me");
    String_View copy = sv_clone(&original);
    assert(copy.string != original.string); // distinct allocations
    assert(copy.size == original.size);
    assert(copy.capacity == original.capacity);
    assert(!strcmp(copy.string, original.string));
    sv_free(&original);
    // copy should remain valid after original is freed
    assert(!strcmp(copy.string, "clone me"));
    sv_free(&copy);
    printf("STATUS: PASS\n");
}

void detach(void) {
    printf("TEST: detach\t");
    String_View view = sv_from_cstr("detach me");
    char *raw = sv_detach(&view);
    assert(raw != NULL);
    assert(!strcmp(raw, "detach me"));
    assert(view.string == NULL);
    assert(view.size == 0);
    assert(view.capacity == 0);
    free(raw);
    printf("STATUS: PASS\n");
}

void char_at(void) {
    printf("TEST: char_at\t");
    String_View view = sv_from_cstr("abc");
    assert(sv_char_at(&view, 0) == 'a');
    assert(sv_char_at(&view, 1) == 'b');
    assert(sv_char_at(&view, 2) == 'c');
    assert(sv_char_at(&view, 3) == '\0'); // out of bounds
    assert(sv_char_at(&view, 100) == '\0');
    sv_free(&view);
    printf("STATUS: PASS\n");
}

int main(void) {
    from_str();
    to_str();
    starts_with();
    ends_with();
    append_cstr();
    append_char();
    append_view();
    reset();
    free_view();
    contains();
    equals();
    equals_cstr();
    trim();
    clone_view();
    detach();
    char_at();
    return 0;
}

