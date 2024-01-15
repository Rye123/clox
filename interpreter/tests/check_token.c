#include <stdlib.h>
#include <check.h>
#include "../src/token.h"

START_TEST (test_newtoken)
{
  Token* num_token = TokenNew(TOKEN_NUMBER, "42", 2, 1);
  ck_assert_msg((num_token->literal.literal_int == 42), "Expected numerical literal to be 42, was instead %d", num_token->literal.literal_int);
  Token* str_token = TokenNew(TOKEN_STRING, "asdf", 4, 1);
  ck_assert_msg(strcmp(str_token->literal.literal_str, "asdf") == 0, "Expected string literal to be \"asdf\", was instead %s", str_token->literal.literal_str);

  TokenDelete(num_token); TokenDelete(str_token);
}
END_TEST

Suite* token_suite(void)
{
  Suite* s;
  TCase* tc_core;

  s = suite_create("Token");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_newtoken);
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
