

#ifndef buda_h
#define buda_h


/// system defines, must be pre of includes for its usage
/* the system use utf-8 as unified encoding. */
#ifndef UNICODE
#define UNICODE
#endif


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

#include <unistd.h>
#include <stddef.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/time.h>
#include <linux/input.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define BudaZero(T, name) T name; memset(&name, 0, sizeof(name))


namespace BUDA
{

static const char* VERSION = "1.0.0";	
static const int SERVER_LISTEN_PORT =  8888;
static const int SOCK_BUF_RECV_SIZE = 10241 ; // about 10K
static const int SOCK_BUF_RECV_SIZE1 = 10240 ;// subtract 1 for the null terminator at the buf end
static const int SOCK_BUF_SEND_SIZE = 2000001; // about 2M, default max stack memory is 8M
static const int SOCK_BUF_SEND_SIZE1 = 2000000;
static const int SOCK_CONN_QUEUE_MAX = 3;
static const char *MODE_show_client_messages = "show_client_messages";
static const char *MODE_http_single_thread = "http_single_thread";


enum MODE
{
  SHOW_CLIENT_MESSAGES=1, HTTP_SINGLE_THREAD
};


//-------------------- string.cpp ---------------------------

/* get token ended by end in the current line, set the end char to \0 as found token end.
   if not found the token, *start remain origin value, *token=NULL, return -1.
   if found, *start is the next char position of end char, *token is found position, return 0.*/
int get_token_by_char_end(char** start, char end, char** token);


//--------------------------- time.cpp ---------------------------
struct tm * gmtime_s(struct tm *result, time_t *timep);
struct tm *localtime_s(struct tm *result, time_t *timep);


//--------------------------- http.cpp ---------------------------

typedef struct http_req
{
  char* method;
  char* path;
  int content_len;
  char* content;
} HttpReq;

int parse_http_request(char* req, HttpReq* r);

int make_http_response(char* StringBuf, int StringBufSize, char* content=NULL, int content_len=-1, const char* content_type="text/html", const char* encoding="UTF-8", int status_no=200, const char* status_code="OK", char* filename=NULL);


//--------------------------- help.cpp ---------------------------
void print_help(char* program_name);


}


#endif