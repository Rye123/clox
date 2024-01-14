#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

LinkedList* LinkedListNew(int dataSize)
{
  LinkedList* ll = malloc(sizeof(LinkedList));
  ll->size = 0;
  ll->head = NULL;
  ll->tail = NULL;
  return ll;
}

void LinkedListAppend(LinkedList* ll, void* data)
{
  // Define node
  LLNode* node = malloc(sizeof(LLNode));
  node->data = data;
  node->next = NULL;

  // Append node
  if (ll->size == 0) {
    ll->head = node;
    ll->tail = node;
    ll->size++;
  } else {
    ll->tail->next = node;
    ll->tail = node;
    ll->size++;
  }
}

void* LinkedListPop(LinkedList* ll)
{
  if (ll->size == 0)
    return NULL;
  
  // Get node value
  void* value = ll->tail->data;
  
  LLNode* curNode = ll->head;
  if (curNode == ll->tail) {
    // Head IS the tail
    ll->size--;
    ll->head = NULL; ll->tail = NULL;
    free(curNode);
    return value;
  }

  // Otherwise, iterate until we get the node directly before the tail
  for (int i = 0; i < ll->size; i++) {
    if (curNode->next == ll->tail)
      break;
    curNode = curNode->next;
  }

  free(ll->tail); curNode->next = NULL;
  ll->size--;
  ll->tail = curNode;
  return value;
}

void LinkedListDelete(LinkedList* ll)
{
  while (ll->size > 0) {
    LinkedListPop(ll);
  }
  free(ll);
}

int LinkedListLength(LinkedList* ll)
{
  return ll->size;
}
