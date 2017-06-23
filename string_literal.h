#ifndef STRING_LITERAL_H
#define STRING_LITERAL_H

#include <stddef.h>

size_t sprint_string_literal(char *buffer, size_t buffer_size, const char *str);
char * string_literal(const char *str);

// vim: set sw=4 et:
#endif
