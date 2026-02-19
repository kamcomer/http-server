#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "response.h"

START_TEST(test_build_header_basic)
{
    char *header = build_header(200, "OK", 1, 1, "text/html", 100);

    ck_assert_ptr_ne(header, NULL);
    ck_assert(strstr(header, "HTTP/1.1 200 OK") != NULL);
    ck_assert(strstr(header, "Content-Type: text/html") != NULL);
    ck_assert(strstr(header, "Content-Length: 100") != NULL);

    free(header);
}
END_TEST

START_TEST(test_build_header_empty_content_type)
{
    char *header = build_header(200, "OK", 1, 1, "", 50);

    ck_assert_ptr_ne(header, NULL);
    ck_assert(strstr(header, "Content-Type: ") != NULL);

    free(header);
}
END_TEST

START_TEST(test_build_header_http_1_0)
{
    char *header = build_header(200, "OK", 1, 0, "text/plain", 25);

    ck_assert_ptr_ne(header, NULL);
    ck_assert(strstr(header, "HTTP/1.0 200 OK") != NULL);
    ck_assert(strstr(header, "Content-Type: text/plain") != NULL);
    ck_assert(strstr(header, "Content-Length: 25") != NULL);

    free(header);
}
END_TEST

START_TEST(test_build_header_404)
{
    char *header = build_header(404, "Not Found", 1, 1, "text/html", 100);

    ck_assert_ptr_ne(header, NULL);
    ck_assert(strstr(header, "HTTP/1.1 404 Not Found") != NULL);
    ck_assert(strstr(header, "Content-Type: text/html") != NULL);

    free(header);
}
END_TEST

Suite *response_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Response");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_build_header_basic);
    tcase_add_test(tc_core, test_build_header_empty_content_type);
    tcase_add_test(tc_core, test_build_header_http_1_0);
    tcase_add_test(tc_core, test_build_header_404);
    suite_add_tcase(s, tc_core);

    return s;
}
