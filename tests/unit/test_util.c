#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

START_TEST(test_str_to_upper_basic)
{
    char str[] = "hello";
    str_to_upper(str);
    ck_assert_str_eq(str, "HELLO");
}
END_TEST

START_TEST(test_str_to_upper_mixed)
{
    char str[] = "Hello World";
    str_to_upper(str);
    ck_assert_str_eq(str, "HELLO WORLD");
}
END_TEST

START_TEST(test_str_to_upper_already_upper)
{
    char str[] = "ALREADY UPPER";
    str_to_upper(str);
    ck_assert_str_eq(str, "ALREADY UPPER");
}
END_TEST

START_TEST(test_str_to_upper_numbers)
{
    char str[] = "get123";
    str_to_upper(str);
    ck_assert_str_eq(str, "GET123");
}
END_TEST

Suite *util_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Util");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_str_to_upper_basic);
    tcase_add_test(tc_core, test_str_to_upper_mixed);
    tcase_add_test(tc_core, test_str_to_upper_already_upper);
    tcase_add_test(tc_core, test_str_to_upper_numbers);
    suite_add_tcase(s, tc_core);

    return s;
}
