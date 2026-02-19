#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"
#include "util.h"

START_TEST(test_parse_valid_get_request)
{
    Request request;
    char *raw_request = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";

    int result = parse_request(&request, raw_request);

    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(request.method, METHOD_GET);
    ck_assert_str_eq(request.uri.path, "index.html");
    ck_assert_int_eq(request.protocol.type, PROTOCOL_HTTP);
    ck_assert_int_eq(request.protocol.major_version, 1);
    ck_assert_int_eq(request.protocol.minor_version, 1);

    free(request.uri.path);
}
END_TEST

START_TEST(test_parse_get_request_with_path)
{
    Request request;
    char *raw_request = "GET /path/to/file.html HTTP/1.1\r\nHost: localhost\r\n\r\n";

    int result = parse_request(&request, raw_request);

    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(request.method, METHOD_GET);
    ck_assert_str_eq(request.uri.path, "path/to/file.html");

    free(request.uri.path);
}
END_TEST

START_TEST(test_parse_null_buffer)
{
    Request request;

    int result = parse_request(&request, NULL);

    ck_assert_int_eq(result, INVALID_REQUEST_HEADER);
}
END_TEST

START_TEST(test_parse_empty_buffer)
{
    Request request;
    char *raw_request = "";

    int result = parse_request(&request, raw_request);

    ck_assert_int_eq(result, INVALID_REQUEST_HEADER);
}
END_TEST

START_TEST(test_parse_invalid_method)
{
    Request request;
    char *raw_request = "POST /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";

    int result = parse_request(&request, raw_request);

    ck_assert_int_eq(result, INVALID_REQUEST_HEADER);
}
END_TEST

START_TEST(test_parse_invalid_protocol)
{
    Request request;
    char *raw_request = "GET /index.html FTP/1.1\r\nHost: localhost\r\n\r\n";

    int result = parse_request(&request, raw_request);

    ck_assert_int_eq(result, INVALID_REQUEST_HEADER);
}
END_TEST

START_TEST(test_parse_invalid_version)
{
    Request request;
    char *raw_request = "GET /index.html HTTP/abc\r\nHost: localhost\r\n\r\n";

    int result = parse_request(&request, raw_request);

    ck_assert_int_eq(result, INVALID_REQUEST_HEADER);
}
END_TEST

START_TEST(test_parse_malformed_request)
{
    Request request;
    char *raw_request = "GET\r\nHost: localhost\r\n\r\n";

    int result = parse_request(&request, raw_request);

    ck_assert_int_eq(result, INVALID_REQUEST_HEADER);
}
END_TEST

START_TEST(test_header_parse_method_get)
{
    RequestMethod method;
    char method_str[] = "GET";

    int result = header_parse_method(&method, method_str);

    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(method, METHOD_GET);
}
END_TEST

START_TEST(test_header_parse_method_post)
{
    RequestMethod method;
    char method_str[] = "POST";

    int result = header_parse_method(&method, method_str);

    ck_assert_int_eq(result, INVALID_REQUEST_HEADER);
}
END_TEST

START_TEST(test_header_parse_method_null)
{
    RequestMethod method;

    int result = header_parse_method(&method, NULL);

    ck_assert_int_eq(result, INVALID_REQUEST_HEADER);
}
END_TEST

START_TEST(test_header_parse_protocol_valid)
{
    Protocol protocol;
    char protocol_str[] = "HTTP/1.1";

    int result = header_parse_protocol(&protocol, protocol_str);

    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(protocol.type, PROTOCOL_HTTP);
    ck_assert_int_eq(protocol.major_version, 1);
    ck_assert_int_eq(protocol.minor_version, 1);
}
END_TEST

START_TEST(test_header_parse_protocol_http_1_0)
{
    Protocol protocol;
    char protocol_str[] = "HTTP/1.0";

    int result = header_parse_protocol(&protocol, protocol_str);

    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(protocol.type, PROTOCOL_HTTP);
    ck_assert_int_eq(protocol.major_version, 1);
    ck_assert_int_eq(protocol.minor_version, 0);
}
END_TEST

Suite *request_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Request");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_parse_valid_get_request);
    tcase_add_test(tc_core, test_parse_get_request_with_path);
    tcase_add_test(tc_core, test_header_parse_method_get);
    tcase_add_test(tc_core, test_header_parse_protocol_valid);
    tcase_add_test(tc_core, test_header_parse_protocol_http_1_0);
    suite_add_tcase(s, tc_core);

    tc_limits = tcase_create("Limits");
    tcase_add_test(tc_limits, test_parse_null_buffer);
    tcase_add_test(tc_limits, test_parse_empty_buffer);
    tcase_add_test(tc_limits, test_parse_invalid_method);
    tcase_add_test(tc_limits, test_parse_invalid_protocol);
    tcase_add_test(tc_limits, test_parse_invalid_version);
    tcase_add_test(tc_limits, test_parse_malformed_request);
    tcase_add_test(tc_limits, test_header_parse_method_post);
    tcase_add_test(tc_limits, test_header_parse_method_null);
    suite_add_tcase(s, tc_limits);

    return s;
}
