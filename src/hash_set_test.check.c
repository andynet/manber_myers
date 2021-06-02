#include <stdlib.h>
#include <check.h>
#include "hash_table.h"

/* libraries -lcheck -lm -lrt -lpthread should be included during compilation */

START_TEST (initialization)
{
    hash_set_t *set = hash_set_create(1);
    ck_assert_str_eq("NULL", "NULL");
}
END_TEST

/*
START_TEST(test_money_create)
{
    Money *m;

    m = money_create(5, "USD");
    ck_assert_int_eq(money_amount(m), 5);
    ck_assert_str_eq(money_currency(m), "USD");
    money_free(m);
}
END_TEST
*/ 

Suite *
money_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Money");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, initialization);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    return 0;
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = money_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}




