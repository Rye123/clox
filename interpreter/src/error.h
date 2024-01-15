#ifndef _ERROR_H_
#define _ERROR_H_

typedef struct {
  const int line;  // Line number of the error
  const int srcIndex;  // Position in the source code
  const char* errMsg;
} Error;

Error* ErrorNew(int line, int srcIndex, const char* msg);

void ErrorDelete(Error* error);

#endif
