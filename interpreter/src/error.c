#include <stdlib.h>
#include <string.h>
#include "error.h"

Error* ErrorNew(int line, int srcIndex, const char* msg)
{
  Error* ret = malloc(sizeof(Error));
  Error tempErr = {line, srcIndex, msg};
  memcpy(ret, &tempErr, sizeof(Error));

  return ret;
}

void ErrorDelete(Error* err)
{
  free(err);
}
