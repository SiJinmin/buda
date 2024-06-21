#include "../includes/buda.h"

namespace BUDA
{
	
int get_token_by_char_end(char** start, char** token, char end, char line_end)
{
  char *old=*start; char* p=old; char c;
  while (c=*p)
  {
    if (end == c) { *token=old; *p = 0; *start = p+1; return 0; }
    else if (c == line_end) goto fail;
    else { p++; }
  }
fail:
  *token=NULL; return -1;
}




int hex2dec(char c)
{
  if ('0' <= c && c <= '9') return c - '0';
  else if ('a' <= c && c <= 'f') return c - 'a' + 10;
  else if ('A' <= c && c <= 'F') return c - 'A' + 10;
  else return -1;
}
 
char dec2hex(short int c)
{
  if (0 <= c && c <= 9) return c + '0';
  else if (10 <= c && c <= 15) return c + 'A' - 10;
  else return -1;
}
 
 
#define BudaMaxLen(max_len, dec, fail, source) if((max_len-=dec)<0) { printf("dest buf is full for %s\n", source); goto fail; }

int url_encode(u_char *s, u_char *d, int max_len)
{
  u_char c, c_high, c_low, *ps = s, *pd=d; 
  while(c=*(ps++))
  {
    if(isalnum(c) || c == '/' || c == '.'|| c == '-' || c == '_'|| c == '~') { BudaMaxLen(max_len, 1, fail, s); *(pd++) = c; }
    else if (c == ' ') { BudaMaxLen(max_len, 1, fail, s); *(pd++) = '+'; }
    else 
    {
      BudaMaxLen(max_len, 3, fail, s);
      *(pd++) = '%'; *(pd++) = HEXS[c >> 4]; *(pd++) = HEXS[c & LOW_BYTE_MASK]; 
    }
  }
  *pd=0;

  succeed: return 0;
  fail:return -1;
}


int url_decode(u_char *s, u_char *d, int max_len)
{
  u_char c, *ps = s, *pd=d, c_high, c_low;
  while(c=*(ps++))
  {
    BudaMaxLen(max_len, 1, fail, s); 
    if (c == '+') *(pd++) = ' ';    
    else if (c != '%') *(pd++) = c;
    else
    {
      c = *(ps++); c_high=BudaHex2Dec(c); c = *(ps++); c_low=BudaHex2Dec(c); 
      c = *(pd++) = (c_high<<4) + c_low; //printf("decoded ");
    }
  }
  *pd=0;

  succeed: return 0;
  fail:return -1;
}



}
