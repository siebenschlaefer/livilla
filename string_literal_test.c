#include "string_literal.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <criterion/criterion.h>

static void test_sprint_string_literal_with_null(const char *str, size_t expected_length)
{
    size_t actual_length = sprint_string_literal(NULL, 0, str);
    cr_assert_eq(actual_length, expected_length,
        "sprint_string_literal(NULL, x, \"%s\") should return %zd, actual %zd",
        str, expected_length, actual_length);
}

static void test_sprint_string_literal(const char *str, const char *expected)
{
    // allocate buffer
    const char *boundguard = "|--|";
    const size_t boundguard_size = strlen(boundguard) + 1;
    const size_t expected_length = strlen(expected);
    size_t buffer_size = expected_length + 1 + 2 * boundguard_size;
    char *buffer = (char *) malloc(buffer_size);
    if (! buffer)
    {
        cr_assert_fail("INTERNAL ERROR: could not allocate memory");
        return;
    }
    char *buffer1 = buffer + boundguard_size;
    const size_t buffer1_size = expected_length + 1;
    memset(buffer, '_', buffer_size);
    sprintf(buffer, "%s", boundguard);
    sprintf(buffer1 + buffer1_size, "%s", boundguard);

    // test sprint_string_literal(...)
    size_t actual_length = sprint_string_literal(buffer1, buffer1_size, str);
    if (strcmp(boundguard, buffer))
        cr_assert_fail(
            "sprint_string_literal(%p, %zd, \"%s\") wrote left of the buffer",
            buffer1, buffer1_size, str);
    if (strcmp(boundguard, buffer1 + buffer1_size))
        cr_assert_fail(
            "sprint_string_literal(%p, %zd, \"%s\") wrote right of the buffer",
            buffer1, buffer1_size, str);
    if (actual_length != expected_length)
        cr_assert_fail(
            "sprint_string_literal(buffer, buffer_size, \"%s\") should return %zd, actual %zd",
            str, expected_length, actual_length);
    cr_assert_str_eq(buffer1, expected,
        "sprint_string_literal(buffer, buffer_size, \"%s\") should write \"%s\" to buffer, actual \"%s\"",
        str, expected, buffer1);
    free(buffer);
}

static void test_string_literal(const char *str, const char *expected)
{
    char *actual = string_literal(str);
    if (!actual)
        cr_assert_fail("string_literal(\"%s\") should not return NULL", str);
    if (str)
        cr_assert_str_eq(actual, expected,
            "string_literal(\"%s\") should return \"%s\", actual \"%s\"",
            str, expected, actual);
    else
        cr_assert_str_eq(actual, expected,
            "string_literal(NULL) should return \"%s\", actual \"%s\"",
            expected, actual);
    free(actual);
}

static void perform_tests(const char *str, const char *expected)
{
    test_sprint_string_literal_with_null(str, strlen(expected));
    test_sprint_string_literal(str, expected);
    test_string_literal(str, expected);
}

Test(string_literal, should_handle_corner_cases)
{
    perform_tests(NULL, "NULL");
    perform_tests("", "\"\"");
}

Test(string_literal, should_handle_alphanumerics)
{
    const char *chars =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";
    for (const char *cp = chars; *cp; ++cp)
    {
        char str[10], expected[10];
        sprintf(str, "%c", *cp);
        sprintf(expected, "\"%c\"", *cp);
        perform_tests(str, expected);
    }
}

Test(string_literal, should_handle_simple_escape_sequences)
{
    perform_tests("\"", "\"\\\"\"");
    perform_tests("\\", "\"\\\\\"");
    perform_tests("\a", "\"\\a\"");
    perform_tests("\b", "\"\\b\"");
    perform_tests("\f", "\"\\f\"");
    perform_tests("\n", "\"\\n\"");
    perform_tests("\r", "\"\\r\"");
    perform_tests("\t", "\"\\t\"");
    perform_tests("\v", "\"\\v\"");
}

Test(string_literal, should_handle_printables)
{
    for (int i = CHAR_MIN; i <= CHAR_MAX; ++i)
    {
        char c = (char) i;
        if (c == '\0' || strchr("\"\\\a\b\f\n\r\t\v", c) || !isprint(c))
            continue; // tested elsewhere
        char str[10], expected[10];
        sprintf(str, "%c", c);
        sprintf(expected, "\"%c\"", c);
        perform_tests(str, expected);
    }
}

Test(string_literal, should_handle_unprintables)
{
    for (int i = CHAR_MIN; i <= CHAR_MAX; ++i)
    {
        char c = (char) i;
        if (c == '\0' || strchr("\"\\\a\b\f\n\r\t\v", c) || isprint(c))
            continue; // tested elsewhere
        char str[10], expected[10];
        sprintf(str, "%c", c);
        sprintf(expected, "\"\\x%x%x\"", (unsigned char) c / 16, (unsigned char) c % 16);
        perform_tests(str, expected);
    }
}

Test(string_literal, should_handle_unprintables_and_xdigits)
{
    perform_tests("\x01" "123\x02", "\"\\x01\" \"123\\x02\"");
}

// vim: set sw=4 et:
