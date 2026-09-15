#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#define DEFAULT_BUFFER_SIZE 256

#include <ctype.h>
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
void sv_append_view(String_View *view, const String_View *other);
bool sv_ends_with(const String_View *view, const char *pattern);
void sv_reset(String_View *view);
void sv_free(String_View *view);

bool sv_contains(const String_View *view, const char *pattern);
bool sv_equals(const String_View *view, const String_View *other);
bool sv_equals_cstr(const String_View *view, const char *other);
void sv_trim(String_View *view);
String_View sv_clone(const String_View *view);
char *sv_detach(String_View *view);
char sv_char_at(const String_View *view, size_t index);

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
    if (!view || !other || !other->string || other->size == 0) {
        return;
    }

    size_t required_capacity = view->size + other->size + 1;
    if (required_capacity > view->capacity) {
        size_t new_capacity =
            (view->capacity == 0) ? DEFAULT_BUFFER_SIZE : view->capacity * 2;
        while (new_capacity < required_capacity) {
            new_capacity *= 2;
        }

        char *new_buffer = (char *)realloc(view->string, new_capacity);
        if (!new_buffer) {
            perror("ERROR: Failed to allocate memory in sv_append_view\n");
            return;
        }

        view->string = new_buffer;
        view->capacity = new_capacity;
    }

    memcpy(view->string + view->size, other->string, other->size);
    view->size += other->size;
    view->string[view->size] = '\0';
}

void sv_reset(String_View *view) {
    if (!view) {
        return;
    }
    view->size = 0;
    if (view->string && view->capacity > 0) {
        view->string[0] = '\0';
    }
}

void sv_free(String_View *view) {
    if (!view) {
        return;
    }
    if (view->string) {
        free(view->string);
        view->string = NULL;
    }
    view->size = 0;
    view->capacity = 0;
}

bool sv_contains(const String_View *view, const char *pattern) {
    if (!view || !pattern || !view->string) {
        return false;
    }
    if (pattern[0] == '\0') {
        return true;
    }
    return strstr(view->string, pattern) != NULL;
}

bool sv_equals(const String_View *view, const String_View *other) {
    if (!view || !other) {
        return false;
    }
    if (view == other) {
        return true;
    }
    if (view->size != other->size) {
        return false;
    }
    if (view->size == 0) {
        return true;
    }
    if (!view->string || !other->string) {
        return false;
    }
    return memcmp(view->string, other->string, view->size) == 0;
}

bool sv_equals_cstr(const String_View *view, const char *other) {
    if (!view || !other || !view->string) {
        return false;
    }
    size_t other_len = strlen(other);
    if (view->size != other_len) {
        return false;
    }
    return memcmp(view->string, other, view->size) == 0;
}

void sv_trim(String_View *view) {
    if (!view || !view->string || view->size == 0) {
        return;
    }

    size_t start = 0;
    while (start < view->size && isspace((unsigned char)view->string[start])) {
        start++;
    }

    if (start == view->size) {
        view->string[0] = '\0';
        view->size = 0;
        return;
    }

    size_t end = view->size - 1;
    while (end > start && isspace((unsigned char)view->string[end])) {
        end--;
    }

    size_t new_size = end - start + 1;
    if (start > 0) {
        memmove(view->string, view->string + start, new_size);
    }
    view->string[new_size] = '\0';
    view->size = new_size;
}

String_View sv_clone(const String_View *view) {
    if (!view || !view->string) {
        return (String_View){.string = NULL, .size = 0, .capacity = 0};
    }

    char *buffer = malloc(view->capacity);
    if (!buffer) {
        perror("ERROR: Failed to allocate memory in sv_clone\n");
        return (String_View){.string = NULL, .size = 0, .capacity = 0};
    }

    memcpy(buffer, view->string, view->size + 1);
    return (String_View){
        .string = buffer,
        .size = view->size,
        .capacity = view->capacity,
    };
}

char *sv_detach(String_View *view) {
    if (!view || !view->string) {
        return NULL;
    }

    char *detached = view->string;
    view->string = NULL;
    view->size = 0;
    view->capacity = 0;
    return detached;
}

char sv_char_at(const String_View *view, size_t index) {
    if (!view || !view->string || index >= view->size) {
        return '\0';
    }
    return view->string[index];
}

#endif // !STRING_BUILDER_IMPLEMENTATION
