/*
编译生成可执行程序（Ubuntu Linux 24.04）
# gcc ./socket_client.c -o socket_client
# ./socket_client
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
#define SERVER_DOMAIN_NAME "idealand.space"


/* 通过域名获取socket可用的ip地址格式，用于连接server socket 
   hostname: 例如 idealand.space */
struct in_addr * get_sock_addr(char *hostname)
{
	printf("resolving %s to IP address...\n" , hostname);
	struct hostent *he;
	struct in_addr **addr_list; // data in struct in_addr is long int ip
	int i;
		
	if ( (he = gethostbyname( hostname ) ) == NULL )  goto fail;
	
	addr_list = (struct in_addr **) he->h_addr_list;
	struct in_addr * ip_addr=NULL;	
	for(i = 0; (ip_addr=addr_list[i]) != NULL; i++) 
	{
		printf("%d: %s\n", i+1 , inet_ntoa(*ip_addr) ); // inet_ntoa convert an IP address in long int format to dotted format
		goto succeed; // use the first IP address 
	}	
	if(ip_addr==NULL) goto fail;

	succeed: return ip_addr;
	fail: printf("gethostbyname for %s failed", hostname); return NULL;
}


/* 查看和设置socket发送和接收数据的超时时长，设置为5秒超时 */
/*int set_socket_options(int socket)
{
    static socklen_t len = sizeof(struct timeval); 
	struct timeval SocketTimeout, RecvTimeout, SendTimeout; 
	SocketTimeout.tv_sec = 5; SocketTimeout.tv_usec = 0; 
	RecvTimeout.tv_sec = RecvTimeout.tv_usec = 0; SendTimeout.tv_sec = SendTimeout.tv_usec = 0;

	getsockopt(socket, SOL_SOCKET,SO_RCVTIMEO, &RecvTimeout, &len); printf("recv timeout is %ld.%ld\n",RecvTimeout.tv_sec, RecvTimeout.tv_usec );
	getsockopt(socket, SOL_SOCKET,SO_SNDTIMEO, &SendTimeout, &len); printf("send timeout is %ld.%ld\n",SendTimeout.tv_sec, SendTimeout.tv_usec );
    if(setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &SocketTimeout, len)==-1) { printf("set receive timeout failed\n"); goto fail; }
    if(setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, &SocketTimeout, len)==-1) { printf("set send timeout failed\n"); goto fail; }
	getsockopt(socket, SOL_SOCKET,SO_RCVTIMEO, &RecvTimeout, &len); printf("recv timeout is %ld.%ld\n",RecvTimeout.tv_sec, RecvTimeout.tv_usec );
	getsockopt(socket, SOL_SOCKET,SO_SNDTIMEO, &SendTimeout, &len); printf("send timeout is %ld.%ld\n",SendTimeout.tv_sec, SendTimeout.tv_usec );

	succeed: return 0;
	fail: return -1;
}*/



int main(int argc, char * argv[])
{
	int r=0;

	struct in_addr * ip_addr=get_sock_addr(SERVER_DOMAIN_NAME);
	if(ip_addr==NULL) goto fail;

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET; // IPv4
	server_addr.sin_addr = *ip_addr;	
	server_addr.sin_port = htons(SERVER_LISTEN_PORT);
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
	}

	close(sock); goto succeed;
	fail_close: close(sock); goto fail;
	succeed: return r;
	fail: r=1; return r;
}
