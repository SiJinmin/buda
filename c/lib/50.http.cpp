#include "../includes/buda.h"


namespace BUDA
{

	
int parse_http_request(char* req, HttpReq* r)
{
	if(get_token_by_char_end(&req, &r->method)) goto fail;
	if(get_token_by_char_end(&req, &r->path)) goto fail;
	printf("parse http request succeed: %s %s\n", r->method, r->path);

	return 0;
	fail: printf("parse http request failed.\n"); return -1;
}


#define StreamWriteStep c += len; remain -= len; sum += len; if(remain<=0) goto fail;
int make_http_response(char* StringBuf, int StringBufSize, char* content, int content_len, const char* content_type, const char* encoding, int status_no, const char* status_code, char* filename)
{
	//"HTTP/1.1 200 OK\r\nDate: Fri, 22 May 2009 06:07:21 GMT\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n" 
	if(content_len==-1 && content) content_len=strlen(content);
	char* c = StringBuf; int remain = StringBufSize-1, len=0, sum=0;

	time_t now; time(&now); struct tm parts, * p = &parts; gmtime_s(p, &now); 

	len = snprintf(c, remain, "HTTP/1.1 %d %s\r\nDate: ", status_no, status_code); StreamWriteStep;
	len = (int)strftime(c, remain, "%a, %d %b %Y %H:%M:%S GMT\r\n", p); StreamWriteStep;
	len = snprintf(c, remain, "Content-Length: %d\r\nContent-Type: %s", content_len, content_type); StreamWriteStep;
	if(encoding) { len=snprintf(c, remain, "; charset=%s", encoding); StreamWriteStep; }
	len = snprintf(c, remain, "\r\n"); StreamWriteStep;
	if(filename) { len=snprintf(c, remain, "Content-Disposition: attachment; filename=%s\r\n", filename); StreamWriteStep; }      
	len=snprintf(c, remain, "\r\n"); StreamWriteStep;
	if(content_len>0){ len=snprintf(c, remain, "%s", content); StreamWriteStep; }
	//IdealandMsn(sum + 1, r); memcpy(r, StringBuf, sum);  log_show("http response content type: %s", content_type);return r;
	c[0]=0;return sum;
	fail: return -1;
}

}
