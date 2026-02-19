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

    free_request(&request);
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

    free_request(&request);
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

START_TEST(test_parse_request_with_headers)
{
    Request request;
    char *raw_request = "GET /index.html HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.64.1\r\nAccept: */*\r\n\r\n";

    int result = parse_request(&request, raw_request);

    ck_assert_int_eq(result, 0);
    ck_assert_str_eq(request.headers.host, "localhost:8080");
    ck_assert_str_eq(request.headers.user_agent, "curl/7.64.1");
    ck_assert_str_eq(request.headers.accept, "*/*");

    free_request(&request);
}
END_TEST

START_TEST(test_parse_request_with_multiple_headers)
{
    Request request;
    char *raw_request = "GET /test.html HTTP/1.1\r\n"
                        "Host: example.com\r\n"
                        "User-Agent: Mozilla/5.0\r\n"
                        "Accept: text/html,application/json\r\n"
                        "Accept-Language: en-US,en;q=0.9\r\n"
                        "Connection: keep-alive\r\n"
                        "\r\n";

    int result = parse_request(&request, raw_request);

    ck_assert_int_eq(result, 0);
    ck_assert_str_eq(request.headers.host, "example.com");
    ck_assert_str_eq(request.headers.user_agent, "Mozilla/5.0");
    ck_assert_str_eq(request.headers.accept, "text/html,application/json");
    ck_assert_str_eq(request.headers.accept_language, "en-US,en;q=0.9");
    ck_assert_str_eq(request.headers.connection, "keep-alive");

    free_request(&request);
}
END_TEST

START_TEST(test_parse_request_no_headers)
{
    Request request;
    char *raw_request = "GET /index.html HTTP/1.1\r\n\r\n";

    int result = parse_request(&request, raw_request);

    ck_assert_int_eq(result, 0);
    ck_assert_ptr_eq(request.headers.host, NULL);

    free_request(&request);
}
END_TEST

Suite *request_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;
    TCase *tc_headers;

    s = suite_create("Request");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_parse_valid_get_request);
    tcase_add_test(tc_core, test_parse_get_request_with_path);
    tcase_add_test(tc_core, test_header_parse_method_get);
    tcase_add_test(tc_core, test_header_parse_protocol_valid);
    tcase_add_test(tc_core, test_header_parse_protocol_http_1_0);
    suite_add_tcase(s, tc_core);

    tc_headers = tcase_create("Headers");
    tcase_add_test(tc_headers, test_parse_request_with_headers);
    tcase_add_test(tc_headers, test_parse_request_with_multiple_headers);
    tcase_add_test(tc_headers, test_parse_request_no_headers);
    suite_add_tcase(s, tc_headers);

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
