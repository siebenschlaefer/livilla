#ifndef INT_ARRAY_LITERAL_H
#define INT_ARRAY_LITERAL_H

#include <stddef.h>

size_t sprint_int_array_literal(char *buffer, size_t buffer_size, const int *array, size_t nmemb);
char * int_array_literal(const int *array, size_t nmemb);

// vim: set sw=4 et:
#endif
