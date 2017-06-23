#include "char_literal.h"

#include <criterion/criterion.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>

static void test_sprint_char_literal_with_null(char c, size_t expected)
{
    size_t actual = sprint_char_literal(NULL, 0, c);
    cr_assert_eq(actual, expected,
        "sprint_char_literal(NULL, x, (char) %d) should return %zd, actual %zd",
        (int) c, expected, actual);
}

static void test_sprint_char_literal(char c, const char *expected)
{
    char buffer[100];
    memset(buffer, '_', sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    size_t expected_size = strlen(expected);
    size_t actual_size = sprint_char_literal(buffer, sizeof(buffer), c);
    if (actual_size != expected_size)
        cr_assert_fail(
            "sprint_char_literal(buffer, buffer_size, (char) %d) should return %zd, actual %zd",
            (int) c, expected_size, actual_size);
    cr_assert_str_eq(buffer, expected,
        "sprint_char_literal(buffer, buffer_size, (char) %d) should write \"%s\" to buffer, actual \"%s\"",
        (int) c, expected, buffer);
}

static void test_char_literal(char c, const char *expected)
{
    char *actual = char_literal(c);
    if (!actual)
        cr_assert_fail("char_literal((char) %d) should not return NULL", (int) c);
    cr_assert_str_eq(actual, expected,
        "char_literal((char) %d) should return \"%s\", actual \"%s\"",
        (int) c, expected, actual);
    free(actual);
}

static void perform_tests(char c, const char *expected)
{
    test_sprint_char_literal_with_null(c, strlen(expected));
    test_sprint_char_literal(c, expected);
    test_char_literal(c, expected);
}

Test(sprint_char_literal, should_handle_alphanumerics)
{
    const char *chars =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";
    for (const char *cp = chars; *cp; ++cp)
    {
        char expected[4];
        expected[0] = '\'';
        expected[1] = *cp;
        expected[2] = '\'';
        expected[3] = '\0';
        perform_tests(*cp, expected);
    }
}

Test(sprint_char_literal, should_handle_simple_escape_sequences)
{
    perform_tests('\0', "'\\0'");
    perform_tests('\'', "'\\''");
    perform_tests('\\', "'\\\\'");
    perform_tests('\a', "'\\a'");
    perform_tests('\b', "'\\b'");
    perform_tests('\f', "'\\f'");
    perform_tests('\n', "'\\n'");
    perform_tests('\r', "'\\r'");
    perform_tests('\t', "'\\t'");
    perform_tests('\v', "'\\v'");
}

Test(sprint_char_literal, should_handle_printables)
{
    for (int i = CHAR_MIN; i <= CHAR_MAX; ++i)
    {
        char c = (char) i;
        if (c == '\0' || strchr("\'\\\a\b\f\n\r\t\v", c) || !isprint(c))
            continue; // tested elsewhere
        char buffer[10];
        sprintf(buffer, "'%c'", c);
        perform_tests(c, buffer);
    }
}

Test(sprint_char_literal, should_handle_unprintables)
{
    for (int i = CHAR_MIN; i <= CHAR_MAX; ++i)
    {
        char c = (char) i;
        if (c == '\0' || strchr("\'\\\a\b\f\n\r\t\v", c) || isprint(c))
            continue; // tested elsewhere
        char buffer[10];
        sprintf(buffer, "'\\x%x%x'", (unsigned char) i / 16, (unsigned char) i % 16);
        perform_tests(c, buffer);
    }
}

// vim: set sw=4 et:
