#include "int_array_literal.h"
#include <stdio.h>
#include <criterion/criterion.h>

static void test_int_array_literal(const int *array, size_t nmemb, const char *expected)
{
    // test sprint_int_array_literal(NULL, ...)
    const size_t expected_length = strlen(expected);
    size_t actual_length = sprint_int_array_literal(NULL, 0, array, nmemb);
    cr_assert_eq(actual_length, expected_length,
        "sprint_int_array_literal(NULL, x, %p, %zd) should return %zd, actual %zd",
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

    // test sprint_int_array_literal(...)
    actual_length = sprint_int_array_literal(buffer1, buffer1_size, array, nmemb);
    if (strcmp(boundguard, buffer))
        cr_assert_fail(
            "sprint_int_array_literal(%p, %zd, %p, %zd) wrote left of the buffer",
            buffer1, buffer1_size, array, nmemb);
    if (strcmp(boundguard, buffer1 + buffer1_size))
        cr_assert_fail(
            "sprint_int_array_literal(%p, %zd, %p, %zd) wrote right of the buffer",
            buffer1, buffer1_size, array, nmemb);
    if (actual_length != expected_length)
        cr_assert_fail(
            "sprint_int_array_literal(%p, %zd, %p, %zd) should return %zd, actual %zd",
            buffer1, buffer1_size, array, nmemb, expected_length, actual_length);
    cr_assert_str_eq(buffer1, expected,
        "sprint_int_array_literal(%p, %zd, %p, %zd) should write \"%s\" to buffer, actual \"%s\"",
        buffer1, buffer1_size, array, nmemb, expected, buffer1);
    free(buffer);

    // test int_array_literal(...)
    char *actual = int_array_literal(array, nmemb);
    if (!actual)
        cr_assert_fail("int_array_literal(%p, %zd) should not return NULL", array, nmemb);
    cr_assert_str_eq(actual, expected,
        "int_array_literal(%p, %zd) should return \"%s\", actual \"%s\"",
        array, nmemb, expected, actual);
    free(actual);
}

Test(sprint_int_array_literal, should_pass_some_example_tests)
{
    const int array[15] = {-1000, -999, -100, -99, -10, -9, -1, 0, 1, 9, 10, 99, 100, 999, 1000};
    test_int_array_literal(&array[ 0], 0, "{}");

    test_int_array_literal(&array[ 0], 1, "{-1000}");
    test_int_array_literal(&array[ 1], 1, "{-999}");
    test_int_array_literal(&array[ 2], 1, "{-100}");
    test_int_array_literal(&array[ 3], 1, "{-99}");
    test_int_array_literal(&array[ 4], 1, "{-10}");
    test_int_array_literal(&array[ 5], 1, "{-9}");
    test_int_array_literal(&array[ 6], 1, "{-1}");
    test_int_array_literal(&array[ 7], 1, "{0}");
    test_int_array_literal(&array[ 8], 1, "{1}");
    test_int_array_literal(&array[ 9], 1, "{9}");
    test_int_array_literal(&array[10], 1, "{10}");
    test_int_array_literal(&array[11], 1, "{99}");
    test_int_array_literal(&array[12], 1, "{100}");
    test_int_array_literal(&array[13], 1, "{999}");
    test_int_array_literal(&array[14], 1, "{1000}");

    test_int_array_literal(&array[ 0], 2, "{-1000, -999}");
    test_int_array_literal(&array[ 1], 2, "{-999, -100}");
    test_int_array_literal(&array[ 2], 2, "{-100, -99}");
    test_int_array_literal(&array[ 3], 2, "{-99, -10}");
    test_int_array_literal(&array[ 4], 2, "{-10, -9}");
    test_int_array_literal(&array[ 5], 2, "{-9, -1}");
    test_int_array_literal(&array[ 6], 2, "{-1, 0}");
    test_int_array_literal(&array[ 7], 2, "{0, 1}");
    test_int_array_literal(&array[ 8], 2, "{1, 9}");
    test_int_array_literal(&array[ 9], 2, "{9, 10}");
    test_int_array_literal(&array[10], 2, "{10, 99}");
    test_int_array_literal(&array[11], 2, "{99, 100}");
    test_int_array_literal(&array[12], 2, "{100, 999}");
    test_int_array_literal(&array[13], 2, "{999, 1000}");

    test_int_array_literal(&array[ 0], 3, "{-1000, -999, -100}");
    test_int_array_literal(&array[ 1], 3, "{-999, -100, -99}");
    test_int_array_literal(&array[ 2], 3, "{-100, -99, -10}");
    test_int_array_literal(&array[ 3], 3, "{-99, -10, -9}");
    test_int_array_literal(&array[ 4], 3, "{-10, -9, -1}");
    test_int_array_literal(&array[ 5], 3, "{-9, -1, 0}");
    test_int_array_literal(&array[ 6], 3, "{-1, 0, 1}");
    test_int_array_literal(&array[ 7], 3, "{0, 1, 9}");
    test_int_array_literal(&array[ 8], 3, "{1, 9, 10}");
    test_int_array_literal(&array[ 9], 3, "{9, 10, 99}");
    test_int_array_literal(&array[10], 3, "{10, 99, 100}");
    test_int_array_literal(&array[11], 3, "{99, 100, 999}");
    test_int_array_literal(&array[12], 3, "{100, 999, 1000}");

    test_int_array_literal(array, 15, "{-1000, -999, -100, -99, -10, -9, -1, 0, 1, 9, 10, 99, 100, 999, 1000}");
}

// vim: set sw=4 et:
