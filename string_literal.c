#include "string_literal.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t string_literal_length(const char *str)
{
    size_t result = 4;
    if (str)
    {
        result = 2;
        int previous_was_hex = 0;
        for (const char *cp = str; *cp; ++cp)
        {
            int current_is_hex = 0;
            if (strchr("\"\\\a\b\f\n\r\t\v", *cp))
                result += 2;
            else if (!isprint(*cp))
            {
                result += 4;
                current_is_hex = 1;
            }
            else if (previous_was_hex && isxdigit(*cp))
                result += 4;
            else
                result += 1;
            previous_was_hex = current_is_hex;
        }
    }
    return result;
}

size_t sprint_string_literal(char *buffer, size_t buffer_size, const char *str)
{
    size_t result_size = string_literal_length(str);
    if (!buffer)
        return result_size;
    if (buffer_size <= result_size)
        return 0;

    if (! str)
    {
        sprintf(buffer, "NULL");
        return result_size;
    }

    char *cp = buffer;
    *cp++ = '"';
    int previous_was_hex = 0;
    for (; *str; ++str)
    {
        int current_is_hex = 0;
        assert((size_t) (cp - buffer) < buffer_size);
        if (strchr("\"\\\a\b\f\n\r\t\v", *str))
            cp += sprintf(cp, "\\%c",
                strchr("\"\"\\\\\aa\bb\ff\nn\rr\tt\vv", *str)[1]);
        else if (!isprint(*str))
        {
            cp += sprintf(cp, "\\x%x%x",
                (unsigned char) *str / 16, (unsigned char) *str % 16);
            current_is_hex = 1;
        }
        else if (previous_was_hex && isxdigit(*str))
            cp += sprintf(cp, "\" \"%c", *str);
        else
            *cp++ = *str;
        previous_was_hex = current_is_hex;
    }
    sprintf(cp, "\"");
    return result_size;
}

char * string_literal(const char *string)
{
    size_t result_size = string_literal_length(string) + 1;
    char *result = (char *) malloc(result_size);
    if (result)
        sprint_string_literal(result, result_size, string);
    return result;
}

// vim: set sw=4 et:
