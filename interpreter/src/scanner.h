#ifndef _SCANNER_H_
#define _SCANNER_H_
#include "token.h"
#include "linkedlist.h"

typedef struct {
  const char* source;
  const int length;
  LinkedList* tokens;

  int startIdx; // The start index of the lexeme to be scanned
  int curIdx;   // The current index of the scanner
  int curLine;
} Scanner;

// Defines a new Scanner, given the source code and the size of the source code.
Scanner* ScannerNew(const char* source, const int length);

// Scans through code, saving the tokens in the tokens attribute.
void ScannerScan(Scanner* scanner);

// Frees the memory associated with the scanner.
void ScannerDelete(Scanner* scanner);

#endif
