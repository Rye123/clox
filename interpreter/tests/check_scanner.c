#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <check.h>
#include "../src/linkedlist.h"
#include "../src/token.h"
#include "../src/error.h"
#include "../src/scanner.h"

START_TEST(test_singletoken){
  const char *source = "(){},.-+;*{*}(,.)),-/+/";
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
    case '/': ck_assert_msg(curType == TOKEN_SLASH, "type of '/' was %d", (int) curType); break;
    }
    curNode = curNode->next;
  }

  // Ensure EOF
  LLNode* eofNode = scanner->tokens->tail;
  TokenType eofType = ((Token*) eofNode->data)->type;
  ck_assert_msg(eofType == TOKEN_EOF, "type of last token expected to be EOF, was instead %d", (int) eofType);

  // Ensure no syntax errors
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 0, "expected no errors, instead found %d errors", errCount);

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

  // Ensure no syntax errors
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 0, "expected no errors, instead found %d errors", errCount);

  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_comments){
  const char *source = "/ != // the quick brown fox jumps over the lazy dog.\n! / ! // rest of the comments\n=";
  const TokenType expected[] = {TOKEN_SLASH, TOKEN_BANG_EQUAL, TOKEN_BANG, TOKEN_SLASH, TOKEN_BANG, TOKEN_EQUAL, TOKEN_EOF};
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  // Ensure number of tokens is correct
  ck_assert_msg(LinkedListLength(scanner->tokens) == 7, "number of tokens expected to be 7, was instead %d", LinkedListLength(scanner->tokens));

  // Loop through tokens to validate
  LLNode* curNode = scanner->tokens->head;
  for (int i = 0; i < LinkedListLength(scanner->tokens); i++) {
    TokenType curType = ((Token*) curNode->data)->type;
    ck_assert_msg(curType == expected[i], "type of tokens[%d] expected to be %d, was instead %d", i, (int) expected[i], (int) curType);
    curNode = curNode->next;
  }

  // Ensure no syntax errors
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 0, "expected no errors, instead found %d errors", errCount);

  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_whitespace){
  const char *source = "   \n   \n   /@";
  const TokenType expected[] = {TOKEN_SLASH, TOKEN_EOF};
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  // Ensure number of lines is correct
  ck_assert_msg(scanner->curLine == 2, "expected curLine to be 2, was instead %d", scanner->curLine);

  // Ensure number of tokens is correct
  ck_assert_msg(LinkedListLength(scanner->tokens) == 2, "number of tokens expected to be 2, was instead %d", LinkedListLength(scanner->tokens));

  // Validate token
  LLNode* curNode = scanner->tokens->head;
  TokenType curType = ((Token*) curNode->data)->type;
  ck_assert_msg(curType == expected[0], "expected type to be %d, was instead %d", (int) expected[0], (int) curType);

  int lineNum = ((Token*) curNode->data)->lineNum;
  ck_assert_msg(lineNum == 2, "expected linenum of slash to be 2, was instead %d", lineNum);

  // Ensure 1 syntax error
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 1, "expected 1 error, instead found %d errors", errCount);

  // Ensure error line number is correct
  LLNode* errNode = scanner->errors->head;
  int linenum = ((Error*) errNode->data)->line;
  int errpos = ((Error*) errNode->data)->srcIndex;
  ck_assert_msg(linenum == 2, "expected error line to be 2, was instead %d", linenum);
  ck_assert_msg(errpos == 12, "expected error pos to be 12, was instead %d", errpos);

  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_simplestring){
  const char *source = "\"asdf\" \"The quick brown fox\" \n\"@#&+-\"";
  const TokenType expected[] = {TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_EOF};
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  // Ensure number of lines is correct
  ck_assert_msg(scanner->curLine == 1, "expected curLine to be 1, was instead %d", scanner->curLine);

  // Ensure number of tokens is correct
  ck_assert_msg(LinkedListLength(scanner->tokens) == 4, "number of tokens expected to be 4, was instead %d", LinkedListLength(scanner->tokens));

  // Validate token types
  LLNode* curNode = scanner->tokens->head;
  for (int i = 0; i < LinkedListLength(scanner->tokens); i++) {
    TokenType curType = ((Token*) curNode->data)->type;
    ck_assert_msg(curType == expected[i], "type of tokens[%d] expected to be %d, was instead %d", i, (int) expected[i], (int) curType);
    curNode = curNode->next;
  }

  // Validate token contents
  curNode = scanner->tokens->head;
  // 1. asdf
  char* nodestr = ((Token*) curNode->data)->literal.literal_str;
  int lineNum = ((Token*) curNode->data)->lineNum;
  ck_assert_msg(strcmp(nodestr, "asdf") == 0, "expected \"asdf\", instead got \"%s\"", nodestr);
  ck_assert_msg(lineNum == 0, "expected linenum of \"asdf\" to be 0, was instead %d", lineNum);
  curNode = curNode->next;
  // 2. The quick brown fox
  nodestr = ((Token*) curNode->data)->literal.literal_str;
  lineNum = ((Token*) curNode->data)->lineNum;
  ck_assert_msg(strcmp(nodestr, "The quick brown fox") == 0, "expected \"The quick brown fox\", instead got \"%s\"", nodestr);
  ck_assert_msg(lineNum == 0, "expected linenum of \"The quick brown fox\" to be 0, was instead %d", lineNum);
  curNode = curNode->next;

  // 3. @#&+-
  nodestr = ((Token*) curNode->data)->literal.literal_str;
  lineNum = ((Token*) curNode->data)->lineNum;
  ck_assert_msg(strcmp(nodestr, "@#&+-") == 0, "expected \"@#&+-\", instead got \"%s\"", nodestr);
  ck_assert_msg(lineNum == 1, "expected linenum of \"@#&+-\" to be 0, was instead %d", lineNum);
  curNode = curNode->next;

  // Ensure no syntax errors
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 0, "expected no errors, instead found %d errors", errCount);
  
  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_multilinestring){
  const char *source = "\"asdf\nThequickbrownfox\nPeter Piper\n\"+-/";
  const TokenType expected[] = {TOKEN_STRING, TOKEN_PLUS, TOKEN_MINUS, TOKEN_SLASH, TOKEN_EOF};
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  // Ensure number of lines is correct
  ck_assert_msg(scanner->curLine == 3, "expected curLine to be 3, was instead %d", scanner->curLine);

  // Ensure number of tokens is correct
  ck_assert_msg(LinkedListLength(scanner->tokens) == 5, "number of tokens expected to be 5, was instead %d", LinkedListLength(scanner->tokens));

  // Validate token types
  LLNode* curNode = scanner->tokens->head;
  for (int i = 0; i < LinkedListLength(scanner->tokens); i++) {
    TokenType curType = ((Token*) curNode->data)->type;
    ck_assert_msg(curType == expected[i], "type of tokens[%d] expected to be %d, was instead %d", i, (int) expected[i], (int) curType);
    curNode = curNode->next;
  }

  // Validate token contents
  curNode = scanner->tokens->head;
  // asdf\nThequickbrownfox\nPeter Piper
  char* nodestr = ((Token*) curNode->data)->literal.literal_str;
  int lineNum = ((Token*) curNode->data)->lineNum;
  ck_assert_msg(strcmp(nodestr, "asdf\nThequickbrownfox\nPeter Piper\n") == 0, "expected \"asdf\\nThequickbrownfox\\nPeter Piper\\n\", instead got \"%s\"", nodestr);
  ck_assert_msg(lineNum == 3, "expected linenum to be 3, was instead %d", lineNum);

  // Ensure no syntax errors
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 0, "expected no errors, instead found %d errors", errCount);
  
  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_unterminatedstring){
  const char *source = "\"The quick brown fox\nleaps over the lazy doggo+-";
  const TokenType expected[] = {TOKEN_EOF};
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  // Ensure number of tokens is correct
  ck_assert_msg(LinkedListLength(scanner->tokens) == 1, "number of tokens expected to be 1, was instead %d", LinkedListLength(scanner->tokens));

  // Ensure 1 syntax error
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 1, "expected 1 error, instead found %d errors", errCount);
  
  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_simplenumber){
  const char *source = "1 2 3";
  const TokenType expected[] = {TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_EOF};
  const double expectedNums[] = {1, 2, 3};
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  // Validate token types
  LLNode* curNode = scanner->tokens->head;
  for (int i = 0; i < LinkedListLength(scanner->tokens); i++) {
    TokenType curType = ((Token*) curNode->data)->type;
    double curValue = ((Token*) curNode->data)->literal.literal_num;

    // Validate type
    ck_assert_msg(curType == expected[i], "type of tokens[%d] expected to be %d, was instead %d", i, (int) expected[i], (int) curType);

    // Validate value
    if (curType == TOKEN_NUMBER)
      ck_assert_msg(curValue == expectedNums[i], "tokens[%d] expected to be %f, was instead %f", i, expectedNums[i], curValue);
    
    curNode = curNode->next;
  }

  // Ensure no syntax errors
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 0, "expected no errors, instead found %d errors", errCount);
  
  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_negativenumber){
  const char *source = "-1 2 -3";
  const TokenType expected[] = {TOKEN_MINUS, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_MINUS, TOKEN_NUMBER, TOKEN_EOF};
  const double expectedNums[] = {1, 2, 3};
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  // Validate token types
  LLNode* curNode = scanner->tokens->head;
  int test_number = 0;
  for (int i = 0; i < LinkedListLength(scanner->tokens); i++) {
    TokenType curType = ((Token*) curNode->data)->type;
    double curValue = ((Token*) curNode->data)->literal.literal_num;

    // Validate type
    ck_assert_msg(curType == expected[i], "type of tokens[%d] expected to be %d, was instead %d", i, (int) expected[i], (int) curType);

    // Validate value
    if (curType == TOKEN_NUMBER) {
      ck_assert_msg(curValue == expectedNums[test_number],
                    "tokens[%d] expected to be %f, was instead %f", i,
                    expectedNums[test_number], curValue);
      test_number++;
    }

    curNode = curNode->next;
  }

  // Ensure no syntax errors
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 0, "expected no errors, instead found %d errors", errCount);
  
  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_positivefloat){
  const char *source = "3.141592654 2.718281 6.69 0.0";
  const TokenType expected[] = {TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_EOF};
  const double expectedNums[] = {3.141592654, 2.718281, 6.69, 0.0};
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  LLNode* curNode = scanner->tokens->head;
  int test_number = 1;
  for (int i = 0; i < LinkedListLength(scanner->tokens); i++) {
    TokenType curType = ((Token*) curNode->data)->type;
    double curValue = ((Token*) curNode->data)->literal.literal_num;

    // Validate type
    ck_assert_msg(curType == expected[i], "type of tokens[%d] expected to be %d, was instead %d", i, (int) expected[i], (int) curType);

    // Validate value
    if (curType == TOKEN_NUMBER)
      ck_assert_msg(curValue == expectedNums[i], "tokens[%d] expected to be %f, was instead %f", i, expectedNums[i], curValue);
    
    curNode = curNode->next;
  }

  // Ensure no syntax errors
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 0, "expected no errors, instead found %d errors", errCount);
  
  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_negativefloat){
  const char *source = "-3.141592654 -2.718281 -6.69";
  const TokenType expected[] = {TOKEN_MINUS, TOKEN_NUMBER, TOKEN_MINUS, TOKEN_NUMBER, TOKEN_MINUS, TOKEN_NUMBER, TOKEN_EOF};
  const double expectedNums[] = {3.141592654, 2.718281, 6.69};
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  LLNode* curNode = scanner->tokens->head;
  int test_number = 0;
  for (int i = 0; i < LinkedListLength(scanner->tokens); i++) {
    TokenType curType = ((Token*) curNode->data)->type;
    double curValue = ((Token*) curNode->data)->literal.literal_num;

    // Validate type
    ck_assert_msg(curType == expected[i], "type of tokens[%d] expected to be %d, was instead %d", i, (int) expected[i], (int) curType);

    // Validate value
    if (curType == TOKEN_NUMBER) {
      ck_assert_msg(curValue == expectedNums[test_number],
                    "tokens[%d] expected to be %f, was instead %f", i,
                    expectedNums[test_number], curValue);
      test_number++;
    }
    
    curNode = curNode->next;
  }

  // Ensure no syntax errors
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 0, "expected no errors, instead found %d errors", errCount);
  
  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_bounds){
  char source[2000];
  snprintf(source, 2000, "1%f 1%f -1%f", DBL_MAX, DBL_MAX, DBL_MAX);
  
  const TokenType expected[] = {TOKEN_MINUS, TOKEN_EOF};
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  LLNode* curNode = scanner->tokens->head;
  int test_number = 0;
  for (int i = 0; i < LinkedListLength(scanner->tokens); i++) {
    TokenType curType = ((Token*) curNode->data)->type;
    ck_assert_msg(curType == expected[i], "type of tokens[%d] expected to be %d, was instead %d", i, (int) expected[i], (int) curType);

    curNode = curNode->next;
  }

  // Ensure syntax errors for the range being exceeded
  int errCount = LinkedListLength(scanner->errors);
  ck_assert_msg(errCount == 3, "expected 3 errors, instead found %d errors", errCount);
  
  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_identifiers) {
  const char* source = "i ifconfig a an ad andy the superb sup s";
  const char* expectedVals[] = {"i", "ifconfig", "a", "an", "ad", "andy", "the", "superb", "sup", "s"};
  const TokenType expectedToks[] = {TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_EOF};
  
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  LLNode* curNode = scanner->tokens->head;
  for (int i = 0; i < LinkedListLength(scanner->tokens); i++) {
    TokenType curType = ((Token*) curNode->data)->type;
    ck_assert_msg(curType == expectedToks[i], "token[%d] expected to be type %d, was %d", i, expectedToks[i], curType);

    if (curType == TOKEN_IDENTIFIER) {
      const char* tokenVal = ((Token*) curNode->data)->lexeme;
      ck_assert_msg(strcmp(tokenVal, expectedVals[i]) == 0, "token[%d] expected to be %s, was %s", i, expectedVals[i], tokenVal);
    }
    curNode = curNode->next;
  }

  ScannerDelete(scanner);
}
END_TEST

