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

START_TEST(test_mime_type_html)
{
    ck_assert_str_eq(get_mime_type("index.html"), "text/html");
    ck_assert_str_eq(get_mime_type("page.htm"), "text/html");
}
END_TEST

START_TEST(test_mime_type_css_js)
{
    ck_assert_str_eq(get_mime_type("style.css"), "text/css");
    ck_assert_str_eq(get_mime_type("app.js"), "application/javascript");
}
END_TEST

START_TEST(test_mime_type_images)
{
    ck_assert_str_eq(get_mime_type("image.png"), "image/png");
    ck_assert_str_eq(get_mime_type("photo.jpg"), "image/jpeg");
    ck_assert_str_eq(get_mime_type("photo.jpeg"), "image/jpeg");
    ck_assert_str_eq(get_mime_type("graphic.svg"), "image/svg+xml");
}
END_TEST

START_TEST(test_mime_type_unknown)
{
    ck_assert_str_eq(get_mime_type("file.xyz"), "application/octet-stream");
    ck_assert_str_eq(get_mime_type("noextension"), "application/octet-stream");
    ck_assert_str_eq(get_mime_type(NULL), "application/octet-stream");
}
END_TEST

START_TEST(test_url_decode_simple)
{
    char *decoded = url_decode("hello");
    ck_assert_str_eq(decoded, "hello");
    free(decoded);
}
END_TEST

START_TEST(test_url_decode_spaces)
{
    char *decoded = url_decode("hello+world");
    ck_assert_str_eq(decoded, "hello world");
    free(decoded);
}
END_TEST

START_TEST(test_url_decode_percent)
{
    char *decoded = url_decode("hello%20world");
    ck_assert_str_eq(decoded, "hello world");
    free(decoded);
}
END_TEST

START_TEST(test_url_decode_special_chars)
{
    char *decoded = url_decode("file%2Dname%2Dtest");
    ck_assert_str_eq(decoded, "file-name-test");
    free(decoded);
}
END_TEST

START_TEST(test_url_decode_null)
{
    char *decoded = url_decode(NULL);
    ck_assert_ptr_eq(decoded, NULL);
}
END_TEST

Suite *util_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_mime;
    TCase *tc_url;

    s = suite_create("Util");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_str_to_upper_basic);
    tcase_add_test(tc_core, test_str_to_upper_mixed);
    tcase_add_test(tc_core, test_str_to_upper_already_upper);
    tcase_add_test(tc_core, test_str_to_upper_numbers);
    suite_add_tcase(s, tc_core);

    tc_mime = tcase_create("MIME");
    tcase_add_test(tc_mime, test_mime_type_html);
    tcase_add_test(tc_mime, test_mime_type_css_js);
    tcase_add_test(tc_mime, test_mime_type_images);
    tcase_add_test(tc_mime, test_mime_type_unknown);
    suite_add_tcase(s, tc_mime);

    tc_url = tcase_create("URL");
    tcase_add_test(tc_url, test_url_decode_simple);
    tcase_add_test(tc_url, test_url_decode_spaces);
    tcase_add_test(tc_url, test_url_decode_percent);
    tcase_add_test(tc_url, test_url_decode_special_chars);
    tcase_add_test(tc_url, test_url_decode_null);
    suite_add_tcase(s, tc_url);

    return s;
}
