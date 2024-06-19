/*
编译生成可执行程序（Ubuntu Linux 24.04）
debug版
# cd debug
# gcc ../src/socket_server.c ../lib/*.c -g -o ../debug/socket_server
release版
# gcc ./socket_server.c -o socket_server

# scp -r c root@idealand.space:/root/code/buda/
启动http server
# ./socket_server http_single_thread
启动client message display
# ./socket_server show_client_messages

查看防火墙的状态：(国内的云服务器80,443,8080,8443是备案端口，必须备案以后才能从外网访问，注意避免使用)
# ufw status
# ufw allow 8888
查看端口是否被其它程序占用
# sudo apt lsof
# lsof -i:8888
*/

#include "../includes/buda.h"


// server waits for messages from client, and display messages in server console
// the listen socket and connection sockets work in the same single thread, so the queued clients may be blocked for a very long time.
int show_client_messages_single_thread(int sock, char* buf_recv, int buf_recv_size, char* buf_send, int buf_send_size)
{
	long recv_size=0, buf_recv_size1=buf_recv_size-1;
  while((recv_size = recv(sock, buf_recv, buf_recv_size1, 0))>0)
	{
		if(recv_size<0) { printf("receive from client failed\n"); return -1;}
		buf_recv[recv_size]=0; // set string end
		printf("received %ld bytes data: \n%s\n", recv_size, buf_recv);
	}
}

// server waits for a message from client, response it, and close the connection, then wait for the next client to connect
// the listen socket and connection sockets work in the same single thread
int http_single_thread(int sock, char* buf_recv, int buf_recv_size, char* buf_send, int buf_send_size)
{
	int r=0;
	long recv_size=0, buf_recv_size1=buf_recv_size-1;
  recv_size = recv(sock, buf_recv, buf_recv_size1, 0);
	
	if(recv_size<0) { printf("receive from client socket failed\n"); return -1;}
	buf_recv[recv_size]=0; // set string end
	printf("[received %ld bytes from client]\n%s\n[end of recv]\n", recv_size, buf_recv);
	BudaZero(HttpReq, req); parse_http_request(buf_recv, &req);

  make_http_response(buf_send, buf_send_size, 0, NULL, req.path, -1, "text/plain", "UTF-8", NULL);
	r = send(sock, buf_send, buf_send_size, 0);
	if(r>0) printf("[sent to client]\n%s\n[end of sent]\n", buf_send);
	return r;
}


int main(int argc, char * argv[])
{
	int r=0; 
	enum MODE mode=HTTP_SINGLE_THREAD; FUN_process_connection_sock fun_sock=http_single_thread;	
	char* web_root=NULL;
	
	int optc; char *program_name = argv[0];

	while ((optc = getopt_long(argc, argv, "hmw:", NULL, NULL)) != -1)
	{
			switch (optc) 
			{
				case 'h':
							print_help(program_name);
							exit(EXIT_SUCCESS);
				case 'm':
							mode=SHOW_CLIENT_MESSAGES; 
							fun_sock=show_client_messages_single_thread; 
							printf("server works in client_messages_single_thread mode\n");
							break;
					case 'w':
							web_root = optarg;
							break;
			}
	}

	if(mode==HTTP_SINGLE_THREAD) printf("starting http web server: %s\n", web_root);

	int listen_sock, sock; // sock is the client connection socket
	struct sockaddr_in server_addr, client_addr; socklen_t addrlen = sizeof(struct sockaddr_in), addrlen_client=0;
  int client_count=0;	char *client_ip=NULL; int client_port=-1;
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
		printf("%d --------------- server socket accepted a client connection:  %s:%d ---------------\n", client_count, client_ip, client_port);
		// if(set_socket_options(sock) == -1) goto end_close_client;
    if(fun_sock(sock, buf_send, SOCK_BUF_SEND_SIZE, buf_recv, SOCK_BUF_RECV_SIZE) < 0) goto end_close_client;
		end_close_client: close(sock);
		printf("waiting for next client ...\n");
	}

	close(listen_sock); goto succeed;
	fail_close: close(listen_sock); goto fail;
	succeed: return r;
	fail: r=-1; return r;
}
