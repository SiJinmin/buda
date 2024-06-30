#include "../includes/buda.h"


namespace BUDA
{
	
int parse_http_request(char* req, HttpReq* r)
{
	if(get_token_by_char_end(&req, &r->method)) goto fail;
	if(get_token_by_char_end(&req, &r->path)) goto fail;
	
	char path_real[PATH_MAX]; if(url_decode((u_char*)r->path, (u_char*)path_real)) { goto fail;} else 
	{ 
		if(check_user_input_for_log(path_real)) goto fail; 
		else { strcpy(r->path, path_real); log("parse http request succeed: %s %s", r->method, r->path);	} 
	}

	succeed: return 0;
	fail: log("parse http request failed."); return -1;
}


int make_http_response(MemChain* sender, char* content, int content_len, const char* content_type, const char* encoding, int status_no, const char* status_code, char* filename)
{
	//"HTTP/1.1 200 OK\r\nDate: Fri, 22 May 2009 06:07:21 GMT\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n" 
	MemChainBlock *last=sender->last; int len; char* c;

	if(content_len<0) { if(content) content_len=strlen(content); else { log("cannot get content_len for http head"); goto fail; } }
	use_mem_chain(sender, "HTTP/1.1 %d %s\r\nDate: ", status_no, status_code);
	len = time_text_http_response(last->mem+last->used, last->size - last->used); use_mem_chain(sender, len);
	use_mem_chain(sender, "\r\nContent-Length: %d\r\nContent-Type: %s", content_len, content_type);
	if(!encoding){ if(strncmp(content_type, "text/", 5)==0) encoding="UTF-8"; }  if(encoding){ use_mem_chain(sender, "; charset=%s", encoding); }
	use_mem_chain(sender, "\r\n");
	if(filename) { use_mem_chain(sender, "Content-Disposition: attachment; filename=%s\r\n", filename); }      
	use_mem_chain(sender, "\r\n"); last->mem[last->used]=0; log("made http head: %d bytes\n%s", last->used, last->mem);
	if(content_len>0 && content){ c = use_mem_chain(sender, content_len, content); if(c==NULL) goto fail; }
	
	succeed: return sender->content_used;
	fail: return -1;
}

int make_http_response_file(MemChain* sender, const char* url, const char* content_type)
{	
	log("make_http_response_file for %s", url);
	int r=0; char path_input[PATH_MAX], path_real[PATH_MAX]; struct stat file_info; int len, file_size; char* c; FILE* pf=NULL;

	if(url[0]=='/' && url[1]==0) { goto no_redirect; }

  len = snprintf2(path_input, PATH_MAX_1, "%s%s", web_root, url); if(len==-1){ log("get file path failed"); goto no_redirect; }
	if(realpath2(path_input, path_real)<0) { goto no_redirect;  }
	if(strncmp(web_root, path_real, web_root_len) || path_real[web_root_len]!='/')
	{ log("file does not under web root: %s(%s)", url, path_real); goto no_redirect; }
	if(content_type==NULL)
	{
		for(int i=0;i<HttpMimesLen;i++)
		{
			const HttpMime *m=&(HttpMimes[i]); int len=m->ext_len; const char* ext=m->ext+(len-1); const char *name=path_real; name+=(strlen(name)-1); int same=1;
			for(int j=0;j<len;j++, ext--, name--)	{	if(*ext != *name) {same=0; break;}	}
			if(same) { content_type=m->type; log("mime type is %s", content_type); break; }
		}
		if(content_type==NULL) { log("unknown mime type for: %s", path_real); goto no_redirect;  }
	}
	pf = get_file_info_open(path_real, &file_info); if(pf==NULL) goto no_redirect; else goto response;

	no_redirect:
	url="/index.html"; content_type="text/html"; snprintf2(path_real, PATH_MAX_1, "%s%s", web_root, url); 
	pf = get_file_info_open(path_real, &file_info); if(pf==NULL) goto fail; 

  response:
	file_size=file_info.st_size; log("http response file: %s, size=%d", path_real, file_size);
	r = make_http_response(sender, NULL, file_size, content_type, NULL); if(r<0) goto fail_close;
	if(get_file_content(pf, file_size, sender) < 0) goto fail;

	succeed: return sender->content_used;
	fail_close: BudaFclose(pf); fail: return -1;	
}


}
