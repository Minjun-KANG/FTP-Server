/*
	2017707007 Minjun-KANG
	Server code

	시작 인수에 port 번호를 받음
	ex) ./Server 8080


*/



#include "Myserver_func.h"

int main (int argc, char* argv[])
{
    int er_code = 0;
    int sfd, cfd, port, clientlen;
    //FILE *fp;
    char inmsg[MAXLINE], outmsg[MAXLINE]; //메세지 전용변수 생성
    struct sockaddr_in serveraddr, clientaddr; //sockaddr 생성
    //struct hostent *hp; //DNS 이용하기 위한 st
    char *haddrp;

    signal(SIGCHLD, SIG_IGN); //자식 process 에서 오는 signal은 무시(SIG_IGN)한다.

    if (argc != 2) {
        fprintf(stderr, "사용법: %s <port>\n", argv[0]);
        exit(0);
    }

    port = atoi(argv[1]);
    sfd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL); //ipv4 로, TCP 방법, 0

    bzero((char *) &serveraddr, sizeof(serveraddr)); //0으로 채움
    serveraddr.sin_family = AF_INET; //ipv4
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // host to network long
    serveraddr.sin_port = htons((unsigned short)port); //host to network short
    
    if(bind(sfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr))==-1) //서버의 내부주소와 포트 지정
    {
	fprintf(stderr, "binding error\n");
	close(sfd);
	exit(1);
    }
    if(listen(sfd, 5)==-1) // 5개의 host 까지 연결허용
    {
	fprintf(stderr,"listening error\n");
    }
    printf("연결 대기중....;\n");





    /*
	setting 끝, 연결대기 가능
    */

    while (1) 
    { // accpet 대기 while
    
	if(er_code) //while error
        {
		
	    er_checking(er_code, outmsg);
	    write(cfd, outmsg, strlen(outmsg)+1);
        }

        clientlen = sizeof(clientaddr);
        cfd = accept(sfd, (struct sockaddr *) &clientaddr, (socklen_t *)&clientlen); // 연결 요청 수락

        if(cfd == -1) //accept error handling
	{
		
		er_code |= ER_ACCEPT;
		continue;
	}





	/* 클라이언트의 도메인 이름과 IP 주소 결정 */
       // hp = gethostbyaddr((char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        haddrp = inet_ntoa(clientaddr.sin_addr); //net 주소 변환
        printf("서버: %s (%d)에 연결됨\n", haddrp, clientaddr.sin_port);
	printf("Client 입력 대기중...\n");
        


	printf("fork 시작\n");
	if (fork() == 0) 
	{
		
		er_code |= sock_case(cfd, inmsg);
		//printf("sock_case종료 \n");
		
        } 
	else close(cfd);
    }
    
}


/* using struct

//AF_INET을 목적으로 하기 때문에 이를 선택
struct sockaddr_in
{
	short sin_family //주소체계 AF_INET
	u_short sin_port //2Byte 포트번호, Network byte order
	struct in_addr sin_addr //4Byte IP addr ->이는 u_long s_addr 이라는 멤버변수 하나로 이뤄진 구조체
	char sin_zero[8] // byte aligned
}

struct hostent 
{ 
	char *h_name; //공식 도메인 이름 
	char **h_aliases; //공식 이외 도메인 이름들 
	int h_addrtype; //주소정보 체계(IPv4: AF_INET, IPv6: AF_INET6) 
	int h_length; //IP주소의 크기를 담는다. (IPv4는 4) 
	char **h_addr_list; //도메인 이름에 대한 IP주소가 정수 형태로 반환될 때 이 멤버 변수를 이용 }
}
*/
