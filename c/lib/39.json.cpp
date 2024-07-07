#include "../includes/buda.h"


namespace BUDA
{	

#define BudaWritePad(n, pf) for(int i=0;i<n;i++) file_write(pf, "  ", 2)

void json_write_long(FILE *pf, const char *value)
{
  char buf[50]; int len = snprintf2(buf, 50, "%ld", (long)value);
  file_write(pf, buf, len);
}

void json_write_string(FILE *pf, const char *value)
{
  file_write(pf, "\"", 1);
  char c, *pc=(char*)value, *start=pc; int len=0;
  while(c=*pc)
  {  
    if(c=='\n' || c=='"' || c=='\\')
    {
      if(len>0) file_write(pf, start, len);
      if(c=='\n') file_write(pf, "\\n", 2);
      if(c=='"') file_write(pf, "\\\"", 2);
      if(c=='\\') file_write(pf, "\\\\", 2);
      start=(++pc); len=0;
    } 
    else
    {
      ++pc; ++len;
    }
  }
  if(len>0) file_write(pf, start, len);
  file_write(pf, "\"", 1); 
}

void json_write_object(FILE *pf, const char *value, int pad)
{
  int pad1=pad+1; BudaWritePad(pad, pf); file_write(pf, "{\n", 2); 
  Link *v=(Link*)value; LinkItem *item=v->first; KeyValue *kv; while(item)
  {
    kv=(KeyValue*)item->content; 
    BudaWritePad(pad1, pf); file_write(pf, kv->key, strlen(kv->key));
    file_write(pf, ": ", 2); json_write((Json*)kv->value, pf, pad1); file_write(pf, ",\n", 2);
    item=item->next;
  }  
  BudaWritePad(pad, pf); file_write(pf, "}", 1); 
}

void json_write_array(FILE *pf, const char *value, int pad)
{
  int pad1=pad+1; BudaWritePad(pad, pf); file_write(pf, "[\n", 2); 
  Link *v=(Link*)value; LinkItem *item=v->first; Json *j; while(item)
  {
    j=(Json*)item->content; 
    BudaWritePad(pad, pf); json_write(j, pf, pad1); file_write(pf, ",\n", 2);
    item=item->next;
  }    
  BudaWritePad(pad, pf); file_write(pf, "]", 1); 
}

void json_write(Json *j, FILE *pf, int pad)
{
  if(j->type=='l') { json_write_long(pf, j->value); }
  else if(j->type=='s') { json_write_string(pf, j->value); }
  else if(j->type=='o') { json_write_object(pf, j->value, pad); }
  else if(j->type=='a') { json_write_array(pf, j->value, pad); }
}


Json *json_make_time(Link *mem)
{
  BudaMnl(Json, j, mem); j->type='l'; j->value=0; 
  timespec ts; clock_gettime(CLOCK_REALTIME, &ts); j->value+=(ts.tv_sec*1000000000+ts.tv_nsec);
  return j;
}
Json *json_make_string(const char* content, Link *mem)
{
  BudaMnl(Json, j, mem); j->type='s'; j->value=content; return j;
}
KeyValue *json_make_kv_time(const char* name, Link *mem)
{
  BudaMnl(KeyValue, kv, mem); kv->key=name; kv->value=(char*)json_make_time(mem); return kv;
}
KeyValue *json_make_kv_string(const char* name, const char* content, Link *mem)
{
  BudaMnl(KeyValue, kv, mem); kv->key=name; kv->value=(char*)json_make_string(content, mem); return kv;
}
Json *json_make_obj(Link *mem, ...)
{
  BudaMnl(Json, j, mem); j->type='o'; Link *jv=link_create(mem); j->value=(char*)jv;
  va_list args; va_start(args, mem);
  while(1){ KeyValue *arg = va_arg(args, KeyValue*); if(arg==NULL) break; else link_append_item(jv, arg); }
  va_end(args);
  return j;
}
Json *json_make_array(Link *mem, ...)
{
  BudaMnl(Json, j, mem); j->type='a'; Link *jv=link_create(mem); j->value=(char*)jv;
  va_list args; va_start(args, mem);
  while(1){ Json *arg = va_arg(args, Json*); if(arg==NULL) break; else link_append_item(jv, arg); }
  va_end(args);
  return j;
}


void json_save(Json *j, const char* file_path)
{
  FILE* pf=NULL; file_write(file_path, NULL, 0, "ab",&pf);
  json_write(j, pf); file_write(pf, "\n", 1); 
  BudaFclose(pf);
}

void json_tests()
{
  Link *mem=link_create();

  char p[PATH_MAX]; sprintf(p, "%stest_json.json", log_dir);

  const char *des="i say: \n\"save all the people\"";
  Json *log=json_make_obj(mem, json_make_kv_time("time", mem), json_make_kv_string("des", des, mem), NULL);
  const char *des2="we say: \n\"save all the people\"";
  Json *log2=json_make_obj(mem, json_make_kv_time("time", mem), json_make_kv_string("des", des2, mem), NULL);
  Json *logs=json_make_array(mem, log, log2, NULL);

  json_save(logs, p);

  BudaF(mem);
}


}

