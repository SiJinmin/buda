
#include "../includes/buda.h"


namespace BUDA
{	

int realpath2(char* input_path, char *real_path)
{
    if(realpath(input_path, real_path)!=real_path){ log("get realpath for %s failed", input_path); return -1;} 
    return 0;
}


int get_file_content(FILE* pf, int file_size, MemChain *mc)
{
    char *c=NULL; MemChainBlock* last=mc->last;
    if((c=use_mem_chain(mc, file_size))==NULL) goto fail_close;
    if(fread(c, 1, file_size, pf)!=file_size){ log("read content length is not equal to file size"); goto fail_close; }

    success: BudaFclose(pf); return file_size;
    fail_close: BudaFclose(pf); return -1;
}


int get_file_content(char *path, MemChain *mc)
{
    struct ::stat file_info; int file_size=0; char *c=NULL; MemChainBlock* last=mc->last;
    FILE* pf=get_file_info_open(path, &file_info); if(pf==NULL) return -1; 
    file_size=file_info.st_size; if(file_size<0) { log("file size error: %d", file_size); goto fail_close; }
    return get_file_content(pf, file_size, mc);

    fail_close: BudaFclose(pf); return -1;
}


FILE* get_file_info_open(char *path, struct ::stat *file_info)
{
    // log("get_file_info_open for %s", path);
    FILE* pf=NULL; int r = ::stat(path, file_info); 
    if (r || !(S_IFREG & file_info->st_mode)) { log("file does not exists: %s", path); goto fail;  } 	
    pf = fopen(path, "rb"); if (pf == NULL)  { log("could not open file %s to read.", path); goto fail; }

    succeed: return pf;
    fail: return NULL;
}


int concat_filename(struct dirent *entry, void* filenames)
{
  char* name=entry->d_name;
  if(use_mem_chain(((MemChain*)filenames), "%s\n", name)==NULL) return -1;
  return 0;
}

int get_dir_filenames(const char *dir_path, MemChain *mc)
{
    return iterate_dir(dir_path, concat_filename, mc);  
}

int iterate_dir(const char *dir_path, process_entry processor, void* arg) 
{
  DIR *dir=NULL; struct dirent *entry;

  if ((dir = opendir(dir_path)) == NULL) { log("open dir error: %s", dir_path); goto fail; }
  while ((entry = readdir(dir)) != NULL) 
  {
      if (entry->d_type != DT_REG) continue;
      if(processor(entry, arg)) goto fail;
  }

  succeed: if(dir) closedir(dir); return 0;
  fail: if(dir) closedir(dir); return -1;
}


}