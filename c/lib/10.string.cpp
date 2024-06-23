#include "../includes/buda.h"

namespace BUDA
{

int log_start()
{
  clock_gettime(CLOCK_MONOTONIC, &last_log_time);

  char relative_path[PATH_MAX], real_path[PATH_MAX], *c=relative_path; int remain=PATH_MAX_1, len; 
  len = snprintf(c, remain, "%s", log_dir); BudaWriteStep2(c, len, remain);
  *c=0; c = realpath(relative_path, real_path); if(c!=real_path) { printf("get log dir real path error: %s\n", relative_path); goto fail; }

  c=real_path; remain=PATH_MAX_1; len=strlen(c); BudaWriteStep2(c, len, remain);
  len = snprintf(c, remain, "/"); BudaWriteStep2(c, len, remain);  
  len = time_text_filename(c, remain); BudaWriteStep2(c, len, remain); 
  len = snprintf(c, remain, ".txt"); BudaWriteStep2(c, len, remain);  
  *c=0; log_file = fopen(real_path, "ab"); if (log_file == NULL) { printf("Failed to open file: %s\n", real_path); goto fail; }
  printf("created log file: %s\n", real_path);

  succeed: return 0;
  fail: return -1;
}

void log(const char *format, ...)
{
  char buf[TIME_BUF_SIZE]; long len, sep_sec, sep_nsec; 
  len = time_text(buf, TIME_BUF_SIZE1); buf[len]=0; struct timespec log_time; clock_gettime(CLOCK_REALTIME, &log_time); 
  sep_sec=log_time.tv_sec-last_log_time.tv_sec; sep_nsec=log_time.tv_nsec-last_log_time.tv_nsec; if(sep_nsec<0) { sep_nsec+=1000000000; sep_sec--; }
  last_log_time = log_time;
  fprintf(log_file, "[%s][%ld.%ld] ", buf, sep_sec, sep_nsec); 
  va_list args; va_start(args, format); vfprintf(log_file, format, args); va_end(args); fflush(log_file);
  va_start(args, format); vprintf(format, args); va_end(args);
  fprintf(log_file, "\n"); printf("\n");

  succeed: return;
  fail: printf("log failed\n"); return;
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
    if(isalnum(c) || c == '/' || c == '.'|| c == '-' || c == '_'|| c == '~') { BudaPreWrite(max_len, 1, fail); *(pd++) = c; }
    else if (c == ' ') { BudaPreWrite(max_len, 1, fail); *(pd++) = '+'; }
    else 
    {
      BudaPreWrite(max_len, 3, fail);
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
    BudaPreWrite(max_len, 1, fail); 
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
