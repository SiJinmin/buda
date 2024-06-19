
#pragma once
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



#define VERSION "1.0.0"
#define SERVER_LISTEN_PORT   8888
#define SOCK_BUF_RECV_SIZE  10241  // about 10K
#define SOCK_BUF_RECV_SIZE1 10240 // subtract 1 for the null terminator at the buf end
#define SOCK_BUF_SEND_SIZE  2000001 // about 2M, default max stack memory is 8M
#define SOCK_BUF_SEND_SIZE1 2000000
#define SOCK_CONN_QUEUE_MAX 3
#define MODE_show_client_messages "show_client_messages"
#define MODE_http_single_thread "http_single_thread"

enum MODE
{
  SHOW_CLIENT_MESSAGES=1, HTTP_SINGLE_THREAD
};


typedef int (*FUN_process_connection_sock)(int sock, char* buf_recv, int buf_recv_size, char* buf_send, int buf_send_size); 



#define BudaZero(T, name) T name; memset(&name, 0, sizeof(name))


// time.c
struct tm *localtime_s(struct tm *result, time_t *timep);
struct tm * gmtime_s(struct tm *result, time_t *timep);

// string.c
/* get token ended by end in the current line, set the end char to \0 as found token end.
   if not found the token, *start remain origin value, *token=NULL, return -1.
   if found, *start is the next char position of end char, *token is found position, return 0.*/
int get_token_by_char_end(char** start, char end, char** token);


// http.c
typedef struct http_req
{
  char* method;
  char* path;
  int content_len;
  char* content;
} HttpReq;

int parse_http_request(char* req, HttpReq* r);
char* make_http_response(char* StringBuf, int StringBufSize, int status_number, char* status_message, char* content, int content_len, char* content_type, char* encoding, char* filename);

// help.c
void print_help(char* program_name);





#endif