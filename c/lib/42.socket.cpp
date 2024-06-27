
#include "../includes/buda.h"


namespace BUDA
{	


struct in_addr * get_sock_addr(char *hostname)
{
    log("resolving %s to IP address..." , hostname);
    struct hostent *he; struct in_addr **addr_list, *ip_addr=NULL; int i;
        
    if ( (he = gethostbyname( hostname ) ) == NULL )  goto fail;

    addr_list = (struct in_addr **) he->h_addr_list;
    for(i = 0; (ip_addr=addr_list[i]) != NULL; i++) 
    {
        log("%d: %s\n", i+1 , inet_ntoa(*ip_addr) ); // inet_ntoa convert an IP address in long int format to dotted format
        goto succeed; // use the first IP address 
    }	
    goto fail;

    succeed: return ip_addr;
    fail: log("gethostbyname for %s failed", hostname); return NULL;
}



int set_socket_options(int socket)
{
    // log("set_socket_options");
    static socklen_t len = sizeof(struct timeval); struct timeval SocketTimeout, RecvTimeout, SendTimeout; 
	SocketTimeout.tv_sec = 5; SocketTimeout.tv_usec = 0; 
	RecvTimeout.tv_sec = RecvTimeout.tv_usec = 0; SendTimeout.tv_sec = SendTimeout.tv_usec = 0;

	//getsockopt(socket, SOL_SOCKET,SO_RCVTIMEO, &RecvTimeout, &len); log("recv timeout is %ld.%ld before set",RecvTimeout.tv_sec, RecvTimeout.tv_usec );
	//getsockopt(socket, SOL_SOCKET,SO_SNDTIMEO, &SendTimeout, &len); log("send timeout is %ld.%ld before set",SendTimeout.tv_sec, SendTimeout.tv_usec );
    if(setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &SocketTimeout, len)==-1) { log("set receive timeout failed"); goto fail; }
    if(setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, &SocketTimeout, len)==-1) { log("set send timeout failed"); goto fail; }
	//getsockopt(socket, SOL_SOCKET,SO_RCVTIMEO, &RecvTimeout, &len); log("recv timeout is %ld.%ld after set",RecvTimeout.tv_sec, RecvTimeout.tv_usec );
	//getsockopt(socket, SOL_SOCKET,SO_SNDTIMEO, &SendTimeout, &len); log("send timeout is %ld.%ld after set",SendTimeout.tv_sec, SendTimeout.tv_usec );

	succeed: return 0;
	fail: return -1;
}




}