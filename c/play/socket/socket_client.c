/*
编译生成可执行程序（Ubuntu Linux 24.04）
# gcc ./socket_client.c -o socket_client
# ./socket_client
*/

#include <stdio.h>      // printf
#include <string.h>	    // strlen(char*)
#include <stdlib.h>     // free(*memory)
#include <unistd.h>     // close(int file_descriptor): close socket
#include <sys/socket.h> // socket(), bind(), accept(), etc
#include <netdb.h>      // gethostbyname(char* domain_name)
#include <arpa/inet.h>	// long inet_addr(char* ip_address) : convert ip string to long int format

#define server_listen_port 8888
#define SOCK_BUF_SIZE 10241
#define SOCK_BUF_SIZE1 10240 // subtract 1 for the null terminator at the buf end
#define SERVER_DOMAIN_NAME "idealand.space"
#define BUDA_F(p) if(p){ free(p); p=NULL; } 

int main(int argc, char * argv[])
{
	int r=0;

	struct in_addr * ip_addr=get_sock_addr(SERVER_DOMAIN_NAME);
	if(ip_addr==NULL) goto fail;

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET; // IPv4
	server_addr.sin_addr = *ip_addr;	
	server_addr.sin_port = htons(server_listen_port);
	socklen_t addrlen = sizeof(struct sockaddr_in);

	int sock;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) { perror("create client socket failed\n"); goto fail; 	}
	printf("create client socket succeed\n");

	if (connect(sock , (struct sockaddr *)&server_addr , addrlen) == -1) { perror("connect to server failed\n"); goto fail_close; }
	printf("connect to server succeed\n");

	char *line = NULL; size_t len = 0; ssize_t read;
	while((read = getline(&line, &len, stdin)) > 0)
	{			
		if(send(sock, line, read,	0) <= 0) goto fail_close;
		BUDA_F(line);
	}
	BUDA_F(line);

	close(sock); goto succeed;
	fail_close: close(sock); goto fail;
	succeed: return r;
	fail: r=1; return r;
}

