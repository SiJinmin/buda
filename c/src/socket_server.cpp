#include "../includes/buda.h"

namespace BUDA
{

static char web_root[FILE_PATH_SIZE]; static int web_root_len;
typedef int (*FUN_process_connection_sock)(int sock, char* buf_recv, int buf_recv_size, char* buf_send, int buf_send_size); 

	
// server waits for messages from client, and display messages in server console
// the listen socket and connection sockets work in the same single thread, so the queued clients may be blocked for a very long time.
int show_client_messages_single_thread(int sock, char* buf_recv, int buf_recv_size, char* buf_send, int buf_send_size)
{
	long recv_size=0, buf_recv_size1=buf_recv_size-1;
  while((recv_size = recv(sock, buf_recv, buf_recv_size1, 0))>0)
	{
		buf_recv[recv_size]=0; // set string end
		printf("[received %ld bytes data]\n%s\n[end of received data]\n", recv_size, buf_recv);
	}
	printf("receive from client failed\n"); return -1;
}


/* process a http request, return 0 for succeed, -1 for failure.  */
int http_single_thread(int sock, char* buf_recv, int buf_recv_size, char* buf_send, int buf_send_size)
{
	int r=0; BudaZero(HttpReq, req); 
	long recv_size=0, buf_recv_size1=buf_recv_size-1; int send_size;
  recv_size = recv(sock, buf_recv, buf_recv_size1, 0);
	
	if(recv_size<=0) { printf("receive from client socket failed\n"); goto fail;}
	buf_recv[recv_size]=0; // set string end
	printf("[received %ld bytes from client]\n%s\n[end of recv]\n", recv_size, buf_recv);

	if(parse_http_request(buf_recv, &req)) goto fail;

  send_size = make_http_response_file(buf_send, buf_send_size, web_root, web_root_len, req.path, NULL); if(send_size<=0) goto fail;	
	r = send(sock, buf_send, send_size, 0);
	//if(r==send_size) printf("[sent to client %d bytes]\n", r);
	buf_send[150]=0;
	if(r==send_size) printf("[sent to client %d bytes]\n%s\n[end of sent]\n", r, buf_send);
	else { printf("sent to client error: %d\n", r); goto fail; }

	return 0;
	fail: return -1;
}


int main(int argc, char * argv[])
{
	int r=0; int optc; char *program_name = argv[0]; 
	FUN_process_connection_sock fun_sock=http_single_thread; char *web_root_input=NULL;

	if(argc==1) goto need_help; else goto command_start;
	need_help: print_help(program_name);	exit(EXIT_SUCCESS);

  command_start:
	while ((optc = getopt_long(argc, argv, "hmw:", NULL, NULL)) != -1)
	{
		switch (optc) 
		{
			case 'm':
				fun_sock=show_client_messages_single_thread; 
				printf("server works in client_messages_single_thread mode\n");
				goto command_end;
			case 'w':
				web_root_input = optarg; 
				if(realpath(web_root_input, web_root)!=web_root){ printf("web root path error: %s\n", web_root_input); return -1; }
				struct stat file_info; r = stat(web_root, &file_info); if (r) { printf("web root does not exists: %s\n", web_root); return -1; }
				if(!(file_info.st_mode & S_IFDIR)){ printf("web root is not a directory: %s\n", web_root); return -1; }
				web_root_len=strlen(web_root); printf("starting http web server: %s\n", web_root); 
				goto command_end;
			default:
				goto need_help;
		}
	}	
  command_end:

	int listen_sock, sock; // sock is the client connection socket
	struct sockaddr_in server_addr, client_addr; socklen_t addrlen = sizeof(struct sockaddr_in), addrlen_client=0;
  long client_count=0;	char *client_ip=NULL; int client_port=-1;
	char buf_recv[SOCK_BUF_RECV_SIZE], buf_send[SOCK_BUF_SEND_SIZE];

	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) { perror("create server listen socket failed\n"); goto fail; 	}
	printf("create server listen socket succeed\n");

	server_addr.sin_family = AF_INET; // IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY;	// 监听所有本地IP地址
	server_addr.sin_port = htons(SERVER_LISTEN_PORT);
	if (bind(listen_sock, (struct sockaddr*)&server_addr, addrlen) == -1) 
	{ printf("server listen socket bind to port %d failed\n", SERVER_LISTEN_PORT); goto fail_close; }
	printf("bind server listen socket to port %d succeed\n", SERVER_LISTEN_PORT);

	if (listen(listen_sock, SOCK_CONN_QUEUE_MAX) == -1) { perror("server socket start listening failed\n"); goto fail_close; }
	printf("server socket starts listening ...\n");

	while((sock	= accept(listen_sock, (struct sockaddr*)&client_addr,	&addrlen_client)) > 0)
	{			
		client_count++; client_ip = inet_ntoa(client_addr.sin_addr); client_port = ntohs(client_addr.sin_port);
		printf("%ld --------------- server socket accepted a client connection:  %s:%d ---------------\n", client_count, client_ip, client_port);

		// if(set_socket_options(sock) == -1) goto end_close_client;
    if(fun_sock(sock, buf_recv, SOCK_BUF_RECV_SIZE, buf_send, SOCK_BUF_SEND_SIZE) < 0) goto end_close_client;

		end_close_client: close(sock);
		printf("waiting for next client ...\n");
	}

	fail_close: close(listen_sock); 
	fail: r=-1; return r;
}


}

int main(int argc, char * argv[])
{
	char a=0b10000000; u_char b=0b10000000; a=(a>>4); b=(b>>4); printf("a=%d, b=%d\n", (int)a, (int)b);return 0;
  return BUDA::main(argc, argv);
}