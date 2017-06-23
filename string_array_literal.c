#include "string_array_literal.h"

#include "array_literal.h"
#include "string_literal.h"

static size_t get_str_size(const void *ptr)
{
    return sprint_string_literal(NULL, 0, *((const char **) ptr));
}

static size_t sprint_str(char *buffer, size_t buffer_size, const void *ptr)
{
    return sprint_string_literal(buffer, buffer_size, *((const char **) ptr));
}

size_t sprint_string_array_literal(char *buffer, size_t buffer_size, const char **array, size_t nmemb)
{
    return sprint_array_literal(buffer, buffer_size, array, nmemb, sizeof(char *), get_str_size, sprint_str);
}

char * string_array_literal(const char **array, size_t nmemb)
{
    return array_literal(array, nmemb, sizeof(char *), get_str_size, sprint_str);
}

// vim: set sw=4 et:
