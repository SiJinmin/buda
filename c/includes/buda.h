

#ifndef buda_h
#define buda_h



//--------------- system --------------

/// the system use utf-8 as unified encoding, must be pre of includes for its usage.
#ifndef UNICODE
#define UNICODE
#endif

/// common in windows and linux
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <regex.h>
#include <string.h>
#include <locale.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>

/// by linux only, not by windows
#include <unistd.h>
#include <stddef.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/time.h>
#include <linux/input.h>
//// linux socket
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>



//--------------- macros --------------

 
#define BudaMc(size) (char*)calloc2(size)
#define BudaM(T) (T*)calloc2(sizeof(T))
#define BudaMn(T, name) T *name = (T*)calloc2(sizeof(T))
#define BudaMnl(T, name, mem) T *name = (T*)calloc2(sizeof(T), mem)
#define BudaMn2(T1, name1, T2, name2) BudaMn(T1, name1); BudaMn(T2, name2)
#define BudaMn3(T1, name1, T2, name2, T3, name3) BudaMn(T1, name1); BudaMn2(T2, name2, T3, name3)

#define BudaZ(T, name) T name; memset(&name, 0, sizeof(name))

#define BudaFclose(pf) if(pf){ fclose(pf); pf = NULL; }
#define BudaFree(m) if(m){ free(m); m = NULL; }
#define BudaF(mem) LinkItem* mem_item=mem->first; while(mem_item){ free(mem_item->content); mem_item=mem_item->next; } link_free(mem)

#define BudaHex2Dec(c) (c<='9'? (c-'0'):(c<='Z'? (c-55):(c-87)))

#define BudaWriteStep(c, len, remain, sum, fail) c += len; remain -= len; sum += len; if(remain<=0) goto fail;
#define BudaWriteStep2(c, len, remain) c += len; remain -= len;
#define BudaPreWrite(max_len, dec, fail) if((max_len-=dec)<0) goto fail;
#define BudaPreWrite1(max_len, fail) if((--max_len)<0) goto fail;
#define BudaWriteCheck(max_len, len, fail) if(max_len<=len) goto fail;
#define BudaWritePad(n, pf) for(int i=0;i<n;i++) file_write(pf, "  ", 2)

#define BudaMax(a, b) ((a)<(b)?(b):(a))
#define BudaCast(T, r, s) T r=(T)(s)




