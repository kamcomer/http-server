#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "response.h"
#include "request.h"

START_TEST(test_build_header_basic)
{
    Request request;
    request.protocol.major_version = 1;
    request.protocol.minor_version = 1;
    request.uri.path = strdup("test.html");

    char *header = build_header(request, "text/html", 100);

    ck_assert_ptr_ne(header, NULL);
    ck_assert(strstr(header, "HTTP/1.1 200 OK") != NULL);
    ck_assert(strstr(header, "Content-Type: text/html") != NULL);
    ck_assert(strstr(header, "Content-Length: 100") != NULL);

    free(header);
    free(request.uri.path);
}
END_TEST

START_TEST(test_build_header_empty_content_type)
{
    Request request;
    request.protocol.major_version = 1;
    request.protocol.minor_version = 1;
    request.uri.path = strdup("test.html");

    char *header = build_header(request, "", 50);

    ck_assert_ptr_ne(header, NULL);
    ck_assert(strstr(header, "Content-Type: ") != NULL);

    free(header);
    free(request.uri.path);
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
    suite_add_tcase(s, tc_core);

    return s;
}
