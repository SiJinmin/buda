
#include "../includes/buda.h"


namespace BUDA
{	
    
int realpath2(const char* input_path, char *real_path)
{
    if(realpath(input_path, real_path)!=real_path){ log("get realpath failed"); return -1;} 
    return 0;
}


int file_dir_exist(const char* path, int file_dir)
{
    int r = 0;
    struct stat status; r = stat(path, &status); if (r) return -1;
    if ((file_dir == 2 || file_dir == 3) && (status.st_mode & S_IFDIR)) return 0;
    if ((file_dir == 1 || file_dir == 3) && (S_IFREG & status.st_mode)) return 0;
    return -1;
}

    
const char* dir_create(const char* path)
{
    if (file_dir_exist(path, 2) == 0) { return path; }
    int r = mkdir(path, 0700); // owner can read, write and execute
    if (r) { log("create dir fail: %s", path); goto fail; } 
    // log("create dir succeed(%d): %s", r, path); return path;

    succeed: return path;
    fail: return NULL;
}    
char* dir_create(const char* name, const char* parent)
{
    char* path = NULL, path_relative[PATH_MAX], parent_real[PATH_MAX]; int r=0, len=0, parent_len=0;
    
    if(name==NULL || parent==NULL || strlen(name)<1 || strlen(parent)<1) { log("dir name and parent path cannot be NULL or empty"); goto fail; }
    if(realpath2(parent, parent_real)){ goto fail; } 
    if ((r = file_dir_exist(parent_real, 2)) < 0) { log("parent dir(%s) do not exists, cannot create dir in it.", parent_real); goto fail; }        
    if(snprintf2(path_relative, PATH_MAX_1, "%s/%s", parent_real, name)<2) goto fail;

    path=BudaMc(PATH_MAX); if(path==NULL){ goto fail; }
    if(realpath2(path_relative, path)){ goto fail_free; }
    parent_len=strlen(parent_real); 
    if(strncmp(path, parent_real, parent_len) || (parent_len>1 && path[parent_len]!='/')){ log("create dir name error: %s", name); goto fail_free; }

    if(file_dir_exist(path, 2) == 0) { goto succeed; }
    r = mkdir(path, 0700); // owner can read, write and execute
    if (r) { log("create dir fail: %s", path); goto fail_free; } 
    // log("create dir succeed(%d): %s", r, path); return path;

    succeed: return path;
    fail_free: BudaFree(path); fail: return NULL;
}


int file_write(const char* path, const char* buf, int buf_size, const char* mode, FILE **ppf)
{
    int r = 0; FILE* pf = NULL;
    if ((pf=fopen(path, mode))==NULL) { log("could not open file (%s) by mode wb.", path); goto fail; }
    if(buf_size>0) { r = fwrite(buf, 1, buf_size, pf); fflush(pf); }

    succeed: if(ppf) *ppf=pf; else { BudaFclose(pf); }  return r;
    fail: return -1;
}
void file_write(FILE* pf, const char* buf, int buf_size)
{
    fwrite(buf, 1, buf_size, pf); fflush(pf);
}



FILE* get_file_info_open(const char *path, struct ::stat *file_info)
{
    // log("get_file_info_open for %s", path);
    FILE* pf=NULL; int r = ::stat(path, file_info); 
    if (r || !(S_IFREG & file_info->st_mode)) { log("file does not exists when try to open"); goto fail;  } 	
    pf = fopen(path, "rb"); if (pf == NULL)  { log("could not open file %s to read.", path); goto fail; }

    succeed: return pf;
    fail: return NULL;
}
int get_file_content(FILE* pf, int file_size, MemChain *mc)
{
    char *c=NULL; MemChainBlock* last=mc->last;
    if((c=mem_chain_use(mc, file_size))==NULL) goto fail_close;
    if(fread(c, 1, file_size, pf)!=file_size){ log("read content length is not equal to file size"); goto fail_close; }

    success: BudaFclose(pf); return file_size;
    fail_close: BudaFclose(pf); return -1;
}
int get_file_content(const char *path, MemChain *mc)
{
    struct ::stat file_info; int file_size=0; char *c=NULL; MemChainBlock* last=mc->last;
    FILE* pf=get_file_info_open(path, &file_info); if(pf==NULL) return -1; 
    file_size=file_info.st_size; if(file_size<0) { log("file size error: %d", file_size); goto fail_close; }
    return get_file_content(pf, file_size, mc);

    fail_close: BudaFclose(pf); return -1;
}


int concat_filename(struct dirent *entry, void* filenames)
{
  char* name=entry->d_name;
  if(mem_chain_use(((MemChain*)filenames), "%s\n", name)==NULL) return -1;
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