namespace BUDA
{

//-------------------- structs ---------------------------

typedef struct key_value
{
  const char *key;
  const char *value;
} KeyValue;

typedef struct log_data
{
   const char *content;
   long time;
   long user;
   long device;
   long session;
   long request;   
} LogData;


typedef struct mem_chain_block
{
  char *mem;
  struct mem_chain_block *next;
  int size;
  int used;
} MemChainBlock;
typedef struct mem_chain
{
  struct mem_chain_block *first;
  struct mem_chain_block *last;
  int max_size;
  int used;            // sum of allocated mem of blocks
  int content_used;     // sum of used bytes of blocks
  int block_min_size;
  int block_count;
} MemChain;


typedef struct link_item
{
   void* content;
   struct link_item *pre;
   struct link_item *next;   
} LinkItem;
typedef struct link
{
  // if mem==NULL, use link_free(this) to free all the memory of self and children nodes; 
  // if mem!=NULL, use mem to free memory of self and children nodes.
  struct link *mem; 
  struct link_item *first;
  struct link_item *last;
  int item_count;
} Link;
typedef struct mem_obj
{
   Link *mem;
   void *obj;
} MemObj;


typedef struct json
{
   const char* value; 
   // 0-invalid, n-null, b-false/true, i-32bits.integal, l-64bits.integal, f-float.64bits, s-string, a-array, o-object, d-datetime, t-timenano  
   u_char type; 
} Json;


typedef struct http_req
{
  char* method;
  char* path;
  char* content;
  int content_len;
} HttpReq;

typedef struct http_mime
{
  const char* ext;
  const char* type;
  int ext_len;
} HttpMime;


//--------------- constants --------------

extern const int PATH_MAX_1;
extern const u_char BYTE_LOW4_MASK;
extern const char* HEXS; extern const char* URL_ALLOW;

extern const char *Space_nl; extern const char *Space_nl_quote; extern const char *Space_nl_quote_colon;
extern const char *Endbrace_comma; extern const char *Endbracket_comma;

extern const HttpMime HttpMimes[]; extern const int HttpMimesLen;

extern const int TIME_BUF_SIZE; extern const int TIME_BUF_SIZE1;
extern const int SOCK_BUF_RECV_SIZE;      // 8K
extern const int SOCK_BUF_SEND_SIZE_MAX;  // about 200M, controled by MemChain
extern const int SOCK_BUF_SEND_SIZE_INIT; // about 10M, it's just the init size, the memory need will be allocated by MemChain
extern const int SOCK_CONN_QUEUE_MAX;
extern const int SOCK_PORT_MAX;

extern const char *Conf_path;
extern const char *VERSION;	
extern const char* admin_pw;
extern char web_root[]; extern int web_root_len; 
extern int server_listen_port;

extern const char *Log_dir; extern char log_input_dir[]; extern FILE* log_file;
extern int Logs_max_count; extern int logs_saved_count;
extern Link *logs; extern bool log_ready;

extern const int Thread_buf_size; extern char thread_buf[];


//-------------------- sys.cpp ---------------------------

void show_sys_info();

// Pay attention to not log inside itself
// return 0 for success, return -1 for failure
int log_start();
// Pay attention to not log inside itself
void log(const char *format, ...);

void read_conf(Link *mem);


//-------------------- mem.cpp ---------------------------

// calloc memory, log fail message
// caller should make sure the size >= 1
// return NULL for failure
char* calloc2(int size, Link *mem=NULL);
// MUST use mem_chain_free to free heap mem used
// return NULL for failure
MemChain* mem_chain_create(int max_size=100002048, int block_min_size=2048);
// destroy the whole mem_chain
void mem_chain_free(MemChain *mc);
// delete all the blocks except the first one, reset mem_chain to the beginning
void mem_chain_reset(MemChain *mc);
// return NULL if failed
MemChainBlock* mem_chain_add_block(MemChain *mc, int size);
// return new mc.content_used, mc2 will be destroyed
int mem_chain_concat(MemChain *mc, MemChain *mc2);
// If the memory size to use is known to be size, use this function; otherwise, use the next function. 
// Return NULL if failed, return the start of used memory if succeed.
char* mem_chain_use(MemChain *mc, int size, char* content=NULL);
// Return NULL if failed, return the start of used memory if succeed.
char* mem_chain_use(MemChain *mc, const char* format, ...);


//-------------------- link.cpp ---------------------------
Link* link_create(Link *mem=NULL);
LinkItem* link_append_item(Link *link, void* content);
int link_concat(Link *link, Link *link2);
void link_free(Link *link);
void link_reset(Link *link);
MemObj* mem_obj_create();


//-------------------- string.cpp ---------------------------

// check the result len and log error info
// Return -1 for error, and len for succeed
int vsnprintf2 (char *s, size_t size, const char *format, va_list args) ;
// check the result len and log error info
// Return -1 for error, and len for succeed
int snprintf2 (char *s, size_t size, const char *format, ...);


long get_long_int(char **ppc, char *end1);
char* convert_escaped_string(char **ppc, char *end1);
char next_char(char **ppc, char* end1, const char *scope);
char next_char_not(char **ppc, char* end1, const char *scope);

// return -1 for invalid user input, 0 for good input with no match
// int check_user_input_for_log(char* input);
// caller should provide the regex to receive compiled result
// return 0 for success , -1 for failure
int compile_regex(char *pattern, regex_t *regex) ;
// search the first match pattern in content, return it's start and end pointer
// return -1 for failure, 0 for no match, 1 for match
int search_first(char *content, char *pattern, char **startPtr=NULL, char **endPtr=NULL);
// search the first match pattern in content, return it's start and end pointer
// return -1 for failure, 0 for no match, 1 for match
int search_first(char *content, regex_t *regex, char **startPtr=NULL, char **endPtr=NULL);
// return -1 for failure, 0 for no match, 1 for match
int search_log_time(char *content);

// set the next space or newline to 0 as the string end
// if not found, set pc_end to 0 as string end
// return the next char of string end
char* set_next_space_0(char *pc, char *pc_end);
// get the start of next line from pc
// return NULL for not found, return next line start for success
char* next_line(char *pc);
/* This function is originally written for parse http request, but can be used by others.
   It gets the first token ended by end in the current line starting from *start.
   If not found, *start remains original value, *token=NULL, return -1.
   If found, set the end char to \0 as the found token end, set *start point to the next char of end char, set *token to the found token, and return 0.*/
int get_token_by_char_end(char** start, char** token, char end=' ', char line_end='\r');
int hex2dec(char c);
// return -1 for failure, 0 for success
int url_encode(u_char *s, u_char *d, int max_len=PATH_MAX_1);
// return -1 for failure, 0 for success
int url_decode(u_char *s, u_char *d, int max_len=PATH_MAX_1);


//--------------------------- time.cpp ---------------------------

long time_nano();
/* type=t (time): 2024-06-25 13:40:33
   type=d (date): 2024-06-25
   type=w (week): Fri, 22 May 2009 06:07:21 GMT
   type=f (filename): 2024_06_25_13_40_33
   use_gmt is bool value, indicate using GMT/Local time
   disable_log is bool value, indicate using log() in it
   return -1 for failure, return written lengh for success */
int time_text(char *r, int max_len, char type='t', int use_gmt=0, int disable_log=0);
// time format like 2024_06_25_13_40_33
// return -1 for failure, return written lengh for success
int time_text_filename(char *r, int max_len);
// time format like 2024-06-25
// return -1 for failure, return written lengh for success
int time_text_date(char *r, int max_len);
// time format like Fri, 22 May 2009 06:07:21 GMT
// return -1 for failure, return written lengh for success
int time_text_http_response(char *r, int max_len);


//--------------------------- file.cpp ---------------------------
// return -1 if failed, return 0 for succeed
int realpath2(const char* input_path, char *real_path);

/* file_dir is 1 for file, 2 for dir, 3 for file or dir. return 0 if exists, -1 for not exist*/
int file_dir_exist(const char* path, int file_dir);

/* Make sure the path is not user input info.
   return NULL for faiure; 
   return path for existence or create success. */
const char* dir_create(const char* path);
/* Make sure the name and parent are not user input info.
   return NULL for failure, 
   return created dir real path for create success or existence.  */
char* dir_create(const char* name, const char* parent);
// if ppf is provided, the FILE* will keep open and return to ppf; otherwise, the file will be closed.
// mode=wb: overwrite the content of this file, create it if not exist
// mode=ab: append to the content of this file, create it if not exist
// return written bytes for success, return -1 for failure
int file_write(const char* path, const char* buf=NULL, int buf_size=0, const char* mode="wb", FILE **ppf=NULL);
void file_write(FILE* pf, const char* buf, int buf_size);


// return -1 for failure, return file size for sucess
int file_content_get(FILE* pf, int file_size, MemChain *mc=NULL, Link *mem=NULL, char **content=NULL);
// Make sure the path is not user input info.
// return -1 for failure, return file size for sucess
int file_content_get(const char *path, MemChain *mc=NULL, Link *mem=NULL, char **content=NULL);
// return NULL if failed, return open file for success
FILE* file_info_open(const char *input_path, struct ::stat *file_info);

// return -1 if failed, return 0 for succeed
typedef int (*process_entry)(struct dirent *entry, void* arg);
// make sure the dir_path is existing
// return -1 if failed, return 0 for succeed
int dir_iterate(const char *dir_path, process_entry processor, void* arg);
// return -1 if failed, return 0 for succeed
int filename_concat(struct dirent *entry, void* filenames);
// return -1 if failed, return 0 for succeed
int dir_filenames(const char *dir_path, MemChain *mc);


//--------------------------- json.cpp ---------------------------

Json *json_make_long(long value, Link *mem);
Json *json_make_time(Link *mem);
Json *json_make_string(const char* content, Link *mem);
KeyValue *json_make_kv(const char* key, Json *value, Link *mem);
KeyValue *json_make_kv_time(const char* name, Link *mem);
KeyValue *json_make_kv_string(const char* name, const char* content, Link *mem);
Json *json_make_obj(Link *mem, ...);
Json *json_make_array(Link *mem, ...);

void json_write(Json *j, FILE *pf, int pad=0);
void json_write_long(FILE *pf, const char *value);
void json_write_string(FILE *pf, const char *value);
void json_write_object(FILE *pf, const char *value, int pad=0);
void json_write_array(FILE *pf, const char *value, int pad);

Json* json_parse(char **ppc, char *end1, Link *mem);
Json* json_value(Json *obj, const char* key);
void json_save(Json *j, const char* file_path, Link *mem);
Json* json_load(const char* file_path, Link *mem);

void json_test_load();
void json_test_save();


//--------------------------- socket.cpp ---------------------------

/* 通过域名获取socket可用的ip地址格式，用于连接server socket 
   hostname为域名，例如： idealand.space */
struct in_addr * get_sock_addr(char *hostname);
/* 查看和设置socket发送和接收数据的超时时长，设置为5秒超时 */
int set_socket_options(int socket);


//--------------------------- http.cpp ---------------------------

/* return 0 for succeed, return -1 for failure. */
int parse_http_request(char* req, HttpReq* r);
/* Return response size if succeed, or -1 for failure.
   if encoding is NULL, it will be auto detected by content_type
   content_len = -1 means it should be strlen(content)  */
int make_http_response(MemChain* sender, char* content=NULL, int content_len=-1, const char* content_type="text/html", const char* encoding="UTF-8", int status_no=200, const char* status_code="OK", char* filename=NULL);
// return -1 for failure, return response total size for success
int make_http_response_file(MemChain* sender, const char* url, const char* content_type="text/html");
// return -1 for failure, return response total size for success
int http_route(HttpReq* req, MemChain* sender);
// return -1 for failure, return response total size for success
int http_route_log(HttpReq* req, MemChain* sender);
// return -1 for failure, return response total size for success
int http_route_log_file(HttpReq* req, MemChain* sender);
// return -1 for failure, return response total size for success
int http_route_log_files(HttpReq* req, MemChain* sender);


//--------------------------- help.cpp ---------------------------
void print_help(char* program_name);




//--------------------------- tset.cpp ---------------------------

int test_threads();
int test_threads_mutex();




}


#endif