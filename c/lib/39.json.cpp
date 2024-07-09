#include "../includes/buda.h"


namespace BUDA
{	
Json *json_make_long(long value, Link *mem)
{
  BudaMnl(Json, j, mem); j->type='l'; j->value=0; j->value+=value; return j;
}
Json *json_make_time(Link *mem)
{
  timespec ts; clock_gettime(CLOCK_REALTIME, &ts); return json_make_long(ts.tv_sec*1000000000+ts.tv_nsec, mem);
}
Json *json_make_string(const char* content, Link *mem)
{
  BudaMnl(Json, j, mem); j->type='s'; j->value=content; return j;
}
KeyValue *json_make_kv(const char* key, Json *value, Link *mem)
{
  BudaMnl(KeyValue, kv, mem); kv->key=key; kv->value=(const char*)value; return kv;
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


Json* json_parse(char **ppc, char *end1, Link *mem)
{
  Json *j=NULL; char c = next_char_not(ppc, end1, Space_nl); if(c<=0) goto end;
  if(c=='{')
  {    
    j=json_make_obj(mem, NULL); Link *kvs=(Link*)j->value; char *key; Json *value;
    get_key: ++(*ppc); c=next_char_not(ppc, end1, Space_nl_quote); if(c=='}' || c<=0){ ++(*ppc); goto end; } key=*ppc; 
    ++(*ppc); c=next_char(ppc, end1, Space_nl_quote_colon); if(c<=0) goto end; 
    if(c==':'){ **ppc=0;  } else{ **ppc=0; c=next_char(ppc, end1, ":"); if(c<=0) goto end; }
    ++(*ppc); value=json_parse(ppc, end1, mem); if(value==NULL) goto end;
    link_append_item(kvs, json_make_kv(key, value, mem));
    c=next_char(ppc, end1, Endbrace_comma); if(c<=0 || c=='}') goto end; if(c==',') goto get_key;
  }
  else if(c=='[')
  {
    j=json_make_array(mem, NULL); Link *js=(Link*)j->value; Json *value;
    get_value: ++(*ppc); value=json_parse(ppc, end1, mem); if(value==NULL) goto end;
    link_append_item(js, value);
    c=next_char(ppc, end1, Endbracket_comma); if(c<=0 || c==']') goto end; if(c==',') goto get_value;
  }
  else if(c=='"')
  {
    char *content=convert_escaped_string(ppc, end1); if(content==NULL) goto end;
    j=json_make_string(content, mem);
  }
  else if( c=='+' || c=='-' ||  (c>='0' && c<='9'))
  {    
    j=json_make_long(get_long_int(ppc, end1), mem);
  }
  end: return j;
}
Json* json_value(Json *obj, const char* key)
{
  if(obj->type!='o') return NULL;
  Link *kvs=(Link*)obj->value; LinkItem* li=kvs->first; while(li)
  {
    BudaCast(KeyValue*, kv, li->content); if(strcmp(key, kv->key)==0) return (Json*)kv->value;
  }
  return NULL;
}

void json_save(Json *j, const char* file_path, Link *mem)
{
  FILE* pf=NULL; file_write(file_path, NULL, 0, "ab",&pf);
  json_write(j, pf); // file_write(pf, "\n", 1); 
  BudaFclose(pf);
}
Json* json_load(const char* file_path, Link *mem)
{
  char *content=NULL; int len=file_content_get(file_path, NULL, mem, &content); if(content==NULL) return NULL;
  Json *j=json_parse(&content, content+len, mem);
  return j;
}


void json_test_load()
{
  Link *mem=link_create();

  Json *j = json_load(Conf_path, mem); if(j)
  {
    char p[PATH_MAX]; sprintf(p, "%stest_json.json", Log_dir);
    json_save(j, p, mem);
  }

  BudaF(mem);
}
void json_test_save()
{
  Link *mem=link_create();

  char p[PATH_MAX]; sprintf(p, "%stest_json.json", Log_dir);

  const char *des="i say: \n\"save all the people\"";
  Json *log=json_make_obj(mem, json_make_kv_time("time", mem), json_make_kv_string("des", des, mem), NULL);
  const char *des2="we say: \n\"save all the people\"";
  Json *log2=json_make_obj(mem, json_make_kv_time("time", mem), json_make_kv_string("des", des2, mem), NULL);
  Json *logs=json_make_array(mem, log, log2, NULL);

  json_save(logs, p, mem);

  BudaF(mem);
}


}

