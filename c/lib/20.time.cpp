#include "../includes/buda.h"


namespace BUDA
{	

int time_text(char *r, int max_len, char type, int use_gmt, int disable_log)
{  
  static const char* format_error="return len error(%d/%d) for %s\n";

	time_t now; time(&now); struct tm parts, *p = &parts; 
  if(use_gmt) gmtime_r(&now, p); else localtime_r(&now, p);
  const char *format="%Y-%m-%d %H:%M:%S";
  if(type=='f') format="%Y_%m_%d_%H_%M_%S";
  else if(type=='d') format="%Y-%m-%d";
  else if(type=='w') format="%a, %d %b %Y %H:%M:%S GMT";
  int len = strftime(r, max_len, format, p); 

  if(len<=0 || len>=max_len) // strftime return 0 when buf size is not enough
  { 
    if(disable_log) printf(format_error, len, max_len, format); else log(format_error, len, max_len, format);  
    goto fail; 
  }
  
  succeed: return len;
  fail: return -1;
}

int time_text_filename(char *r, int max_len){  return time_text(r, max_len, 'f', false); }
int time_text_date(char *r, int max_len){  return time_text(r, max_len, 'd', false); }
int time_text_http_response(char *r, int max_len){  return time_text(r, max_len, 'w', true); }


}

