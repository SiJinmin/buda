

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


//--------------- constants --------------
static const u_char LOW_BYTE_MASK = 15;
static const char* HEXS = "0123456789ABCDEF";

static const char *VERSION = "1.0.0";	
static const int PATH_MAX_1 = PATH_MAX - 1;
static const int TIME_BUF_SIZE = 2048;
static const int TIME_BUF_SIZE1 = TIME_BUF_SIZE - 1;
static const char *log_dir = "../../../log/"; // ~/code/log

static const int SOCK_BUF_RECV_SIZE = 8192;     // 8K
static const int SOCK_BUF_SEND_SIZE = 20008192; // about 20M, default max stack memory is 8M, so must use heap
static const int SOCK_CONN_QUEUE_MAX = 3;
static const int SOCK_PORT_MAX = 65535;
static const char *MODE_show_client_messages = "show_client_messages";
static const char *MODE_http_single_thread = "http_single_thread";

//--------------- global vars --------------
static int server_listen_port = 8888;
static FILE* log_file = NULL;
static struct timespec last_log_time = {0, 0};


//-------------------- mem.cpp ---------------------------
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
  int blocks_used;     // sum of used bytes of blocks
  int block_min_size;
  int block_count;
} MemChain;

// calloc memory, log fail message
void* alloc(int size);
// MUST use free_mem_chain to free heap mem used
MemChain* create_mem_chain(int max_size=100002048, int block_min_size=2048);
void free_mem_chain(MemChain *mc);
// return NULL if failed
MemChainBlock* mem_chain_add_block(MemChain *mc, int size);
// If the memory size to use is known to be size, use this function; otherwise, use the next function. 
// Return NULL if failed, return the start of used memory if succeed.
char* use_mem_chain(MemChain *mc, int size);
// Return NULL if failed, return the start of used memory if succeed.
char* use_mem_chain(MemChain *mc, char* format, ...);


//-------------------- string.cpp ---------------------------
int log_start();
void log(const char *format, ...);
/* This function is originally written for parse http request, but can be used by others.
   It gets the first token ended by end in the current line starting from *start.
   If not found, *start remains original value, *token=NULL, return -1.
   If found, set the end char to \0 as the found token end, set *start point to the next char of end char, set *token to the found token, and return 0.*/
int get_token_by_char_end(char** start, char** token, char end=' ', char line_end='\r');
int hex2dec(char c);
int url_encode(u_char *s, u_char *d, int max_len=PATH_MAX_1);
int url_decode(u_char *s, u_char *d, int max_len=PATH_MAX_1);


//--------------------------- time.cpp ---------------------------
int time_text(char *r, int max_len);
int time_text_filename(char *r, int max_len);
int time_text_date(char *r, int max_len);
int time_text_http_response(char *r, int max_len);


//--------------------------- http.cpp ---------------------------
typedef struct http_req
{
  char* method;
  char* path;
  char* content;
  int content_len;
} HttpReq;

#include "mime.h"

/* return 0 for succeed, return -1 for failure. */
int parse_http_request(char* req, HttpReq* r);
/* Return response size if succeed, or -1 for failure.
   encoding should be auto detected and reset by content_type
   content_len = -1 means it should be caculated by content  */
int make_http_response(char* StringBuf, int StringBufSize, char* content=NULL, int content_len=-1, const char* content_type="text/html", const char* encoding="UTF-8", int status_no=200, const char* status_code="OK", char* filename=NULL);
int make_http_response_file(char* buf, int buf_size, char* web_root, int web_root_len, const char* url, const char* content_type="text/html");
int http_route(HttpReq* req);


//--------------------------- help.cpp ---------------------------
void print_help(char* program_name);


}


#endif