#include "../includes/buda.h"

namespace BUDA
{

int vsnprintf2 (char *s, size_t size, const char *format, va_list args)  
{
  int len = vsnprintf(s, size, format, args); 
  if(len<0 || len>=size) { log("vsnprintf2 failed for %s, return len = %d, max = %d", format, len, (int)size); return -1; }
  return len;
}
int snprintf2 (char *s, size_t size, const char *format, ...)  
{
  int len; va_list args; va_start(args, format); len = vsnprintf2(s, size, format, args); va_end(args);
  return len;
}


long get_long_int(char **ppc, char *end1)
{
  char *pc=*ppc, c=*pc; long r=0; bool neg=false;  
  if(c=='+') pc++; else if(c=='-') {neg=true; pc++;} 
  while((pc<end1) && (c=*pc))
  {
    if(c>='0' && c<='9') { r=(r*10)+(c-'0'); ++pc; } else goto succeed;
  }
  goto fail;

  succeed: *ppc=pc; return neg ? -r: r;
  fail: *ppc=end1; return 0;
}
char* convert_escaped_string(char **ppc, char *end1)
{
  char c, *pc=*ppc; c=*pc; if(c!='"') return 0; ++pc; char *start=pc, *next=pc;
  while((pc<end1) && (c=*pc))
  {
    if(c=='"') {*next=0; pc++; goto succeed;} 
    else if(c=='\\')
    {   
      c=*(++pc); if(pc>=end1) goto fail;
      if(c=='n'){ *(next++)='\n';  }
      else if(c=='t'){ *(next++)='\t'; }
      else if(c=='b'){ *(next++)='\b'; }
      else if(c=='\''){ *(next++)='\''; }
      else if(c=='"'){ *(next++)='"'; }
      else if(c=='\\'){ *(next++)='\\'; }
      else if(c=='r'){ }
      else{ *(next++)=c; }
    }
    else{ *(next++)=c; }
    ++pc;
  }
  goto fail;

  succeed: *ppc=pc; return start;
  fail: *ppc=end1; return 0;
}
char next_char_not(char **ppc, char* end1, const char *scope)
{
  char c, *pc=*ppc;  while((pc<end1) && (c=*pc))
  {
    if(NULL == strchr(scope, c)) goto succeed; else ++pc;
  }
  goto fail;

  succeed: *ppc=pc; return c;
  fail: *ppc=end1; return 0;
}
char next_char(char **ppc, char* end1, const char *scope)
{
  char c, *pc=*ppc; while((pc<end1) && (c=*pc) )
  {
    if(strchr(scope, c)) goto succeed; else ++pc;
  }
  goto fail;

  succeed: *ppc=pc; return c;
  fail: *ppc=end1; return 0;
}


int compile_regex(char *pattern, regex_t *regex) 
{
  if(regcomp(regex, pattern, REG_EXTENDED)) { log("Could not compile regex: %s", pattern); goto fail; }

  succeed: return 0;
  fail: regfree(regex); return -1;
}
int search_first(char *content, regex_t *regex, char **startPtr, char **endPtr) 
{
  int r; regmatch_t match; int match_count=0;
  r = regexec(regex, content, 1, &match, 0);
  if (r==0) { match_count=1; if(startPtr) *startPtr = content+match.rm_so; if(endPtr) *endPtr = content + match.rm_eo; } 

  succeed: return match_count;
  fail: return -1;
}
int search_first(char *content, char *pattern, char **startPtr, char **endPtr) 
{
  int r; regex_t regex; regmatch_t match; int match_count=0;
  r = regcomp(&regex, pattern, REG_EXTENDED); if (r) { log("Could not compile regex: %s", pattern); goto fail; }
  r = regexec(&regex, content, 1, &match, 0); regfree(&regex); 
  if (r==0) { match_count=1; if(startPtr) *startPtr = content+match.rm_so; if(endPtr) *endPtr = content + match.rm_eo; } 

  succeed: return match_count;
  fail: regfree(&regex); return -1;
}


char* set_next_space_0(char *pc, char *pc_end)
{
  char c; while( (c=*pc) &&  pc<pc_end)
  {
    if(c==' ' || c=='\r' || c=='\n') {*pc=0; return ++pc;} else pc++;
  }
  *pc_end=0; return ++pc_end;
}
char* next_line(char *pc)
{
  char c; int found=false;
  while(c=*pc)
  {
    if(c=='\r' || c=='\n') { found=true; pc++; } else { if(found) return pc; else pc++; }
  }
  return NULL;
}
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
int url_encode(u_char *s, u_char *d, int max_len)
{
  u_char c, c_high, c_low, *ps = s, *pd=d; 
  while(c=*(ps++))
  {
    if(strchr(URL_ALLOW, c)) { BudaPreWrite1(max_len, fail); *(pd++) = c; }
    else { BudaPreWrite(max_len, 3, fail); *(pd++) = '%'; *(pd++) = HEXS[c >> 4]; *(pd++) = HEXS[c & BYTE_LOW4_MASK]; }
  }
  *pd=0;

  succeed: return 0;
  fail: log("url_encode failed"); return -1;
}
int url_decode(u_char *s, u_char *d, int max_len)
{
  u_char c, *ps=s, *pd=d, c_high, c_low;
  while(c=*(ps++))
  {
    BudaPreWrite1(max_len, fail); 
    if (c != '%') *(pd++) = c;
    else
    {
      c = *(ps++); c_high=BudaHex2Dec(c); c = *(ps++); c_low=BudaHex2Dec(c); 
      c = *(pd++) = ((c_high<<4) | c_low); //printf("decoded ");
    }
  }
  *pd=0;

  succeed: return 0;
  fail: log("url_decode failed"); return -1;
}


}
