#include <stdlib.h>
#include <check.h>
#include "rpn-stack.h"
struct _rpn_stack
{
    void *data;
    struct _rpn_stack *prev;
};

typedef struct _rpn_stack rpn_stack_t;

START_TEST(test_new)
{
    rpn_stack_t *s = rpn_stack_new();
    void *ptr;
    ptr = s->prev;
    ck_assert_ptr_null(ptr); //check if new stack handle points to NULL
}
END_TEST

START_TEST(test_push)
{
    rpn_stack_t *s = rpn_stack_new();
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    void *ptr;
    ptr = arr;
    rpn_stack_t *tester;
    tester = s->prev;
    for (int i = 0; i < 10; i++)
    {
        rpn_stack_push(s, ptr);
        tester = s->prev;
        ck_assert_ptr_eq(tester->data, ptr); //with every push, check if data has been entered to the stack
        ptr = ptr + sizeof(int);
    }
    ck_assert_ptr_nonnull(s->prev); //shows that the stack is non empty
}
END_TEST

START_TEST(test_pop)
{
    rpn_stack_t *s = rpn_stack_new();
    char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'}; //testing different datatype
    void *ptr;
    ptr = arr;
    for (int i = 0; i < 10; i++)
    {
        rpn_stack_push(s, ptr);
        ptr = ptr + sizeof(char); //ptr now at the tail of the array
    }
    void *popper;
    for (int i = 0; i < 10; i++)
    {
        popper = rpn_stack_pop(s);
        ptr = ptr - sizeof(char);      //ptr now at the tail of the array
        ck_assert_ptr_eq(popper, ptr); //check that cthe first element popped is last element pushed
    }
    //ck_assert_ptr_nonnull(s->prev);
    rpn_stack_del(s);
    ck_assert_ptr_null(s->prev); //check if the handle points to NULL
}
END_TEST

START_TEST(test_peek)
{
    rpn_stack_t *s = rpn_stack_new();
    float arr[] = {1.5, 2.4, 3.5, 4.4, 5.5, 6.4, 7.5};
    void *ptr;
    ptr = arr;
    void *peeker;
    for (int i = 0; i < 7; i++)
    {
        rpn_stack_push(s, ptr);
        peeker = rpn_stack_peek(s);
        ck_assert_ptr_eq(peeker, ptr);
        ptr = ptr + sizeof(float); //ptr now at the tail of the array
    }
}
END_TEST

START_TEST(test_empty)
{
    rpn_stack_t *s = rpn_stack_new();
    int result = rpn_stack_empty(s);
    ck_assert_int_eq(result, 1);
    double arr[] = {1.534, 2.454, 3.523, 4.445, 5.565, 6.465, 7.534};
    void *ptr;
    ptr = arr;
    for (int i = 0; i < 7; i++)
    {
        rpn_stack_push(s, ptr);
        result = rpn_stack_empty(s);
        ck_assert_int_eq(result, 0);
        ptr = ptr + sizeof(double); //ptr now at the tail of the array
    }
}
END_TEST

START_TEST(test_delete)
{
    rpn_stack_t *s = rpn_stack_new();
    int result = rpn_stack_empty(s);
    ck_assert_int_eq(result, 1);
    double arr[] = {1.534, 2.454, 3.523, 4.445, 5.565, 6.465, 7.534};
    void *ptr;
    ptr = arr;
    for (int i = 0; i < 7; i++)
    {
        rpn_stack_push(s, ptr);
        ptr = ptr + sizeof(double); //ptr now at the tail of the array
    }
    result = rpn_stack_empty(s);
    ck_assert_int_eq(result, 0);
    rpn_stack_del(s);
    result = rpn_stack_empty(s);
    ck_assert_int_eq(result, 1);
}
END_TEST

static Suite *
stack_suite(void)
{
    Suite *s;
    TCase *tc_new, *tc_push, *tc_pop, *tc_empty, *tc_delete, *tc_peek;

    s = suite_create("all");
    tc_new = tcase_create("new");
    tc_push = tcase_create("push");
    tc_pop = tcase_create("pop");
    tc_empty = tcase_create("empty");
    tc_peek = tcase_create("peek");
    tc_delete = tcase_create("delete");

    tcase_add_test(tc_new, test_new);
    suite_add_tcase(s, tc_new);
    tcase_add_test(tc_push, test_push);
    suite_add_tcase(s, tc_push);
    tcase_add_test(tc_pop, test_pop);
    suite_add_tcase(s, tc_pop);
    tcase_add_test(tc_empty, test_empty);
    suite_add_tcase(s, tc_empty);
    tcase_add_test(tc_peek, test_peek);
    suite_add_tcase(s, tc_peek);
    tcase_add_test(tc_delete, test_delete);
    suite_add_tcase(s, tc_delete);

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
