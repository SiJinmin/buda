#include "../includes/buda.h"

namespace BUDA
{

//------------global externs definitions-------------

const u_char BYTE_LOW4_MASK = 15;
const char* HEXS = "0123456789ABCDEF";
const char* URL_ALLOW = "abcdefghijklmnopqrstuvwxyz/:#?&=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ+-,._*~@!$^()[]{}';|";

const char *Space_nl=" \t\r\n";
const char *Space_nl_quote=" \t\r\n\"";
const char *Space_nl_quote_colon=" \t\r\n\":";
const char *Endbrace_comma=",}";
const char *Endbracket_comma=",]";

const int PATH_MAX_1 = PATH_MAX - 1;
const int TIME_BUF_SIZE = 2048;
const int TIME_BUF_SIZE1 = TIME_BUF_SIZE - 1;
const int SOCK_BUF_RECV_SIZE = 8192;
const int SOCK_BUF_SEND_SIZE_MAX = 200008192;
const int SOCK_BUF_SEND_SIZE_INIT = 10008192;
const int SOCK_CONN_QUEUE_MAX = 3;
const int SOCK_PORT_MAX = 65535;
const char *VERSION = "1.0.0";	
const char *Conf_path = "/etc/buda.conf";
const char *Log_dir = "/var/log/buda/";
const char *Pattern_log_time = "\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\]\\[[0-9]+\\.[0-9]+\\]";


char log_input_dir[30];
FILE* log_file=NULL;
struct timespec last_log_time = {0, 0};
regex_t regex_log_time;
const char* admin_pw="Buda123456";
char web_root[PATH_MAX]="../../vue/dist/"; 
int web_root_len = 0; 
int server_listen_port = 8888;



//------------end of global externs definitions-------------

void read_conf()
{
	Link *mem=link_create();

  char* content=NULL; int len; if((len=file_content_get((char*)Conf_path, NULL, mem, &content)) > 0) 
	{
		char *pc=content, *pc_end=pc + len, c;
		while(pc < pc_end)
		{
			c=*pc; if(c=='#' || c=='\r' || c=='\n') {pc=next_line(pc); if(pc==NULL) break; else continue;}
			if(strncmp(pc, "pw=", 3)==0 && pc+3<pc_end)
			{
				admin_pw=pc+3; pc=set_next_space_0(pc, pc_end);
			} else pc++;
		}
		// printf("admin_pw: %s\n", admin_pw);
	}

	BudaF(mem);
}

typedef int (*FUN_process_connection_sock)(int sock, char* buf_recv, int buf_recv_size, MemChain* sender); 
	
// server waits for messages from client, and display messages in server console
// the listen socket and connection sockets work in the same single thread, so the queued clients may be blocked for a very long time.
int show_client_messages_single_thread(int sock, char* buf_recv, int buf_recv_size, MemChain* sender)
{
	long recv_size=0, buf_recv_size1=buf_recv_size-1;
  while((recv_size = recv(sock, buf_recv, buf_recv_size1, 0))>0)
	{
		buf_recv[recv_size]=0; // if(check_user_input_for_log(buf_recv)) goto fail;
		log("[received %ld bytes data]\n%s\n[end of received data]", recv_size, buf_recv);
	}
	log("receive from client failed");  goto fail;
	
	fail: return -1;
}

/* process a http request, return 0 for succeed, -1 for failure.  */
int http_single_thread(int sock, char* buf_recv, int buf_recv_size, MemChain* sender)
{
	int r=0; BudaZ(HttpReq, req); int recv_size=0; MemChainBlock *mcb=sender->first; 

  recv_size = recv(sock, buf_recv, buf_recv_size-1, 0);	if(recv_size<=0) { log("receive from client socket failed"); goto fail;}
	buf_recv[recv_size]=0; // if(check_user_input_for_log(buf_recv)) goto fail;
	log("[received %ld bytes from client]\n%s\n[end of recv]", recv_size, buf_recv);

	if(parse_http_request(buf_recv, &req)) goto fail;

	if(http_route(&req, sender)==-1) // if route failed, use the default file as reponse
	{
		if(make_http_response_file(sender, req.path, NULL)<0) goto fail;	
	}
	
	while(mcb) 
	{ 
		if(mcb->used>0) { if(send(sock, mcb->mem, mcb->used, 0) < 1){ log("sent to client error: %d", r); goto fail; } }
		mcb=mcb->next; 
	}
	log("[sent to client %d bytes]", sender->content_used);

	succeed: return 0;
	fail: return -1;
}


int main(int argc, char * argv[])
{
	show_sys_info(); json_test_load(); return 0;

	sprintf(log_input_dir, "%s%s", Log_dir, "input/");
	if(log_start()<0) return -1;
	read_conf(); //return 0;

	int r=0; int optc; char *program_name = argv[0]; 
	FUN_process_connection_sock fun_sock = http_single_thread; char *web_root_input = NULL;

	if(argc==1) goto need_help; else goto command_start;
	need_help: print_help(program_name);	exit(EXIT_SUCCESS);

  command_start:
	while ((optc = getopt_long(argc, argv, "hmw:p:", NULL, NULL)) != -1)
	{
		switch (optc) 
		{
			case 'm':
				fun_sock=show_client_messages_single_thread; 
				log("server works in client_messages_single_thread mode");
				goto command_end;
			case 'w':
				web_root_input = optarg; 
				if(realpath2(web_root_input, web_root)<0){ return -1; }
				struct stat file_info; r = stat(web_root, &file_info); if (r) { log("web root does not exists: %s", web_root); return -1; }
				if(!(file_info.st_mode & S_IFDIR)){ log("web root is not a directory: %s", web_root); return -1; }
				web_root_len=strlen(web_root); log("web server root path: %s", web_root); 
				break;
			case 'p':
				server_listen_port = atoi(optarg); 
				if(server_listen_port<1 || server_listen_port>=SOCK_PORT_MAX) { log("port should in 1 ~ %d", SOCK_PORT_MAX); goto need_help; }
				else log("web server listen port: %d", server_listen_port); 
				break;
			default:
				goto need_help;
		}
	}	
  command_end:

	int listen_sock, sock; // sock is the client connection socket
	struct sockaddr_in server_addr, client_addr; socklen_t addrlen = sizeof(struct sockaddr_in), addrlen_client=0;
  long client_count=0;	char *client_ip=NULL; int client_port=-1;
	char buf_recv[SOCK_BUF_RECV_SIZE];
	MemChain* sender=mem_chain_create(SOCK_BUF_SEND_SIZE_MAX, SOCK_BUF_SEND_SIZE_INIT); if(sender==NULL) goto fail;

	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) { log("create server listen socket failed"); goto fail; 	}
	log("create server listen socket succeed");

