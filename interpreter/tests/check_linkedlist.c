#include <stdlib.h>
#include <check.h>
#include "../src/linkedlist.h"

START_TEST(test_basic_linkedlist) {
  LinkedList *ll = LinkedListNew(sizeof(int));
  ck_assert_msg(LinkedListLength(ll) == 0, "size of ll: expected 0, was %d",
                LinkedListLength(ll));

  int value1 = 5;
  LinkedListAppend(ll, &value1);
  int data = (int)*((int *)ll->head->data);
  ck_assert_msg(LinkedListLength(ll) == 1, "size of ll: expected 1, was %d",
                LinkedListLength(ll));
  ck_assert_msg(data == value1, "ll[0]: expected %d, was %d", value1, data);

  int popped = (int)*((int *) LinkedListPop(ll) );
  ck_assert_msg(LinkedListLength(ll) == 0, "size of ll: expected 0, was %d",
                LinkedListLength(ll));
  ck_assert_msg(data == popped, "popped data: expected %d, was %d", value1, popped);
  LinkedListDelete(ll);
}
END_TEST

START_TEST(test_complex_linkedlist) {
  LinkedList *ll = LinkedListNew(sizeof(int));
  int value1 = 1;
  int value2 = 2;
  int value3 = 3;
  int value4 = 4;
  LinkedListAppend(ll, &value1);
  LinkedListAppend(ll, &value2);
  LinkedListAppend(ll, &value3);
  LinkedListAppend(ll, &value4);
  ck_assert_msg(LinkedListLength(ll) == 4, "size of ll: expected 4, was %d", LinkedListLength(ll));
  LLNode* test_node = ll->head->next->next;
  int test_value = (int) *((int *) test_node->data);
  ck_assert_msg(test_value == value3, "ll[2]: expected %d, was %d", test_value, value3);
  int data = (int) *((int *) ll->tail->data);
  ck_assert_msg(data == value4, "tail: expected %d, was %d", data, value4);

  LinkedListDelete(ll);
}
END_TEST

START_TEST(test_multi_pops) {
  int test_count = 100;
  int* list_of_ints = malloc(test_count * sizeof(int));
  int curSize = 0;
  LinkedList *ll = LinkedListNew(sizeof(int));
  LLNode* curNode = NULL;
  for (int i = 0; i < test_count; i++) {
    list_of_ints[i] = i;
    LinkedListAppend(ll, &list_of_ints[i]);
    curSize++;
    if (i == 0) {
      curNode = ll->head;
    } else {
      curNode = curNode->next;
    }
    int curVal = (int) *((int*) curNode->data);
    ck_assert_msg(curVal == i, "ll[%d]: expected %d, was %d", i, curVal, i);
    ck_assert_msg(LinkedListLength(ll) == curSize, "size of ll: expected %d, was %d", curSize, LinkedListLength(ll));
  }

  for (int j = curSize - 1; j >= 0; j--) {
    int data = (int) *((int *) LinkedListPop(ll) );
    curSize--;
    ck_assert_msg(LinkedListLength(ll) == curSize, "size of ll: expected %d, was %d", curSize, LinkedListLength(ll));
    ck_assert_msg(data == j, "popped ll[%d]: expected %d, was %d", curSize, j, data);
  }
  LinkedListDelete(ll);
  free(list_of_ints);
}

Suite* token_suite(void)
{
  Suite* s;
  TCase* tc_core;

  s = suite_create("LinkedList");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_basic_linkedlist);
  tcase_add_test(tc_core, test_complex_linkedlist);
  tcase_add_test(tc_core, test_multi_pops);
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
