#include <assert.h>
#include <stdio.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "string_util.h"

void starts_with() {
    printf("TEST: starts_with\n");
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
    starts_with();
    return 0;
}
