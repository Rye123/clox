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

  TokenDelete(num_token); TokenDelete(num_token2); TokenDelete(num_token3);
  TokenDelete(str_token);
}
END_TEST

START_TEST (test_oobtoken)
{
  char buf[400];
  snprintf(buf, 400, "1%f", DBL_MAX);
  Token* num_token = TokenNew(TOKEN_NUMBER, buf, strlen(buf), 1);
  ck_assert_msg(errno == ERANGE, "Expected errno to be set to ERANGE, but instead err_no=%d", errno);
  TokenDelete(num_token);
  snprintf(buf, 400, "-1%f", DBL_MAX);
  num_token = TokenNew(TOKEN_NUMBER, buf, strlen(buf), 1);
  ck_assert_msg(errno == ERANGE, "Expected errno to be set to ERANGE, but instead err_no=%d", errno);
  TokenDelete(num_token);
}

START_TEST (test_matchkeyword)
{
  ck_assert_int_eq(TokenMatchKeyword("and", 3), TOKEN_AND);
  ck_assert_int_eq(TokenMatchKeyword("an", 2), TOKEN_IDENTIFIER);
  ck_assert_int_eq(TokenMatchKeyword("or", 2), TOKEN_OR);
  ck_assert_int_eq(TokenMatchKeyword("if", 2), TOKEN_IF);
  ck_assert_int_eq(TokenMatchKeyword("i", 1), TOKEN_IDENTIFIER);
  ck_assert_int_eq(TokenMatchKeyword("else", 4), TOKEN_ELSE);
  ck_assert_int_eq(TokenMatchKeyword("return", 6), TOKEN_RETURN);
  ck_assert_int_eq(TokenMatchKeyword("nil", 3), TOKEN_NIL);
  ck_assert_int_eq(TokenMatchKeyword("print", 5), TOKEN_PRINT);
  ck_assert_int_eq(TokenMatchKeyword("asdf", 4), TOKEN_IDENTIFIER);
}

Suite* token_suite(void)
{
  Suite* s;
  TCase* tc_core;

  s = suite_create("Token");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_newtoken);
  tcase_add_test(tc_core, test_oobtoken);
  tcase_add_test(tc_core, test_matchkeyword);
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
