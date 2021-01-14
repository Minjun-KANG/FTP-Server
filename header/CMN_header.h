/*
2017707007 Minjun-KANG

CMN_header.h

Hedaer file for Cloud Server production purpose.
The server side and the clientt side share 
and use the corresponding header file.

The prototype of the function used by both sides.
This is included.

*/

/*

	Header file declaration.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/*

	Constant declaration.
*/
#define DEFAULT_PROTOCOL 0
#define MAXLINE 100

	//Error code
#define ER_EXIST 0b00000001 //파일이 존재하지 않을 때
#define ER_INPUT 0b00000010 //client 측 input을 잘못한 경우
#define ER_ACCEPT 0b00000100 //Accept() Error checking
#define ER_SAME 0b00001000 //upload 상황에 server에 같은 이름 파일이 존재
#define ER_NOID 0b00010000 //ID 없음 에러
#define ER_END 0b00100000 //종료 flag
#define ER_GOOD 0b01000000 //정상 flag
/*
	Socket state flag
*/
#define SC_LOGIN "SC_LOGIN"
#define SC_FALSE "SC_FALSE"
#define SC_DOWN "SC_DOWN"
#define SC_UP "SC_UP"
#define SC_FIN "SC_FIN"
#define SC_LS "SC_LS"





//	Prototype
//	User Defined Function

int readLine(int fd, char* str);
/* Error : 0 , return 1
	pre : fd와 str을 하나 받음
	post : str에 이를 저장 후, n리턴

*/

int er_checking(const int, char []);
/*
	pre - pre defined error을 읽고, 해당하는 문장을 char * 에 카피
	post - 해당하는 error 문장을 출력 및 카피, 각각의 case 마다 return 하는 경우가 발생
*/
