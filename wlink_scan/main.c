
/*
	UPS wlink udp 扫描协议
	
	UPS 在 tcp:8100 提供 wlink udp 服务， 包括扫描协议。
	
	请求和应答均是 json{ } 结构数据
	
	扫描请求	{ "search":1, "seq":10 }
	设备应答	{ "report":1, "seq":10, ..... }
		
*/


//#define OS_WINDOWS
#define OS_LINUX

#ifdef OS_WINDOWS

#include <windows.h>
#define sleep Sleep
#define close closesocket
#pragma  comment(lib, "ws2_32.lib" )

#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <sys/types.h>

#ifdef OS_LINUX

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#endif

//----------------------------------------------------------------------
int main(  )
{
	printf( "\n\n" );

	#ifdef OS_WINDOWS
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD( 2,0 );
	WSAStartup( wVersionRequested, &wsaData );
	#endif
	
	struct sockaddr_in addr ;
	addr.sin_family = AF_INET ;
	addr.sin_addr.s_addr = inet_addr( "192.168.0.255" );
	addr.sin_port = htons( 8100 );
		
	int fd = socket ( AF_INET, SOCK_DGRAM, 0 );
		
	int option = 1 ;
	setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,(char*)&option, sizeof( option ) );
	setsockopt( fd, SOL_SOCKET, SO_BROADCAST,(char*)&option, sizeof( option ) );

	char buf[1024];
	int  bn = sprintf( buf, "{ \"search\":1, \"seq\":10 }" ) ;
	sendto( fd, buf, bn, 0, (struct sockaddr*) &addr, sizeof( addr ) );
	
	while( 1 )
	{
		fd_set r ;
		FD_ZERO(&r);
		FD_SET(fd, &r);

		struct timeval tv = { 2, 0 } ;

		int rn = select( fd+1, &r, NULL, NULL, &tv) ;

		if( rn == -1 )
		{
			if( errno == EINTR )
			continue;

			break ;
		}
		
		if( rn == 0 ) break ; // timeout
		
		char answer[1024];
		int an = recv(fd, answer, 1024, 0 );
		if( an < 0 ) continue ;
		
		answer[an] = 0 ;
		printf( "\n%s\n", answer );
		
	}

	
	close(fd);
	
	printf( "\n\n" );
	
	return 0 ;
	
}






