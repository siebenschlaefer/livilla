#include "char_literal.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

size_t sprint_char_literal(char *buffer, size_t buffer_size, char c)
{
    size_t literal_size = 3;
    if (!c || strchr("\'\\\a\b\f\n\r\t\v", c))
        literal_size = 4;
    else if (!isprint(c))
        literal_size = 6;

    if (!buffer)
        return literal_size;
    if (buffer_size <= literal_size)
        return 0;

    *buffer++ = '\'';
    if (c == '\0')
    {
        *buffer++ = '\\';
        *buffer++ = '0';
    }
    else if (strchr("'\\\a\b\f\n\r\t\v", c))
    {
        *buffer++ = '\\';
        *buffer++ = strchr("''\\\\\aa\bb\ff\nn\rr\tt\vv", c)[1];
    }
    else if (!isprint(c))
    {
        *buffer++ = '\\';
        *buffer++ = 'x';
        *buffer++ = "0123456789abcdef"[(unsigned char) c / 0x10];
        *buffer++ = "0123456789abcdef"[(unsigned char) c % 0x10];
    }
    else
        *buffer++ = c;
    *buffer++ = '\'';
    *buffer = '\0';
    return literal_size;
}

char *char_literal(char c)
{
    size_t literal_size = sprint_char_literal(NULL, 0, c) + 1;
    char *literal = (char *) malloc(literal_size);
    if (literal)
        sprint_char_literal(literal, literal_size, c);
    return literal;
}

// vim: set sw=4 et:
