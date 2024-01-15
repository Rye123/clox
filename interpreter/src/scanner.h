#ifndef _SCANNER_H_
#define _SCANNER_H_
#include "token.h"
#include "linkedlist.h"

typedef struct {
  const char* source;
  const int length;
  LinkedList* tokens;  // Linked list of Token*
  LinkedList* errors;  // Linked list of Error*

  int startIdx; // The start index of the lexeme to be scanned
  int curIdx;   // The current index of the scanner
  int curLine;
} Scanner;

// Defines a new Scanner, given the source code and the size of the source code.
Scanner* ScannerNew(const char* source, const int length);

// Scans through code, saving the tokens in the tokens attribute and any errors in the errors attribute.
void ScannerScan(Scanner* scanner);

// Frees the memory associated with the scanner.
void ScannerDelete(Scanner* scanner);

#endif
