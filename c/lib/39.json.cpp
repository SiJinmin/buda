#include "../includes/buda.h"


namespace BUDA
{	

void save_json(Json *j, char* file_path)
{
  if(j->type=='l')
  {
    char buf[50]; int len = snprintf2(buf, 50, "%ld", (long)j->value);
    file_write(file_path, buf, len, "ab");
  }
  else if(j->type=='s')
  {
    file_write(file_path, "\"", 1, "ab", 1);
    char c, *pc=j->value, *start=pc; int len=0;
    while(c=*pc)
    {  
      if(c=='\n' || c=='"' || c=='\\')
      {
        if(len>0) file_write(file_path, start, len, "ab", 1);
        if(c=='\n') file_write(file_path, "\\n", 2, "ab", 1);
        if(c=='"') file_write(file_path, "\\\"", 2, "ab", 1);
        if(c=='\\') file_write(file_path, "\\\\", 2, "ab", 1);
        start=(++pc); len=0;
      } 
      else
      {
        ++pc; ++len;
      }
    }
    if(len>0) file_write(file_path, start, len, "ab", 1);
    file_write(file_path, "\"\n\n", 3, "ab");
  }
}

void json_tests()
{
  // timespec ts; clock_gettime(CLOCK_REALTIME, &ts); printf("time: %ld.%ld\n", ts.tv_sec, ts.tv_nsec);
  // Json j; j.type='l'; j.value=NULL; j.value+=(ts.tv_sec*1000000000+ts.tv_nsec);

  Json j; j.type='s'; j.value=(char*)"i say: \n\"l love you\"";

  char p[PATH_MAX]; sprintf(p, "%stest_json.json", log_dir);
  save_json(&j, p);
}


}

