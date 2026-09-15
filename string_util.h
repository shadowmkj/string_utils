#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#define DEFAULT_BUFFER_SIZE 256

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *string;
    size_t size;
    size_t capacity;
} String_View;

String_View sv_from_cstr(const char *cstring);
const char *sv_to_cstr(const String_View *view);
bool sv_starts_with(const String_View *view, const char *pattern);
void sv_append_cstr(String_View *view, const char *cstring);
void sv_append_char(String_View *view, char c);
void sv_append_view(String_View *view, const String_View *other); // TODO:
bool sv_ends_with(const String_View *view, const char *pattern);
void sv_reset(String_View *view); // TODO:
void sv_free(String_View *view);  // TODO:

#endif // !STRING_BUILDER_H

#ifdef STRING_BUILDER_IMPLEMENTATION
#undef STRING_BUILDER_IMPLEMENTATION

String_View sv_from_cstr(const char *cstring) {
    size_t sv_size = strlen(cstring);
    size_t sv_capacity = DEFAULT_BUFFER_SIZE;
    if (sv_size > DEFAULT_BUFFER_SIZE - 1) {
        sv_capacity = sv_size + 255;
    }
    char *buffer = malloc(sv_capacity);
    if (!buffer) {
        perror("ERROR: Failed to allocate memory\n");
        return (String_View){.string = NULL, .size = 0, .capacity = 0};
    }
    memcpy(buffer, cstring, sv_size + 1);
    return (String_View){
        .string = buffer,
        .size = sv_size,
        .capacity = sv_capacity,
    };
}

bool sv_starts_with(const String_View *view, const char *pattern) {
    size_t size_of_pattern = strlen(pattern);
    if (size_of_pattern > view->size)
        return false;
    for (size_t i = 0; i < size_of_pattern; i++) {
        if (view->string[i] != pattern[i]) {
            return false;
        }
    }
    return true;
}

void sv_append_cstr(String_View *view, const char *cstring) {
    if (!view || !cstring) {
        return;
    }

    size_t append_len = strlen(cstring);
    if (append_len == 0) {
        return;
    }

    size_t required_capacity = view->size + append_len + 1;
    if (required_capacity > view->capacity) {
        size_t new_capacity =
            (view->capacity == 0) ? DEFAULT_BUFFER_SIZE : view->capacity * 2;
        while (new_capacity < required_capacity) {
            new_capacity *= 2;
        }

        char *new_buffer = (char *)realloc(view->string, new_capacity);
        if (!new_buffer) {
            perror("ERROR: Failed to allocate memory in sv_append_cstr\n");
            return;
        }

        view->string = new_buffer;
        view->capacity = new_capacity;
    }

    memcpy(view->string + view->size, cstring, append_len);
    view->size += append_len;
    view->string[view->size] = '\0';
}

const char *sv_to_cstr(const String_View *view) {
    if (!view || !view->string)
        return NULL;
    return view->string;
}

bool sv_ends_with(const String_View *view, const char *pattern) {
    if (!view || !pattern || !view->string)
        return false;
    size_t pattern_size = strlen(pattern);
    if (pattern_size > view->size)
        return false;

    return memcmp(view->string + (view->size - pattern_size), pattern,
                  pattern_size) == 0;
}

void sv_append_char(String_View *view, char c) {
    if (!view || !c) {
        return;
    }

    size_t required_capacity = view->size + 2;
    if (required_capacity > view->capacity) {
        size_t new_capacity =
            (view->capacity == 0) ? DEFAULT_BUFFER_SIZE : view->capacity * 2;
        while (new_capacity < required_capacity) {
            new_capacity *= 2;
        }

        char *new_buffer = (char *)realloc(view->string, new_capacity);
        if (!new_buffer) {
            perror("ERROR: Failed to allocate memory in sv_append_char\n");
            return;
        }

        view->string = new_buffer;
        view->capacity = new_capacity;
    }

    memcpy(view->string + view->size, &c, 1);
    view->size += 1;
    view->string[view->size] = '\0';
}

void sv_append_view(String_View *view, const String_View *other) {
    if (!view || !other)
        return;
}

#endif // !STRING_BUILDER_IMPLEMENTATION
