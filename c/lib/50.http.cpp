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
int make_http_response(char* buf, int buf_size, char* content, int content_len, const char* content_type, const char* encoding, int status_no, const char* status_code, char* filename)
{
	//"HTTP/1.1 200 OK\r\nDate: Fri, 22 May 2009 06:07:21 GMT\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n" 
	if(content_len==-1 && content) content_len=strlen(content);
	char* c = buf; int remain = buf_size-1, len=0, sum=0;

	time_t now; time(&now); struct tm parts, * p = &parts; gmtime_s(p, &now); 

	len = snprintf(c, remain, "HTTP/1.1 %d %s\r\nDate: ", status_no, status_code); StreamWriteStep;
	len = (int)strftime(c, remain, "%a, %d %b %Y %H:%M:%S GMT\r\n", p); StreamWriteStep;
	len = snprintf(c, remain, "Content-Length: %d\r\nContent-Type: %s", content_len, content_type); StreamWriteStep;
	if(encoding) { len=snprintf(c, remain, "; charset=%s", encoding); StreamWriteStep; }
	len = snprintf(c, remain, "\r\n"); StreamWriteStep;
	if(filename) { len=snprintf(c, remain, "Content-Disposition: attachment; filename=%s\r\n", filename); StreamWriteStep; }      
	len=snprintf(c, remain, "\r\n"); StreamWriteStep;
	if(content_len>0 && content){ len=snprintf(c, remain, "%s", content); StreamWriteStep; }
	//IdealandMsn(sum + 1, r); memcpy(r, StringBuf, sum);  log_show("http response content type: %s", content_type);return r;
	c[0]=0;return sum;
	fail: return -1;
}


#define RedirectToIndexPage url="/index.html"; content_type="text/html"
#define RedirectToIndexPageGoto url="/index.html"; content_type="text/html"; goto no_redirect
int make_http_response_file(char* buf, int buf_size, char* web_root, int web_root_len, const char* url, const char* content_type)
{	
	if(url[0]=='/' && url[1]==0) { RedirectToIndexPage; }

	int r=0; char path_input[FILE_PATH_SIZE], path_real[FILE_PATH_SIZE]; struct stat file_info; int sum, remain, len, file_size; char* c; FILE* pf=NULL;

  if(url_decode((u_char*)url, (u_char*)path_real)) { printf("decode url failed: %s\n", url); RedirectToIndexPageGoto;} 
	else { printf("decoded url: %s\n", path_real); }
  len = snprintf(path_input, FILE_PATH_SIZE1, "%s%s", web_root, path_real); 
	if(len >= FILE_PATH_SIZE1){ printf("file path too long: %d, redirect to index\n", len); RedirectToIndexPageGoto; }
	if(realpath(path_input, path_real)!=path_real) { printf("get file real path error: %s\n", path_input); RedirectToIndexPageGoto;  }
	if(strncmp(web_root, path_real, web_root_len) || path_real[web_root_len]!='/')
	{ printf("file does not under web root: %s(%s)\n", url, path_real); RedirectToIndexPageGoto; }
	if(content_type==NULL)
	{
		for(int i=0;i<HttpMimesLen;i++)
		{
			HttpMime *m=&(HttpMimes[i]); int len=m->ext_len; const char* ext=m->ext+(len-1); const char *name=path_real; name+=(strlen(name)-1); int same=1;
			for(int j=0;j<len;j++, ext--, name--)	{	if(*ext != *name) {same=0; break;}	}
			if(same) { content_type=m->type; printf("mime type is %s\n", content_type); break; }
		}
		if(content_type==NULL) { printf("unknown mime type: %s\n", url); RedirectToIndexPageGoto;  }
	}
	r = stat(path_real, &file_info); 
	if (r || !(S_IFREG & file_info.st_mode)) { printf("file does not exists: %s\n", path_real); RedirectToIndexPageGoto;  } 	
	pf = fopen(path_real, "rb"); if (pf == NULL)  { printf("could not open file (%s) to read.", path_real); RedirectToIndexPageGoto; }	
  goto response;

	no_redirect:
	snprintf(path_real, FILE_PATH_SIZE1, "%s%s", web_root, url);
	stat(path_real, &file_info); pf = fopen(path_real, "rb"); 

  response:
	file_size=file_info.st_size; printf("http response file size=%d\n", file_size);
	sum = make_http_response(buf, buf_size, NULL, file_size, content_type, NULL); 
	c=buf+sum; remain=buf_size-sum; printf("http response head len=%d, remain=%d\n", sum, remain);
	if(file_size>remain){ printf("file too big for http response(size/remain=%d/%d): %s\n", file_size, remain, path_real); goto fail; }
	len=fread(c, 1 , file_size, pf); StreamWriteStep; printf("total http response len=%d\n", sum);	

	succeed: BudaFclose(pf); return sum;
	fail: BudaFclose(pf); return -1;	
}


}