START_TEST(test_keywords) {
  const char* source = "if true and else return super or";
  const char* expectedVals[] = {"if", "true", "and", "else", "return", "super", "or"};
  const TokenType expectedToks[] = {TOKEN_IF, TOKEN_TRUE, TOKEN_AND, TOKEN_ELSE, TOKEN_RETURN, TOKEN_SUPER, TOKEN_OR, TOKEN_EOF};
  
  Scanner *scanner = ScannerNew(source, strlen(source));
  ScannerScan(scanner);

  LLNode* curNode = scanner->tokens->head;
  for (int i = 0; i < LinkedListLength(scanner->tokens); i++) {
    TokenType curType = ((Token*) curNode->data)->type;
    ck_assert_msg(curType == expectedToks[i], "token[%d] expected to be type %d, was %d", i, expectedToks[i], curType);

    if (curType != TOKEN_EOF) {
      const char* tokenVal = ((Token*) curNode->data)->lexeme;
      ck_assert_msg(strcmp(tokenVal, expectedVals[i]) == 0, "token[%d] expected to be %s, was %s", i, expectedVals[i], tokenVal);
    }
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
  tcase_add_test(tc_core, test_comments);
  tcase_add_test(tc_core, test_whitespace);
  tcase_add_test(tc_core, test_simplestring);
  tcase_add_test(tc_core, test_multilinestring);
  tcase_add_test(tc_core, test_unterminatedstring);
  tcase_add_test(tc_core, test_simplenumber);
  tcase_add_test(tc_core, test_negativenumber);
  tcase_add_test(tc_core, test_positivefloat);
  tcase_add_test(tc_core, test_negativefloat);
  tcase_add_test(tc_core, test_bounds);
  tcase_add_test(tc_core, test_identifiers);
  tcase_add_test(tc_core, test_keywords);
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
