#include "int_array_literal.h"

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include "array_literal.h"

static size_t get_int_size(const void *ptr)
{
    assert(ptr);
    int value = *((int *) ptr);
    if (value == INT_MIN)
        return 1 + log10(INT_MAX);
    if (value < 0)
        return 2 + log10(-value);
    if (value > 0)
        return 1 + log10(value);
    return 1;
}

static size_t sprint_int(char *buffer, size_t buffer_size, const void *ptr)
{
    assert(ptr);
    size_t result = snprintf(buffer, buffer_size, "%d", *((const int *) ptr));
    if (result < buffer_size)
        return result;
    if (buffer_size)
        buffer[0] = '\0';
    return 0;
}

size_t sprint_int_array_literal(char *buffer, size_t buffer_size, const int *array, size_t nmemb)
{
    return sprint_array_literal(buffer, buffer_size, array, nmemb, sizeof(int), get_int_size, sprint_int);
}

char * int_array_literal(const int *array, size_t nmemb)
{
    return array_literal(array, nmemb, sizeof(int), get_int_size, sprint_int);
}

// vim: set sw=4 et:
