#include "../includes/buda.h"


namespace BUDA
{

	
int parse_http_request(char* req, HttpReq* r)
{
	if(get_token_by_char_end(&req, &r->method)) goto fail;
	if(get_token_by_char_end(&req, &r->path)) goto fail;
	log("parse http request succeed: %s %s", r->method, r->path);

	return 0;
	fail: log("parse http request failed."); return -1;
}


int make_http_response(char* buf, int buf_size, char* content, int content_len, const char* content_type, const char* encoding, int status_no, const char* status_code, char* filename)
{
	//"HTTP/1.1 200 OK\r\nDate: Fri, 22 May 2009 06:07:21 GMT\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n" 
	if(content_len==-1 && content) content_len=strlen(content);
	char* c = buf; int remain = buf_size-1, len=0, sum=0;

	len = snprintf(c, remain, "HTTP/1.1 %d %s\r\nDate: ", status_no, status_code); BudaWriteStep(c, len, remain, sum, fail);
	len = time_text_http_response(c, remain); BudaWriteStep(c, len, remain, sum, fail);
	len = snprintf(c, remain, "\r\nContent-Length: %d\r\nContent-Type: %s", content_len, content_type); BudaWriteStep(c, len, remain, sum, fail);
	if(encoding) { len=snprintf(c, remain, "; charset=%s", encoding); BudaWriteStep(c, len, remain, sum, fail); }
	len = snprintf(c, remain, "\r\n"); BudaWriteStep(c, len, remain, sum, fail);
	if(filename) { len=snprintf(c, remain, "Content-Disposition: attachment; filename=%s\r\n", filename); BudaWriteStep(c, len, remain, sum, fail); }      
	len=snprintf(c, remain, "\r\n"); BudaWriteStep(c, len, remain, sum, fail);
	if(content_len>0 && content){ len=snprintf(c, remain, "%s", content); BudaWriteStep(c, len, remain, sum, fail); }
	//IdealandMsn(sum + 1, r); memcpy(r, StringBuf, sum);  log_show("http response content type: %s", content_type);return r;
	c[0]=0;return sum;
	fail: return -1;
}


#define RedirectToIndexPage url="/index.html"; content_type="text/html"
#define RedirectToIndexPageGoto url="/index.html"; content_type="text/html"; goto no_redirect
int make_http_response_file(char* buf, int buf_size, char* web_root, int web_root_len, const char* url, const char* content_type)
{	
	if(url[0]=='/' && url[1]==0) { RedirectToIndexPage; }

	int r=0; char path_input[PATH_MAX], path_real[PATH_MAX]; struct stat file_info; int sum, remain, len, file_size; char* c; FILE* pf=NULL;

  if(url_decode((u_char*)url, (u_char*)path_real)) { log("decode url failed: %s", url); RedirectToIndexPageGoto;} 
	else { log("decoded url: %s", path_real); }
  len = snprintf(path_input, PATH_MAX_1, "%s%s", web_root, path_real); 
	if(len >= PATH_MAX_1){ log("file path too long: %d", len); RedirectToIndexPageGoto; }
	if(realpath(path_input, path_real)!=path_real) { log("get file real path error: %s", path_input); RedirectToIndexPageGoto;  }
	if(strncmp(web_root, path_real, web_root_len) || path_real[web_root_len]!='/')
	{ log("file does not under web root: %s(%s)", url, path_real); RedirectToIndexPageGoto; }
	if(content_type==NULL)
	{
		for(int i=0;i<HttpMimesLen;i++)
		{
			HttpMime *m=&(HttpMimes[i]); int len=m->ext_len; const char* ext=m->ext+(len-1); const char *name=path_real; name+=(strlen(name)-1); int same=1;
			for(int j=0;j<len;j++, ext--, name--)	{	if(*ext != *name) {same=0; break;}	}
			if(same) { content_type=m->type; log("mime type is %s", content_type); break; }
		}
		if(content_type==NULL) { log("unknown mime type: %s", url); RedirectToIndexPageGoto;  }
	}
	r = stat(path_real, &file_info); 
	if (r || !(S_IFREG & file_info.st_mode)) { log("file does not exists: %s", path_real); RedirectToIndexPageGoto;  } 	
	pf = fopen(path_real, "rb"); if (pf == NULL)  { log("could not open file (%s) to read.", path_real); RedirectToIndexPageGoto; }	
  goto response;

	no_redirect:
	snprintf(path_real, PATH_MAX_1, "%s%s", web_root, url);
	stat(path_real, &file_info); pf = fopen(path_real, "rb"); 

  response:
	file_size=file_info.st_size; log("http response file size=%d", file_size);
	sum = make_http_response(buf, buf_size, NULL, file_size, content_type, NULL); if(sum<0) goto fail;
	c=buf+sum; remain=buf_size-sum; log("http response head len=%d, remain=%d", sum, remain);
	if(file_size>remain){ log("file too big for http response(size/remain=%d/%d): %s", file_size, remain, path_real); goto fail; }
	len=fread(c, 1 , file_size, pf); BudaWriteStep(c, len, remain, sum, fail); log("total http response len=%d", sum);	

	succeed: BudaFclose(pf); return sum;
	fail: BudaFclose(pf); return -1;	
}


}
