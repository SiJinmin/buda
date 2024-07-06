#include "../includes/buda.h"

namespace BUDA
{

extern FILE* log_file;
extern struct timespec last_log_time;


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


int check_user_input_for_log(char* input)
{
  int r = search_first(input, &regex_log_time); if(r>=1) 
	{ 
		char filename[PATH_MAX]; int len; if((len=snprintf2(filename, PATH_MAX_1, "%s", log_input_dir))>0)
		{  
		  if(time_text(filename+len, PATH_MAX_1-len, 'f')>0) 
			{ file_write(filename, input, strlen(input)); log("input from user contains forbidden info: %s", filename);  }
		}
    return -1;
	}
  return 0;
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


int log_start()
{  
  int r=0; char real_path[PATH_MAX], *c=real_path; int remain=PATH_MAX_1, len; struct stat status; 
  
  r = stat(log_dir, &status); if (!(r==0 && (status.st_mode & S_IFDIR))) 
  {
    r = mkdir(log_dir, 0700); // owner can read, write and execute
    if (r) { printf("create dir fail: %s\n", log_dir); goto fail; }   
  }
  r = stat(log_input_dir, &status); if (!(r==0 && (status.st_mode & S_IFDIR))) 
  { r = mkdir(log_input_dir, 0700); if (r) { printf("create dir fail: %s\n", log_input_dir); goto fail; } }

  len = snprintf(c, remain, "%s", log_dir); BudaWriteStep2(c, len, remain);
  len = time_text(c, remain, 'f', false, true); BudaWriteStep2(c, len, remain); 
  len = snprintf(c, remain, ".txt"); BudaWriteStep2(c, len, remain);  
  log_file = fopen(real_path, "ab"); if (log_file == NULL) { printf("Failed to open file: %s\n", real_path); goto fail; }
  printf("created log file: %s\n", real_path);

  clock_gettime(CLOCK_MONOTONIC, &last_log_time);

  succeed: return 0;
  fail: return -1;
}
void log(const char *format, ...)
{
  char buf[TIME_BUF_SIZE]; int len; long sep_sec, sep_nsec; 
  len = time_text(buf, TIME_BUF_SIZE1, 't', false, true); struct timespec log_time; clock_gettime(CLOCK_MONOTONIC, &log_time); 
  sep_sec=log_time.tv_sec-last_log_time.tv_sec; sep_nsec=log_time.tv_nsec-last_log_time.tv_nsec; if(sep_nsec<0) { sep_nsec+=1000000000; sep_sec--; }
  last_log_time = log_time;
  fprintf(log_file, "[%s][%ld.%ld] ", buf, sep_sec, sep_nsec); 
  va_list args; va_start(args, format); vfprintf(log_file, format, args); va_end(args); fflush(log_file);
  va_start(args, format); vprintf(format, args); va_end(args);
  fprintf(log_file, "\n"); printf("\n");

  succeed: return;
  fail: printf("log failed\n"); return;
}


char* set_next_space_0(char *pc, char *pc_end)
{
  char c;
  while( (c=*pc) &&  pc<pc_end)
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
