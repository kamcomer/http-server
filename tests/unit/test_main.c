#include <check.h>
#include <stdlib.h>

Suite *request_suite(void);
Suite *response_suite(void);
Suite *util_suite(void);

int main(void)
{
    int number_failed;
    SRunner *sr;

    sr = srunner_create(request_suite());
    srunner_add_suite(sr, response_suite());
    srunner_add_suite(sr, util_suite());

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
