#ifndef STRING_ARRAY_LITERAL_H
#define STRING_ARRAY_LITERAL_H

#include <stddef.h>

size_t sprint_string_array_literal(char *buffer, size_t buffer_size, const char **array, size_t nmemb);
char * string_array_literal(const char **array, size_t nmemb);

// vim: set sw=4 et:
#endif
