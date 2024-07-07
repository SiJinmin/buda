#include "../includes/buda.h"

namespace BUDA
{
  int http_route(HttpReq* req, MemChain* sender)
  {
    int r=0; char *url = req->path; 
    if(strcmp(url, "/api/log_files")==0)
    {
      r = http_route_log_files(req, sender);
    }
    else if(strncmp(url, "/api/log_file/", strlen("/api/log_file/"))==0)
    {
      r = http_route_log_file(req, sender);
    }
    else goto fail;
    
    succeed: return r;
    fail: return -1;
  }

  int http_route_log_file(HttpReq* req, MemChain* sender)
  {
    int r=0; MemChain *file_content; char input_path[PATH_MAX]; char real_path[PATH_MAX]; 

    char* filename=req->path+strlen("/api/log_file/");
    if(snprintf2(input_path, PATH_MAX_1, "%s%s", log_dir, filename)<0) goto fail;
    if(realpath2(input_path, real_path)<0){  goto fail;}
    file_content=mem_chain_create(); if(file_content==NULL) goto fail; 
    if((r=get_file_content(real_path, file_content)) < 0) goto fail;
	  r = make_http_response(sender, NULL, r, "text/plain"); if(r<0) goto fail;
    r=mem_chain_concat(sender, file_content);

    succeed: return r;
    fail: return -1;
  }


  int http_route_log_files(HttpReq* req, MemChain* sender)
  {
    int r=0; 

    MemChain *filenames=mem_chain_create(); if(filenames==NULL) goto fail; 
    if(get_dir_filenames(log_dir, filenames)) goto fail;
	  r = make_http_response(sender, NULL, filenames->content_used, "text/plain"); if(r<0) goto fail;
    r=mem_chain_concat(sender, filenames);

    succeed: return r;
    fail: return -1;
  }



}