#include "../includes/buda.h"


int parse_http_request(char* req, HttpReq* r)
{
	if(get_token_by_char_end(&req, ' ', &r->method)) return -1;
	if(get_token_by_char_end(&req, ' ', &r->path)) return -1;
	printf("parse http request succeed: %s %\n", r->method, r->path);
	return 0;
}


char* make_http_response(char* StringBuf, int StringBufSize, int status_number, char* status_message, char* content, int content_len, char* content_type, char* encoding, char* filename)
{
	//"HTTP/1.1 200 OK\r\nDate: Fri, 22 May 2009 06:07:21 GMT\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n" 
	if(status_number==0) status_number=200;
	if(status_message==NULL) status_message="OK";
	if(content_len==-1) content_len=strlen(content);
	char* c = StringBuf; int remain = StringBufSize-1, len=0, sum=0;

	time_t now; time(&now); struct tm parts, * p = &parts; gmtime_s(p, &now); 

	len = snprintf(c, remain, "HTTP/1.1 %d %s\r\nDate: ", status_number, status_message); c += len; remain -= len; sum += len;
	len = (int)strftime(c, remain, "%a, %d %b %Y %H:%M:%S GMT\r\n", p); c += len; remain -= len; sum += len;
	len = snprintf(c, remain, "Content-Length: %d\r\nContent-Type: %s", content_len, content_type); c += len; remain -= len; sum += len;
	if(encoding) { len=snprintf(c, remain, "; charset=%s", encoding); c += len; remain -= len; sum += len; }
	len = snprintf(c, remain, "\r\n"); c += len; remain -= len; sum += len;
	if (filename) { len=snprintf(c, remain, "Content-Disposition: attachment; filename=%s\r\n", filename); c += len; remain -= len; sum += len; }      
	len=snprintf(c, remain, "\r\n"); c += len; remain -= len; sum += len;
	if(content_len>0){ len=snprintf(c, remain, "%s", content); c += len; remain -= len; sum += len; }
	//IdealandMsn(sum + 1, r); memcpy(r, StringBuf, sum);  log_show("http response content type: %s", content_type);return r;
	c[0]=0;return StringBuf;
}