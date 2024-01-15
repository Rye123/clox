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

  // Ensure EOF
  LLNode* eofNode = scanner->tokens->tail;
  TokenType eofType = ((Token*) eofNode->data)->type;
  ck_assert_msg(eofType == TOKEN_EOF, "type of last token expected to be EOF, was instead %d", (int) eofType);

  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_doubletoken){
  const char *source = "! != = == < <= > >=";
  const TokenType expected[] = {TOKEN_BANG, TOKEN_BANG_EQUAL, TOKEN_EQUAL, TOKEN_EQUAL_EQUAL, TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_GREATER, TOKEN_GREATER_EQUAL, TOKEN_EOF};
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  // Ensure number of tokens is correct
  ck_assert_msg(LinkedListLength(scanner->tokens) == 9, "number of tokens expected to be 9, was instead %d", LinkedListLength(scanner->tokens));

  // Loop through tokens to validate
  LLNode* curNode = scanner->tokens->head;
  for (int i = 0; i < LinkedListLength(scanner->tokens); i++) {
    TokenType curType = ((Token*) curNode->data)->type;
    ck_assert_msg(curType == expected[i], "type of tokens[%d] expected to be %d, was instead %d", i, (int) expected[i], (int) curType);
    curNode = curNode->next;
  }

  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_syntaxerr){
  const char *source = "@#&";
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  // Ensure scanner tokens is empty
  int tokenCount = LinkedListLength(scanner->tokens);
  ck_assert_msg(tokenCount == 1, "expected 1 token (EOF), instead tokenCount = %d", tokenCount);
  
  // Ensure EOF
  LLNode* eofNode = scanner->tokens->tail;
  TokenType eofType = ((Token*) eofNode->data)->type;
  ck_assert_msg(eofType == TOKEN_EOF, "type of last token expected to be EOF, was instead %d", (int) eofType);

  // Ensure all errors
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 3, "expected 3 errors, instead errCount = %d", errCount);
  
  ScannerDelete(scanner);
}
END_TEST

Suite *token_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Scanner");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_singletoken);
  tcase_add_test(tc_core, test_doubletoken);
  tcase_add_test(tc_core, test_syntaxerr);
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
