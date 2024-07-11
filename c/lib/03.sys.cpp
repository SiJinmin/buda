#include "../includes/buda.h"

namespace BUDA
{
  void show_sys_info()
  {
	  printf("void*=%ld, double=%ld, long long=%ld, long=%ld, float=%ld, int=%ld, short=%ld, u_char=%ld\n",
    sizeof(void*), sizeof(double), sizeof(long long), sizeof(long), sizeof(float), sizeof(int), sizeof(short), sizeof(u_char)); 

	  // printf("char=%d~%d, short=%d~%d, int=%d~%d, long=%ld~%ld, long long=%lld~%lld\n",
    // CHAR_MIN, CHAR_MAX, SHRT_MIN, SHRT_MAX, INT_MIN, INT_MAX, LONG_MIN, LONG_MAX, LLONG_MIN, LLONG_MAX ); 

	  printf("u_char=0~%d, u_short=0~%d, u_int=0~%u, u_long=0~%lu, u_long_long=0~%llu\n",
    UCHAR_MAX, USHRT_MAX, UINT_MAX, ULONG_MAX, ULLONG_MAX); 

  }

  int log_start()
  {  
    return 0;
    int r=0; char real_path[PATH_MAX], *c=real_path; int remain=PATH_MAX_1, len; struct stat status; 
    
    r = stat(Log_dir, &status); if (!(r==0 && (status.st_mode & S_IFDIR))) 
    {
      r = mkdir(Log_dir, 0700); // owner can read, write and execute
      if (r) { printf("create dir fail: %s\n", Log_dir); goto fail; }   
    }    
	  sprintf(log_input_dir, "%s%s", Log_dir, "input/"); r = stat(log_input_dir, &status); if (!(r==0 && (status.st_mode & S_IFDIR))) 
    { r = mkdir(log_input_dir, 0700); if (r) { printf("create dir fail: %s\n", log_input_dir); goto fail; } }

    len = snprintf(c, remain, "%s", Log_dir); BudaWriteStep2(c, len, remain);
    len = time_text(c, remain, 'f', false, true); BudaWriteStep2(c, len, remain); 
    len = snprintf(c, remain, ".txt"); BudaWriteStep2(c, len, remain);  
    log_file = fopen(real_path, "ab"); if (log_file == NULL) { printf("Failed to open file: %s\n", real_path); goto fail; }
    printf("created log file: %s\n", real_path);

    logs=link_create();

    succeed: log_ready=true; return 0;
    fail: return -1;
  }

  void log(const char *format, ...)
  {
    return;
    if(!log_ready) return;
       
    va_list args; va_start(args, format); vsnprintf(thread_buf, Thread_buf_size, format, args); va_end(args); 
    va_start(args, format); vprintf(format, args); va_end(args);

    LogData* d = (LogData*)calloc(1, sizeof(LogData)); if (!d) { printf("alloc memory for LogData failed."); return; }  
    char* content = (char*)calloc(1, strlen(thread_buf)+1); if (!content) { printf("alloc memory for LogData content failed."); return; }  
    d->content=content; d->time=time_nano(); link_append_item(logs, d);
  }

  void log_save()
  {
    // Json *j=json_make_obj(mem, NULL); 
    // KeyValue *kv_time = json_make_kv_time("t", mem); KeyValue *kv_content=json_make_kv_string("c", )
    // fprintf(log_file, "\n"); printf("\n");
    // fflush(log_file);
  }

  void read_conf(Link *mem)
  {
    Json *conf = json_load(Conf_path, mem); if(conf)
    {
      Json *pw=json_value(conf, "pw"); if(pw && pw->type=='s') printf("admin password: %s\n", pw->value);
    }
  }


}