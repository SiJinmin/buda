

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
#define BudaZero(T, name) T name; memset(&name, 0, sizeof(name))



namespace BUDA
{


//--------------- constants --------------
static const char *VERSION = "1.0.0";	
static const int SERVER_LISTEN_PORT =  8888;
static const int SOCK_BUF_RECV_SIZE = 10240 ; // about 10K
static const int SOCK_BUF_SEND_SIZE = 2001024; // about 2M, default max stack memory is 8M
static const int SOCK_CONN_QUEUE_MAX = 3;
static const char *MODE_show_client_messages = "show_client_messages";
static const char *MODE_http_single_thread = "http_single_thread";



//-------------------- string.cpp ---------------------------
/* This function is originally written for parse http request, but can be used by others.
   It gets the first token ended by end in the current line starting from *start.
   If not found, *start remains original value, *token=NULL, return -1.
   If found, set the end char to \0 as the found token end, set *start point to the next char of end char, set *token to the found token, and return 0.*/
int get_token_by_char_end(char** start, char** token, char end=' ', char line_end='\r');



//--------------------------- time.cpp ---------------------------
struct tm *gmtime_s(struct tm *result, time_t *timep);
struct tm *localtime_s(struct tm *result, time_t *timep);



//--------------------------- http.cpp ---------------------------
typedef struct http_req
{
  char* method;
  char* path;
  char* content;
  int content_len;
} HttpReq;

/* return 0 for succeed, return -1 for failure. */
int parse_http_request(char* req, HttpReq* r);
/* Return response size if succeed, or -1 for failure.
   encoding should be auto detected and reset by content_type
   content_len = -1 means it should be caculated by content
*/
int make_http_response(char* StringBuf, int StringBufSize, char* content=NULL, int content_len=-1, const char* content_type="text/html", const char* encoding="UTF-8", int status_no=200, const char* status_code="OK", char* filename=NULL);



//--------------------------- help.cpp ---------------------------
void print_help(char* program_name);


}


#endif