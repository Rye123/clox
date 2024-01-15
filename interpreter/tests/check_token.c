#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <errno.h>
#include <check.h>
#include "../src/token.h"

START_TEST (test_newtoken)
{
  Token* num_token = TokenNew(TOKEN_NUMBER, "42", 2, 1);
  ck_assert_msg((num_token->literal.literal_num == 42.0), "Expected numerical literal to be 42, was instead %f", num_token->literal.literal_num);
  
  Token* num_token2 = TokenNew(TOKEN_NUMBER, "69.6969", 7, 1);
  ck_assert_msg((num_token2->literal.literal_num == 69.6969), "Expected numerical literal to be 69.6969, was instead %f", num_token2->literal.literal_num);
  
  Token* num_token3 = TokenNew(TOKEN_NUMBER, "-3.1415926535898", 16, 1);
  ck_assert_msg((num_token3->literal.literal_num == -3.1415926535898), "Expected numerical literal to be -3.1415926535898, was instead %f", num_token3->literal.literal_num);
  
  Token* str_token = TokenNew(TOKEN_STRING, "asdf", 4, 1);
  ck_assert_msg(strcmp(str_token->literal.literal_str, "asdf") == 0, "Expected string literal to be \"asdf\", was instead %s", str_token->literal.literal_str);

  TokenDelete(num_token); TokenDelete(str_token);
}
END_TEST

START_TEST (test_oobtoken)
{
  char buf[400];
  snprintf(buf, 400, "1%f", DBL_MAX);
  Token* num_token = TokenNew(TOKEN_NUMBER, buf, strlen(buf), 1);
  ck_assert_msg(errno == ERANGE, "Expected errno to be set to ERANGE, but instead err_no=%d", errno);
  snprintf(buf, 400, "-1%f", DBL_MAX);
  num_token = TokenNew(TOKEN_NUMBER, buf, strlen(buf), 1);
  ck_assert_msg(errno == ERANGE, "Expected errno to be set to ERANGE, but instead err_no=%d", errno);
}

Suite* token_suite(void)
{
  Suite* s;
  TCase* tc_core;

  s = suite_create("Token");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_newtoken);
  tcase_add_test(tc_core, test_oobtoken);
  suite_add_tcase(s, tc_core);
  return s;
}

int main(void)
{
  int failed_count;
  Suite* s; SRunner* sr;
  s = token_suite(); sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  failed_count = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
