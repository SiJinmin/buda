#include "../includes/buda.h"

namespace BUDA
{

static char web_root[PATH_MAX]; static int web_root_len;
typedef int (*FUN_process_connection_sock)(int sock, char* buf_recv, int buf_recv_size, char* buf_send, int buf_send_size); 

	
// server waits for messages from client, and display messages in server console
// the listen socket and connection sockets work in the same single thread, so the queued clients may be blocked for a very long time.
int show_client_messages_single_thread(int sock, char* buf_recv, int buf_recv_size, char* buf_send, int buf_send_size)
{
	long recv_size=0, buf_recv_size1=buf_recv_size-1;
  while((recv_size = recv(sock, buf_recv, buf_recv_size1, 0))>0)
	{
		buf_recv[recv_size]=0; // set string end
		log("[received %ld bytes data]\n%s\n[end of received data]", recv_size, buf_recv);
	}
	log("receive from client failed"); return -1;
}


/* process a http request, return 0 for succeed, -1 for failure.  */
int http_single_thread(int sock, char* buf_recv, int buf_recv_size, char* buf_send, int buf_send_size)
{
	int r=0; BudaZ(HttpReq, req); int recv_size=0, send_size;

  recv_size = recv(sock, buf_recv, buf_recv_size-1, 0);	if(recv_size<=0) { log("receive from client socket failed"); goto fail;}
	buf_recv[recv_size]=0; log("[received %ld bytes from client]\n%s\n[end of recv]", recv_size, buf_recv);

	if(parse_http_request(buf_recv, &req)) goto fail;

	if(http_route(&req)) // if route failed, use the default file as reponse
	{
		send_size = make_http_response_file(buf_send, buf_send_size, web_root, web_root_len, req.path, NULL); if(send_size<=0) goto fail;	
	}
	
	r = send(sock, buf_send, send_size, 0);

	/* const char* test= "HTTP/1.1 200 OK\r\nDate: Fri, 22 May 2009 06:07:21 GMT\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
	send(sock, test, strlen(test)+1, 0);
	test= "<html><body>hello world 2</body></html>";
	send(sock, test, strlen(test)+1, 0); */

	if(r==send_size) log("[sent to client %d bytes]", r);
	else { log("sent to client error: %d", r); goto fail; }

	succeed: return 0;
	fail: return -1;
}


int main(int argc, char * argv[])
{
	if(log_start()<0) return -1;

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
				if(realpath(web_root_input, web_root)!=web_root){ log("web root path error: %s", web_root_input); return -1; }
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
	char buf_recv[SOCK_BUF_RECV_SIZE]; char *buf_send = (char *)malloc(SOCK_BUF_SEND_SIZE);

	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) { log("create server listen socket failed"); goto fail; 	}
	log("create server listen socket succeed");

	server_addr.sin_family = AF_INET; // IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY;	// 监听所有本地IP地址
	server_addr.sin_port = htons(server_listen_port);
	if (bind(listen_sock, (struct sockaddr*)&server_addr, addrlen) == -1) 
	{ log("server listen socket bind to port %d failed", server_listen_port); goto fail_close; }
	log("bind server listen socket to port %d succeed", server_listen_port);

	if (listen(listen_sock, SOCK_CONN_QUEUE_MAX) == -1) { log("server socket start listening failed"); goto fail_close; }
	log("server socket starts listening ...");

	while((sock	= accept(listen_sock, (struct sockaddr*)&client_addr,	&addrlen_client)) > 0)
	{			
		client_count++; client_ip = inet_ntoa(client_addr.sin_addr); client_port = ntohs(client_addr.sin_port);
		log("\n%ld --- server socket accepted a client connection:  %s:%d ---", client_count, client_ip, client_port);

		// if(set_socket_options(sock) == -1) goto end_close_client;
    if(fun_sock(sock, buf_recv, SOCK_BUF_RECV_SIZE, buf_send, SOCK_BUF_SEND_SIZE) < 0) goto end_close_client;

		end_close_client: close(sock);
		log("waiting for next client ...");
	}

	fail_close: close(listen_sock); 
	fail: r=-1; return r;
}


}

int main(int argc, char * argv[])
{
	//char a=0b10000000; u_char b=0b10000000; a=(a>>4); b=(b>>4); printf("a=%d, b=%d\n", (int)a, (int)b);return 0;
  return BUDA::main(argc, argv);
}