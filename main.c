#include <assert.h>
#include <stdio.h>
#include <string.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "string_util.h"

void from_str() {
    printf("TEST: from_str\t");
    char *pattern = "Hello World";
    char *other_pattern = "Something Else";
    String_View view = sv_from_cstr(pattern);
    assert(!strcmp(view.string, pattern));      // Positive Case
    assert(strcmp(view.string, other_pattern)); // Negative Case
    printf("STATUS: PASS\n");
}

void to_str() {
    printf("TEST: to_str\t");
    char *pattern = "Hello";
    String_View view = sv_from_cstr(pattern);
    assert(!strcmp(sv_to_cstr(&view), pattern));
    printf("STATUS: PASS\n");
}

void append_cstr() {
    printf("TEST: append_cstr\t");
    char *pattern = "Hello";
    String_View view = sv_from_cstr(pattern);
    char *other = " World";
    sv_append_cstr(&view, other);
    assert(!strcmp("Hello World", view.string));
    printf("STATUS: PASS\n");
}

void starts_with() {
    printf("TEST: starts_with\t");
    char *pattern = "hello";
    String_View view = sv_from_cstr("hello world");
    assert(sv_starts_with(&view, pattern));
    assert(!sv_starts_with(&view, "Milan"));
    printf("STATUS: PASS\n");
}

int main(void) {
    char *name = "Milan";
    String_View name_view = sv_from_cstr(name);
    printf("%s\n", name_view.string);
    from_str();
    to_str();
    starts_with();
    append_cstr();
    return 0;
}
