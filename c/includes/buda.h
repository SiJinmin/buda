

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
#define BudaFclose(pf) if(pf){ fclose(pf); pf = NULL; }

#define BudaHex2Dec(c) (c<='9'? (c-'0'):(c<='Z'? (c-55):(c-87)))


namespace BUDA
{


//--------------- constants --------------
static const u_char LOW_BYTE_MASK = 15;
static const char* HEXS = "0123456789ABCDEF";

static const char *VERSION = "1.0.0";	
static const int FILE_PATH_SIZE =  6000;
static const int FILE_PATH_SIZE1 = 5999;
static const int SOCK_BUF_RECV_SIZE = 10240; // about 10K
static const int SOCK_BUF_SEND_SIZE = 2001024; // about 2M, default max stack memory is 8M
static const int SOCK_CONN_QUEUE_MAX = 3;
static const int SOCK_PORT_MAX=65535;
static int server_listen_port =  8888;
static const char *MODE_show_client_messages = "show_client_messages";
static const char *MODE_http_single_thread = "http_single_thread";



//-------------------- string.cpp ---------------------------
/* This function is originally written for parse http request, but can be used by others.
   It gets the first token ended by end in the current line starting from *start.
   If not found, *start remains original value, *token=NULL, return -1.
   If found, set the end char to \0 as the found token end, set *start point to the next char of end char, set *token to the found token, and return 0.*/
int get_token_by_char_end(char** start, char** token, char end=' ', char line_end='\r');
int hex2dec(char c);
int url_encode(u_char *s, u_char *d, int max_len=FILE_PATH_SIZE1);
int url_decode(u_char *s, u_char *d, int max_len=FILE_PATH_SIZE1);


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

typedef struct http_mime
{
  const char* ext;
  const char* type;
  int ext_len;
} HttpMime;
static HttpMime HttpMimes[]=
{
  {"htm", "text/html", 3},
  {"html", "text/html", 4},
  {"js", "text/javascript", 2},
  {"mjs", "text/javascript", 3},
  {"jpg", "image/jpeg", 3},
  {"jpeg", "image/jpeg", 4},
  {"png", "image/png", 3},
  {"webp", "image/webp", 4},
  {"svg", "image/svg+xml", 3},
  {"css", "text/css", 3},
  {"mp3", "audio/mpeg", 3},
  {"mp4", "video/mp4", 3},
  {"json", "application/json", 4},
  {"ico", "image/vnd.microsoft.icon", 3},
  {"pdf", "application/pdf", 3},
  {"zip", "application/zip", 3},
  {"rar", "application/vnd.rar", 3},
  {"doc", "application/msword", 3},
  {"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document", 4},
  {"xls", "application/vnd.ms-excel", 3},
  {"xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", 4},
  {"ppt", "application/vnd.ms-powerpoint", 3},
  {"pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation", 4},
  {"txt", "text/plain", 3},
  {"xml", "application/xml", 3},
  {"bmp", "image/bmp", 3},
  {"wav", "audio/wav", 3},
  {"bin", "application/octet-stream", 3},
  {"avi", "video/x-msvideo", 3},
  {"csv", "text/csv", 3},

  {"aac", "audio/aac", 3},
  {"abw", "application/x-abiword", 3},
  {"apng", "image/apng", 4},
  {"arc", "application/x-freearc", 3},
  {"avif", "image/avif", 4},
  {"azw", "application/vnd.amazon.ebook", 3},
  {"bz", "application/x-bzip", 2},
  {"bz2", "application/x-bzip", 3},
  {"cda", "application/x-cdf", 3},
  {"csh", "application/x-csh", 3},
  {"eot", "application/vnd.ms-fontobject", 3},
  {"epub", "application/epub+zip", 4},
  {"gz", "application/gzip", 2},
  {"ics", "text/calendar", 3},
  {"jar", "application/java-archive", 3},
  {"jsonld", "application/ld+json", 6},
  {"midi", "audio/midi", 4},
  {"mid", "audio/x-midi", 3},
  {"mpeg", "video/mpeg", 4},
  {"mpkg", "application/vnd.apple.installer+xml", 4},
  {"odp", "application/vnd.oasis.opendocument.presentation", 3},
  {"ods", "application/vnd.oasis.opendocument.spreadsheet", 3},
  {"odt", "application/vnd.oasis.opendocument.text", 3},
  {"oga", "audio/ogg", 3},
  {"ogv", "video/ogg", 3},
  {"ogx", "application/ogg", 3},
  {"opus", "audio/ogg", 4},
  {"otf", "font/otf", 3},
  {"php", "application/x-httpd-php", 3},
  {"rtf", "application/rtf", 3},
  {"sh", "application/x-sh", 2},
  {"tar", "application/x-tar", 3},
  {"tif", "image/tiff", 3},
  {"tiff", "image/tiff", 4},
  {"ts", "video/mp2t", 2},
  {"ttf", "font/ttf", 3},
  {"vsd", "application/vnd.visio", 3},
  {"weba", "audio/webm", 4},
  {"webm", "video/webm", 4},
  {"woff", "font/woff", 4},
  {"woff2", "font/woff2", 5},
  {"xhtml", "application/xhtml+xml", 5},
  {"xul", "application/vnd.mozilla.xul+xml", 3},
  {"3gp", "video/3gpp", 3},
  {"3g2", "video/3gpp2", 3},
  {"7z", "application/x-7z-compressed", 2}
};
static int HttpMimesLen=sizeof(HttpMimes)/sizeof(HttpMime);

/* return 0 for succeed, return -1 for failure. */
int parse_http_request(char* req, HttpReq* r);
/* Return response size if succeed, or -1 for failure.
   encoding should be auto detected and reset by content_type
   content_len = -1 means it should be caculated by content
*/
int make_http_response(char* StringBuf, int StringBufSize, char* content=NULL, int content_len=-1, const char* content_type="text/html", const char* encoding="UTF-8", int status_no=200, const char* status_code="OK", char* filename=NULL);
int make_http_response_file(char* buf, int buf_size, char* web_root, int web_root_len, const char* url, const char* content_type="text/html");



//--------------------------- help.cpp ---------------------------
void print_help(char* program_name);


}


#endif