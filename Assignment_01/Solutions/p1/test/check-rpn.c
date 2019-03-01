#include "rpn.h"
#include <stdlib.h>
#include <check.h>

START_TEST(test_invalid_token)
{
    char *result = NULL;
    char const inp[] = {'2', '2', ' ', '2', ' ', '+', '0', '/', '\0'};
    rpn_eval_expr(inp, &result);
}
END_TEST

START_TEST(test_missing_operand)
{
    char *result = NULL;
    char const inp[] = {'2', '2', ' ', '2', ' ', '+', ' ', '0', ' ', '+', ' ', '-', '\0'};
    rpn_eval_expr(inp, &result);
}
END_TEST

START_TEST(test_missing_operator)
{
    char *result = NULL;
    char const inp[] = {'2', '2', ' ', '2', ' ', '3', ' ', '8', ' ', '+', '\0'};
    rpn_eval_expr(inp, &result);
}
END_TEST

START_TEST(test_arthemtic_error)
{
    char *result = NULL;
    char const inp[] = {'2', '2', ' ', '2', ' ', '+', ' ', '0', ' ', '/', '\0'};
    rpn_eval_expr(inp, &result);
}
END_TEST

START_TEST(test_random_calc)
{
    char *result = NULL;
    char const inp[] = {'1', '0', '0', ' ', '2', '3', '4', ' ', '+', ' ', '2', ' ', '/', '\0'};
    rpn_eval_expr(inp, &result);
    ck_assert_int_eq(*result, 167);
}
END_TEST

static Suite *stack_suite(void)
{
    Suite *s;
    TCase *tc_invalid_token, *tc_missing_operand, *tc_missing_operator, *tc_arthemtic_error, *tc_random_calc;

    s = suite_create("all");

    tc_missing_operand = tcase_create("missing_operand");
    tcase_add_exit_test(tc_missing_operand, test_missing_operand, -2);
    suite_add_tcase(s, tc_missing_operand);

    tc_invalid_token = tcase_create("invalid_token");
    tcase_add_exit_test(tc_invalid_token, test_invalid_token, -1);
    suite_add_tcase(s, tc_invalid_token);

    tc_missing_operator = tcase_create("missing_operator");
    tcase_add_exit_test(tc_missing_operator, test_missing_operator, -3);
    suite_add_tcase(s, tc_missing_operator);

    tc_arthemtic_error = tcase_create("arthemtic_error");
    tcase_add_exit_test(tc_arthemtic_error, test_arthemtic_error, -4);
    suite_add_tcase(s, tc_arthemtic_error);

    tc_random_calc = tcase_create("random_calc");
    tcase_add_test(tc_random_calc, test_random_calc);
    suite_add_tcase(s, tc_random_calc);
    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = stack_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}