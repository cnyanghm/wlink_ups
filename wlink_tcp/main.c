
/*
	UPS wlink tcp 协议
	
	UPS 在 tcp:8200 提供 wlink 服务， 包括查询，设置和命令。
	
	请求和应答均是 json{ } 结构数据
	
	
	请求连接	{ "connect":1, "pass":"123456" }
	正常应答	{ "answer": "200 OK"}
	异常应答	{ "answer": "505 busy now" }
			{ "answer": "404 auth required", "challenge": "...." }
			{ "answer": "405 auth error", "challenge": "...." }
		
	. 查询
				
	基本信息	{ "get":["info"] }
	应答	{ "return": [<info>]}
	<info> 格式 {"type",...}
	
	额定信息	{ "get":["RTD"] }
	应答	{ "return": [<RTD>]}
	正常 <RTD> 格式 "(品牌 固件版本 型号 
					输入电压 
					输出电压  输出电流  输出频率  
					电池电压 
					输入相位/输出相位  
					功率 "
	异常 <RTD> 格式 "noReady", "rs232Error"
	

	动态信息	{ "get":["QAD"] }
	应答	{ "return": [<QAD>]}
	正常 <QAD> 格式 "(输入电压[3] 输入频率 输入电流[3]
					故障电压
					输出电压[3] 输出频率 输出电流[3]  负载百分比[3]
					电池电压 电池剩余时间（分钟）  电池容量百分比 
					机内温度 
					状态 "					
	异常 <QAD> 格式 "noReady", "rs232Error"
		
	. 设置
		
	名称	{ "set":{'name":"new name"} }
	应答	{ "return": [true]}

	密码	{ "set":{'pass":"new pass"} }
	应答	{ "return": [true]}
			
	. 命令
		
	开机	{ "cmd":["SON"] }
	应答	{ "return": [true]}

	支持的命令
	SON  开机
	SOFF 关机
	T 测试
	TL 放电
	CT 结束放电
	BZON 打开蜂鸣
	BZOFF 关闭蜂鸣

		
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

#include <sys/types.h>

#ifdef OS_LINUX

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#endif



static int request( int fd, char buf[], char answer[] ) ;
static int check_json_format( char answer[] ) ;

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
	addr.sin_addr.s_addr = inet_addr( "192.168.0.239" );
	addr.sin_port = htons( 8200 );
		
	int fd = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	
	int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr) );
	if( ret == -1 ) 
	{
		printf( "connect fail\n" );
		return 0 ;
	}
	
	char buf[2048];
	char answer[2048];
	
	sprintf( buf, "{ \"connect\":1, \"pass\":\"123456\" }" ) ;
	if( request( fd, buf, answer ) == 0 ) return 0 ;
	
	sprintf( buf, "{ \"get\":[\"info\"] }" ) ;
	if( request( fd, buf, answer ) == 0 ) return 0 ;
	
	sprintf( buf, "{ \"get\":[\"RTD\"] }" ) ;
	if( request( fd, buf, answer ) == 0 ) return 0 ;

	sprintf( buf, "{ \"get\":[\"QAD\"] }" ) ;
	if( request( fd, buf, answer ) == 0 ) return 0 ;

	sprintf( buf, "{ \"set\":{\"name\":\"new name\"} }" ) ;
	if( request( fd, buf, answer ) == 0 ) return 0 ;

	sprintf( buf, "{ \"cmd\":[\"SOFF\"] }" ) ;
	if( request( fd, buf, answer ) == 0 ) return 0 ;

	close(fd);
	
	printf( "\n\n" );
	
	return 0 ;
	
}

int request( int fd, char buf[], char answer[] )
{
	printf( "\nrequest : %s\n", buf );
	
	int bn = strlen(buf);
	
	int n = send( fd, buf, bn, 0);
	
	if( n < bn ) 
	{
		printf( "send fail\n" );
		return 0 ;
	}
	
	int an = 0 ;
	
	while( 1 )
	{
		int n = recv( fd, answer+an, 2048, 0);
		
		if( n > 0 )
		{
			an += n ;
			
			answer[an]= 0 ;
			
			int cn  = check_json_format( answer ) ;
			if( cn ) 
			{
				answer[cn] = 0 ;
				
				printf( "answer : %s\n", answer );	
				return cn ;
			}
			
			sleep(1);
		}
		else 
		{
			printf( "recv fail\n" );
			return 0 ;
		}
	}
	
}

int check_json_format( char answer[] )
{
	char *p = answer ;
	
	while( *p && *p != '{' ) p++ ;
	if( *p == 0 ) return 0 ;
	
	int deep = 0 ;
	for( ; *p; p++ )
	{
		if( *p == '{' )
		{
			deep ++ ;
		}
		else if( *p == '}' )
		{
			if( -- deep == 0 )
			return p+1-answer;
		}
	}
	
	return 0 ;
}
	





