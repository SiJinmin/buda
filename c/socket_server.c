/*
编译生成可执行程序（Ubuntu Linux 24.04）
# gcc ./socket_server.c -o socket_server
# ./socket_server

查看防火墙的状态：(国内的云服务器80,443,8080,8443是备案端口，必须备案以后才能从外网访问，注意避免使用)
# ufw status
# ufw allow 8888
查看端口是否被其它程序占用
# sudo apt lsof
# lsof -i:8888
*/

#include <stdio.h>      // printf
#include <string.h>	    // strlen(char*)
#include <unistd.h>     // close(int file_descriptor): close socket
#include <sys/socket.h> // socket(), bind(), accept(), etc
#include <netdb.h>      // gethostbyname(char* domain_name)
#include <arpa/inet.h>	// long inet_addr(char* ip_address) : convert ip string to long int format

#define SERVER_LISTEN_PORT 8888
#define SOCK_BUF_SIZE 10241
#define SOCK_BUF_SIZE1 10240 // subtract 1 for the null terminator at the buf end
#define SOCK_CONN_QUEUE_MAX 3


int main(int argc, char * argv[])
{
	int r=0;

	int listen_sock, sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);

  int client_count=0;	char *client_ip; int client_port;

	char buffer[SOCK_BUF_SIZE] = { 0 };
	long recv_size=0, send_size=0;

	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) { perror("create server listen socket failed"); goto fail; 	}
	printf("create server listen socket succeed\n");

	server_addr.sin_family = AF_INET; // IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY;	// 监听所有本地IP地址
	server_addr.sin_port = htons(SERVER_LISTEN_PORT);
	if (bind(listen_sock, (struct sockaddr*)&server_addr, addrlen) == -1) { perror("server listen socket bind to ip:port failed"); goto fail_close; }
	printf("bind server listen socket to port %d succeed\n", SERVER_LISTEN_PORT);

	if (listen(listen_sock, SOCK_CONN_QUEUE_MAX) == -1) { perror("server socket start listening failed"); goto fail_close; }
	printf("server socket starts listening ...\n");

	while((sock	= accept(listen_sock, (struct sockaddr*)&client_addr,	&addrlen)) > 0)
	{			
		client_count++;
		client_ip = inet_ntoa(client_addr.sin_addr); client_port = ntohs(client_addr.sin_port);
		printf("%d --------------- \nserver socket accepted a client connection:  %s:%d ---------------\n", client_count, client_ip, client_port);
		// if(set_socket_options(sock) == -1) goto end_close_client;
		while((recv_size = recv(sock, buffer, SOCK_BUF_SIZE1, 0))>0)
		{
			if(recv_size<0) { printf("receive from client failed\n"); goto end_close_client;}
			buffer[recv_size]=0; // set string end
			printf("received %ld bytes data: \n%s\n", recv_size, buffer);
		}
		//send_size = send(sock, hello, strlen(hello), 0); if(send_size<0) { printf("write to client failed\n"); goto end_close_client;}
		//printf("Hello message sent\n");
		end_close_client: close(sock);
		printf("waiting for next client ...\n");
	}

	close(listen_sock); goto succeed;
	fail_close: close(listen_sock); goto fail;
	succeed: return r;
	fail: r=1; return r;
}