	server_addr.sin_family = AF_INET; /* IPv4 */ 
	server_addr.sin_port = htons(server_listen_port); server_addr.sin_addr.s_addr = INADDR_ANY;	// 监听所有本地IP地址
	
	if (bind(listen_sock, (struct sockaddr*)&server_addr, addrlen) == -1) 
	{ log("server listen socket bind to port %d failed", server_listen_port); goto fail_close; }
	log("bind server listen socket to port %d succeed", server_listen_port);

	if (listen(listen_sock, SOCK_CONN_QUEUE_MAX) == -1) { log("server socket start listening failed"); goto fail_close; }
	log("server socket starts listening ...");

	while((sock	= accept(listen_sock, (struct sockaddr*)&client_addr,	&addrlen_client)) > 0)
	{			
		client_count++; client_ip = inet_ntoa(client_addr.sin_addr); client_port = ntohs(client_addr.sin_port);
		log("%ld --- server socket accepted a client connection:  %s:%d ---", client_count, client_ip, client_port);

		if(set_socket_options(sock)) goto end_close_client;
		mem_chain_reset(sender); 
    if(fun_sock(sock, buf_recv, SOCK_BUF_RECV_SIZE, sender) < 0) goto end_close_client;

		end_close_client: close(sock);
		log("waiting for next client ...");
	}

	fail_close: close(listen_sock); 
	fail: r=-1; return r;
}


}

int main(int argc, char * argv[])
{
  return BUDA::main(argc, argv);
}