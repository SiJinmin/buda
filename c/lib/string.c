#include "../includes/buda.h"

int get_token_by_char_end(char** start, char end, char** token)
{
  char* old=*start; char* p=old; char c;
  while (c=*p)
  {
    if (end == c) { *token=old; *p = 0; *start = p+1; return 0; }
    else if (c == '\r') goto fail;
    else p++;
  }
fail:
  *token=NULL; return -1;
}