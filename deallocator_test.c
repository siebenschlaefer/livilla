#include "deallocator.h"

#include <stdio.h>
#include <criterion/criterion.h>

Test(deallocator_define, should_pass_some_example_tests)
{
    DA_STRUCT(1000) da = {{1000, 0}, {0, }};
    cr_assert(sizeof(da) >= sizeof(struct deallocator) + 1000 * sizeof(void *));
    cr_assert_eq(da.head.capacity, 1000);
    cr_assert_eq(da.head.size, 0);
}

Test(deallocator_init, should_pass_some_example_tests)
{
    DA_STRUCT(1000) da;
    deallocator_init(&da.head, 1000);
    cr_assert_eq(da.head.capacity, 1000);
    cr_assert_eq(da.head.size, 0);
}

Test(deallocator_cleanup, should_pass_some_example_tests)
{
    DA_STRUCT(1000) da;
    deallocator_init(&da.head, 1000);
    da_c(&da, strdup("some string 1"));
    da_c(&da, strdup("some string 2"));
    da_c(&da, strdup("some string 3"));
    da_c(&da, strdup("some string 4"));
    da_c(&da, strdup("some string 5"));
    da_c(&da, strdup("some string 6"));
    da_c(&da, strdup("some string 7"));
    da_c(&da, strdup("some string 8"));
    da_c(&da, strdup("some string 9"));

    deallocator_cleanup(&da.head);
    cr_assert_eq(da.head.size, 0);
}

Test(deallocator_da_c, should_pass_some_example_tests)
{
    DA_STRUCT(1000) da;
    deallocator_init(&da.head, 1000);

    char *string1 = strdup("some string");
    cr_assert_eq(da_c(&da, string1), string1);
    cr_assert_eq(da.head.size, 1);
    cr_assert_eq(da.head.pointers[0], string1);

    deallocator_cleanup(&da.head);
}

// vim: set sw=4 et:
