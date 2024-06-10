/*
pscp D:\buda\c\server.c root@idealand.space:/buda/c/
cd /buda/c && gcc server.c -o server

查看防火墙的状态：(80,443,8080,8443是备案端口，必须备案以后才能从外网访问)
# ufw status
# ufw allow 8888

# lsof -i:8888
*/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>	// long inet_addr(char*) convert ip string to long
#include <string.h>	    // strlen(char*)
#include <netdb.h>	    // hostent
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

#define ListenPort 8888
#define BufSize 10241
#define BufSize1 10240 // subtract 1 for the null terminator at the buf end
#define ListenQueueMax 3

int GetAddress(char *hostname)
{
	// char *hostname = "www.google.com";
	char ip[100]; // ip address in string format
	struct hostent *he;
	struct in_addr **addr_list; // data in struct in_addr is long int ip
	int i;
		
	if ( (he = gethostbyname( hostname ) ) == NULL)  { printf("gethostbyname for %s error", hostname); return 1; }
	
	//Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
	addr_list = (struct in_addr **) he->h_addr_list;	
	for(i = 0; addr_list[i] != NULL; i++) 
	{
		strcpy(ip , inet_ntoa(*addr_list[i]) ); // Function inet_ntoa will convert an IP address in long format to dotted format. This is just the opposite of inet_addr.
	}	
	printf("%s resolved to : %s" , hostname , ip);
}

int main(int argc, char * argv[])
{
	int listen_socket, connected_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	int ClientCount=0;
	int setsockopt_r;
  socklen_t TimevalLen = sizeof(struct timeval); 
	struct timeval ReadWriteTimeout, ReadTimeout, WriteTimeout; 
	ReadWriteTimeout.tv_sec = 5; ReadWriteTimeout.tv_usec = 0; ReadTimeout.tv_sec = ReadTimeout.tv_usec = 0; WriteTimeout.tv_sec = WriteTimeout.tv_usec = 0;
	//int sokect_option = 1;
	char buffer[BufSize] = { 0 };
	long read_size, write_size;
	char* hello = "<html><body>Hello from server</body></html>";

	if ((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) { perror("create server listen socket failed"); goto end_r; 	}
  printf("create server listen socket succeed\n");

	//if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &sokect_option, sizeof(sokect_option))) { perror("setsockopt for server listen socket failed"); exit(EXIT_FAILURE); }

	server_addr.sin_family = AF_INET;	server_addr.sin_addr.s_addr = INADDR_ANY;	server_addr.sin_port = htons(ListenPort);
	if (bind(listen_socket, (struct sockaddr*)&server_addr, addrlen) < 0) { perror("server listen socket bind to port failed"); goto end_close; }
  printf("bind server listen socket to port %d succeed\n", ListenPort);

	if (listen(listen_socket, ListenQueueMax) < 0) { perror("server socket start listening failed"); goto end_close; }
  printf("server socket is listening\n");

  while((connected_socket	= accept(listen_socket, (struct sockaddr*)&client_addr,	&addrlen)) > 0)
  {
		ClientCount++;
		char *client_ip = inet_ntoa(client_addr.sin_addr); int client_port = ntohs(client_addr.sin_port);
    printf("%d --------------------------------------------\nserver socket accepted a client connection:  %s:%d\n", ClientCount, client_ip, client_port);
    if(setsockopt(connected_socket, SOL_SOCKET, SO_RCVTIMEO, &ReadWriteTimeout, TimevalLen)<0) { printf("set receive timeout failed\n"); goto end_close_client; }
    if(setsockopt(connected_socket, SOL_SOCKET, SO_SNDTIMEO, &ReadWriteTimeout, TimevalLen)<0) { printf("set send timeout failed\n"); goto end_close_client; }
		//getsockopt(connected_socket, SOL_SOCKET,SO_RCVTIMEO, &ReadTimeout, &TimevalLen); printf("read timeout is %ld.%ld\n",ReadTimeout.tv_sec, ReadTimeout.tv_usec );
		//getsockopt(connected_socket, SOL_SOCKET,SO_SNDTIMEO, &WriteTimeout, &TimevalLen); printf("write timeout is %ld.%ld\n",WriteTimeout.tv_sec, WriteTimeout.tv_usec );
    read_size = read(connected_socket, buffer, BufSize1); if(read_size<0) { printf("read from client failed\n"); goto end_close_client;}
    printf("received %ld bytes data: \n%s\n", read_size, buffer);
    write_size = send(connected_socket, hello, strlen(hello), 0); if(write_size<0) { printf("write to client failed\n"); goto end_close_client;}
    printf("Hello message sent\n");
	  end_close_client: close(connected_socket);
    printf("accepting for next client\n");
  }
  perror("accept client connection failed");

	end_close: close(listen_socket);
	end_r: return 0;
}
