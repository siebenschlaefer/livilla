#ifndef ARRAY_LITERAL_H
#define ARRAY_LITERAL_H

#include <stddef.h>

size_t sprint_array_literal(char *buffer, size_t buffer_size, const void *array, size_t nmemb,
    size_t elem_size, size_t (*get_size)(const void *), size_t (*sprint)(char *, size_t, const void *));

char * array_literal(const void *array, size_t nmemb,
    size_t elem_size, size_t (*get_size)(const void *), size_t (*sprint)(char *, size_t, const void *));

// vim: set sw=4 et:
#endif
