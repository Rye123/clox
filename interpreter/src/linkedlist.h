#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct LLNode {
  void* data;
  struct LLNode* next;
} LLNode;

typedef struct {
  int size;
  LLNode* head;
  LLNode* tail;
} LinkedList;

// Create a new linkedlist
LinkedList* LinkedListNew(int dataSize);

// Append data to the linked list
void LinkedListAppend(LinkedList* ll, void* data);

// Pop data from the end of the linked list
void* LinkedListPop(LinkedList* ll);

// Free the memory associated with the entire linked list.
void LinkedListDelete(LinkedList* ll);

// Returns the size of the linked list.
int LinkedListLength(LinkedList* ll);

#endif
