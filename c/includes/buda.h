

#ifndef buda_h
#define buda_h



//--------------- system --------------

/// the system use utf-8 as unified encoding, must be pre of includes for its usage.
#ifndef UNICODE
#define UNICODE
#endif

/// common in windows and linux
#include <stdio.h>
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

 
#define BudaMc(size) (char*)alloc(size)
#define BudaM(T) (T*)alloc(sizeof(T))
#define BudaMn(T, name) T *name = (T*)alloc(sizeof(T))
#define BudaMn2(T1, name1, T2, name2) BudaMn(T1, name1); BudaMn(T2, name2)
#define BudaMn3(T1, name1, T2, name2, T3, name3) BudaMn(T1, name1); BudaMn2(T2, name2, T3, name3)

#define BudaZ(T, name) T name; memset(&name, 0, sizeof(name))

#define BudaFclose(pf) if(pf){ fclose(pf); pf = NULL; }
#define BudaFree(m) if(m){ free(m); m = NULL; }

#define BudaHex2Dec(c) (c<='9'? (c-'0'):(c<='Z'? (c-55):(c-87)))

#define BudaWriteStep(c, len, remain, sum, fail) c += len; remain -= len; sum += len; if(remain<=0) goto fail;
#define BudaWriteStep2(c, len, remain) c += len; remain -= len;
#define BudaPreWrite(max_len, dec, fail) if((max_len-=dec)<0) goto fail;
#define BudaWriteCheck(max_len, len, fail) if(max_len<=len) goto fail;

#define BudaMax(a, b) ((a)<(b)?(b):(a))




namespace BUDA
{

//-------------------- structs ---------------------------
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
extern const u_char BYTE_LOW4_MASK;
extern char* HEXS;
extern const HttpMime HttpMimes[];
extern const int HttpMimesLen;

extern const int PATH_MAX_1;
extern const int TIME_BUF_SIZE;
extern const int TIME_BUF_SIZE1;
extern const int SOCK_BUF_RECV_SIZE;      // 8K
extern const int SOCK_BUF_SEND_SIZE_MAX;  // about 200M, controled by MemChain
extern const int SOCK_BUF_SEND_SIZE_INIT; // about 10M, it's just the init size, the memory need will be allocated by MemChain
extern const int SOCK_CONN_QUEUE_MAX;
extern const int SOCK_PORT_MAX;
extern char *VERSION;	
extern char *conf_path;

extern MemChain *conf;
extern char* hostname;
extern char* admin_pw;
extern char *log_dir;
extern char log_input_dir[];
extern FILE* log_file;
extern struct timespec last_log_time;
extern char web_root[]; // the real path of web_root dir
extern int web_root_len; 
extern int server_listen_port;
extern char *pattern_log_time;
extern regex_t regex_log_time;



// calloc memory, log fail message
void* alloc(int size);
// MUST use free_mem_chain to free heap mem used
// return NULL for failure
MemChain* create_mem_chain(int max_size=100002048, int block_min_size=2048);
// destroy the whole mem_chain
void free_mem_chain(MemChain *mc);
// delete all the blocks except the first one, reset mem_chain to the beginning
void reset_mem_chain(MemChain *mc);
// return NULL if failed
MemChainBlock* mem_chain_add_block(MemChain *mc, int size);
// return new mc.content_used, mc2 will be destroyed
int concat_mem_chain(MemChain *mc, MemChain *mc2);
// If the memory size to use is known to be size, use this function; otherwise, use the next function. 
// Return NULL if failed, return the start of used memory if succeed.
char* use_mem_chain(MemChain *mc, int size, char* content=NULL);
// Return NULL if failed, return the start of used memory if succeed.
char* use_mem_chain(MemChain *mc, const char* format, ...);


//-------------------- string.cpp ---------------------------

// check the result len and log error info
// Return -1 for error, and len for succeed
int vsnprintf2 (char *s, size_t size, const char *format, va_list args) ;
// check the result len and log error info
// Return -1 for error, and len for succeed
int snprintf2 (char *s, size_t size, const char *format, ...);


// return -1 for invalid user input, 0 for good input with no match
int check_user_input_for_log(char* input);
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
// Pay attention to not log inside itself
// return 0 for success, return -1 for failure
int log_start();
// Pay attention to not log inside itself
void log(const char *format, ...);
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

/* type=t (time): 2024-06-25 13:40:33
   type=d (date): 2024-06-25
   type=w (week): Fri, 22 May 2009 06:07:21 GMT
   type=f (filename): 2024_06_25_13_40_33
   use_gmt is bool value, indicate using GMT/Local time
   disable_log is bool value, indicate using log() in it
   return -1 for failure, return written lengh for success
*/
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
int realpath2(char* input_path, char *real_path);

/* file_dir is 1 for file, 2 for dir, 3 for file or dir. return 0 if exists, -1 for not exist*/
int file_dir_exist(const char* path, int file_dir);

/* Make sure the path is not user input info.
   return NULL for faiure; 
   return path for existence or create success. */
char* dir_create(char* path);
/* Make sure the name and parent are not user input info.
   return NULL for failure, 
   return created dir real path for create success or existence. 
   caller should free(r) if not NULL.  */
char* dir_create(const char* name, const char* parent);
// overwrite the content of this file, create it if not exist
// return written bytes for success, return -1 for failure
int file_write(char* path, char* buf, int buf_size);


// return -1 for failure, return file size for sucess
int get_file_content(FILE* pf, int file_size, MemChain *mc);
// Make sure the path is not user input info.
// return -1 for failure, return file size for sucess
int get_file_content(char *path, MemChain *mc);
// return NULL if failed, return open file for success
FILE* get_file_info_open(char *input_path, struct ::stat *file_info);

// return -1 if failed, return 0 for succeed
typedef int (*process_entry)(struct dirent *entry, void* arg);
// make sure the dir_path is existing
// return -1 if failed, return 0 for succeed
int iterate_dir(const char *dir_path, process_entry processor, void* arg);
// return -1 if failed, return 0 for succeed
int concat_filename(struct dirent *entry, void* filenames);
// return -1 if failed, return 0 for succeed
int get_dir_filenames(const char *dir_path, MemChain *mc);


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


}


#endif