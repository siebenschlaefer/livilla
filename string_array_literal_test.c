#include "string_array_literal.h"
#include <stdio.h>
#include <criterion/criterion.h>

static void test_string_array_literal(const char **array, size_t nmemb, const char *expected)
{
    // test sprint_string_array_literal(NULL, ...)
    size_t actual_length = sprint_string_array_literal(NULL, 0, array, nmemb);
    size_t expected_length = strlen(expected);
    cr_assert_eq(actual_length, expected_length,
        "sprint_string_array_literal(NULL, x, %p, %zd) should return %zd, actual %zd",
        array, nmemb, expected_length, actual_length);

    // allocate buffer
    const char *boundguard = "|--|";
    const size_t boundguard_size = strlen(boundguard) + 1;
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

    // test sprint_string_array_literal(...)
    actual_length = sprint_string_array_literal(buffer1, buffer1_size, array, nmemb);
    if (strcmp(boundguard, buffer))
        cr_assert_fail(
            "sprint_string_array_literal(%p, %zd, %p, %zd) wrote left of the buffer",
            buffer1, buffer1_size, array, nmemb);
    if (strcmp(boundguard, buffer1 + buffer1_size))
        cr_assert_fail(
            "sprint_string_array_literal(%p, %zd, %p, %zd) wrote right of the buffer",
            buffer1, buffer1_size, array, nmemb);
    if (actual_length != expected_length)
        cr_assert_fail(
            "sprint_string_array_literal(%p, %zd, %p, %zd) should return %zd, actual %zd",
            buffer1, buffer1_size, array, nmemb, expected_length, actual_length);
    cr_assert_str_eq(buffer1, expected,
        "sprint_string_array_literal(%p, %zd, %p, %zd) should write \"%s\" to buffer, actual \"%s\"",
        buffer1, buffer1_size, array, nmemb, expected, buffer1);
    free(buffer);

    // test string_array_literal(...)
    char *actual = string_array_literal(array, nmemb);
    if (!actual)
        cr_assert_fail("string_array_literal(%p, %zd) should not return NULL", array, nmemb);
    cr_assert_str_eq(actual, expected,
        "string_array_literal(%p, %zd) should return \"%s\", actual \"%s\"",
        array, nmemb, expected, actual);
    free(actual);
}

Test(sprint_string_array_literal, should_pass_some_example_tests)
{
    const char* array[9] = {"abc", "xyz", NULL, "\"'", "some string", "", "\\", "\a\n", "\x02\x03"};
    test_string_array_literal(NULL, 0, "NULL");
    test_string_array_literal(&array[ 0], 0, "{}");

    test_string_array_literal(&array[0], 1, "{\"abc\"}");
    test_string_array_literal(&array[1], 1, "{\"xyz\"}");
    test_string_array_literal(&array[2], 1, "{NULL}");
    test_string_array_literal(&array[3], 1, "{\"\\\"'\"}");
    test_string_array_literal(&array[4], 1, "{\"some string\"}");
    test_string_array_literal(&array[5], 1, "{\"\"}");
    test_string_array_literal(&array[6], 1, "{\"\\\\\"}");
    test_string_array_literal(&array[7], 1, "{\"\\a\\n\"}");
    test_string_array_literal(&array[8], 1, "{\"\\x02\\x03\"}");

    test_string_array_literal(&array[0], 2, "{\"abc\", \"xyz\"}");
    test_string_array_literal(&array[1], 2, "{\"xyz\", NULL}");
    test_string_array_literal(&array[2], 2, "{NULL, \"\\\"'\"}");
    test_string_array_literal(&array[3], 2, "{\"\\\"'\", \"some string\"}");
    test_string_array_literal(&array[4], 2, "{\"some string\", \"\"}");
    test_string_array_literal(&array[5], 2, "{\"\", \"\\\\\"}");
    test_string_array_literal(&array[6], 2, "{\"\\\\\", \"\\a\\n\"}");
    test_string_array_literal(&array[7], 2, "{\"\\a\\n\", \"\\x02\\x03\"}");

    test_string_array_literal(&array[0], 3, "{\"abc\", \"xyz\", NULL}");
    test_string_array_literal(&array[1], 3, "{\"xyz\", NULL, \"\\\"'\"}");
    test_string_array_literal(&array[2], 3, "{NULL, \"\\\"'\", \"some string\"}");
    test_string_array_literal(&array[3], 3, "{\"\\\"'\", \"some string\", \"\"}");
    test_string_array_literal(&array[4], 3, "{\"some string\", \"\", \"\\\\\"}");
    test_string_array_literal(&array[5], 3, "{\"\", \"\\\\\", \"\\a\\n\"}");
    test_string_array_literal(&array[6], 3, "{\"\\\\\", \"\\a\\n\", \"\\x02\\x03\"}");

    test_string_array_literal(array, 9, "{\"abc\", \"xyz\", NULL, \"\\\"'\", \"some string\", \"\", \"\\\\\", \"\\a\\n\", \"\\x02\\x03\"}");
}

// vim: set sw=4 et:
