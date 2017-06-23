#include "array_literal.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

size_t sprint_array_literal(char *buffer, size_t buffer_size, const void *array, size_t nmemb,
        size_t elem_size, size_t (*get_size)(const void *), size_t (*sprint)(char *, size_t, const void *))
{
    assert(get_size);
    assert(sprint);
    size_t result_size = 4;
    if (array)
    {
        result_size = 2;
        if (nmemb)
            result_size += get_size(array);
        for (size_t i = 1; i < nmemb; ++i)
            result_size += 2 + get_size(array + i * elem_size);
    }
    if (!buffer)
        return result_size;
    if (buffer_size <= result_size)
        return 0;
    if (array)
    {
        *buffer++= '{';
        if (nmemb)
            buffer += sprint(buffer, buffer_size, array);
        for (size_t i = 1; i < nmemb; ++i)
        {
            buffer += sprintf(buffer, ", ");
            buffer += sprint(buffer, buffer_size, array + i * elem_size);
        }
        sprintf(buffer, "}");
    }
    else
        sprintf(buffer, "NULL");
    return result_size;
}

char * array_literal(const void *array, size_t nmemb,
        size_t elem_size, size_t (*get_size)(const void *), size_t (*sprint)(char *, size_t, const void *))
{
    size_t result_size = sprint_array_literal(NULL, 0, array, nmemb, elem_size, get_size, sprint) + 1;
    char *result = (char *) malloc(result_size);
    if (result)
        sprint_array_literal(result, result_size, array, nmemb, elem_size, get_size, sprint);
    return result;
}

// vim: set sw=4 et:
