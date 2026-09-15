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
    return 0;
}
