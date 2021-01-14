/*
	2017707007 Minjun-KANG
	Client code


*/




#include "CMN_header.h"
#include "Myclient_func.h"
/* 파일 클라이언트 프로그램 */
int main (int argc, char* argv[]) 
{
    //int er_msg=0;
    int sfd, port, result;
    char *host;//, inmsg[MAXLINE], outmsg[MAXLINE];
    struct sockaddr_in serverAddr;
    struct hostent *hp;

    if (argc != 3) {
        
	
	fprintf(stderr, "사용법 :  <host> <port>\n");
	fprintf(stderr, "<host> <port>\n");
	scanf("%s %s",argv[1],argv[2]);
        /*exit(0);*/
    }

    host = argv[1];
    port = atoi(argv[2]);

    sfd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);

    /* 서버의 IP 주소와 포트 번호를 채운다. */
    if ((hp = gethostbyname(host)) == NULL)
        perror("gethostbyname error"); // 호스트 찾기 오류  
    
    bzero((char *) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    bcopy((char *)hp->h_addr_list[0], (char *)&serverAddr.sin_addr.s_addr, hp->h_length);
    serverAddr.sin_port = htons(port);

    do 
    { /* 연결 요청 */
        result = connect(sfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
        if (result == -1) 
	    sleep(1);
    } while (result == -1);
    printf("host : %s, port : %d\n",host,port);
    printf("연결 성공\n");
    

    input_case(&sfd);
    
}



