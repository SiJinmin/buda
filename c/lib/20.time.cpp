#include "../includes/buda.h"


namespace BUDA
{	

int time_text(char *r, int max_len)
{  
	time_t now; time(&now); struct tm parts, *p = &parts; localtime_r(&now, p);
  int len = (int)strftime(r, max_len, "%Y-%m-%d %H:%M:%S", p); r[len]=0; return len;
}

int time_text_filename(char *r, int max_len)
{  
	time_t now; time(&now); struct tm parts, *p = &parts; localtime_r(&now, p);
  int len = (int)strftime(r, max_len, "%Y_%m_%d_%H_%M_%S", p); r[len]=0; return len;
}

int time_text_date(char *r, int max_len)
{  
	time_t now; time(&now); struct tm parts, *p = &parts; localtime_r(&now, p);
  int len = (int)strftime(r, max_len, "%Y-%m-%d", p); r[len]=0; return len;
}

int time_text_http_response(char *r, int max_len)
{  
	time_t now; time(&now); struct tm parts, *p = &parts; localtime_r(&now, p);
  int len = (int)strftime(r, max_len, "%a, %d %b %Y %H:%M:%S GMT", p); r[len]=0; return len;
}


}

