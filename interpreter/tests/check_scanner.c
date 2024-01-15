#include <stdlib.h>
#include <check.h>
#include "../src/linkedlist.h"
#include "../src/token.h"
#include "../src/scanner.h"

START_TEST(test_singletoken){
  const char *source = "(){},.-+;*{*}(,.)),-+";
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  // Loop through tokens to validate
  LLNode* curNode = scanner->tokens->head;
  for (int i = 0; i < strlen(source); i++) {
    TokenType curType = ((Token*) curNode->data)->type;
    char curChar = source[i];
    switch (curChar) {
    case '(': ck_assert_msg(curType == TOKEN_PAREN_L, "type of '(' was %d", (int) curType); break;
    case ')': ck_assert_msg(curType == TOKEN_PAREN_R, "type of ')' was %d", (int) curType); break;
    case '{': ck_assert_msg(curType == TOKEN_BRACE_L, "type of '{' was %d", (int) curType); break;
    case '}': ck_assert_msg(curType == TOKEN_BRACE_R, "type of '}' was %d", (int) curType); break;
    case ',': ck_assert_msg(curType == TOKEN_COMMA, "type of ',' was %d", (int) curType); break;
    case '.': ck_assert_msg(curType == TOKEN_PERIOD, "type of '.' was %d", (int) curType); break;
    case '-': ck_assert_msg(curType == TOKEN_MINUS, "type of '-' was %d", (int) curType); break;
    case '+': ck_assert_msg(curType == TOKEN_PLUS, "type of '+' was %d", (int) curType); break;
    case ';': ck_assert_msg(curType == TOKEN_SEMICOLON, "type of ';' was %d", (int) curType); break;
    case '*': ck_assert_msg(curType == TOKEN_STAR, "type of '*' was %d", (int) curType); break;
    }
    curNode = curNode->next;
  }

  ScannerDelete(scanner);
}
END_TEST

Suite *token_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Scanner");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_singletoken);
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
