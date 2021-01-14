#include"Myclient_func.h"

int cl_down(int sfd, char *inmsg)
{
        write(sfd, (char *)&SC_DOWN, strlen(SC_DOWN) + 1); //파일 전송시작을 알림
        FILE *fp;
        int er_code = 0;
        char input[MAXLINE];
        //readLine(sfd, inmsg);
               
        //readLine(fd, inmsg); //socket에서 파일 이름을 읽고, size를 len에 저장
        //printf("파일 이름 : ");
	//scanf("%s",input);
//	printf("%s 까지는 입력 잘 받음 \n",inmsg);
	write(sfd, inmsg, strlen(inmsg)+1); //file 이름 전송
	//printf("write 성공\n");
        fp = fopen(inmsg, "w+b");
        //수신측 입장에서는 파일을 새로 생성해야함.

        if(fp == NULL) //파일 생성실패
        {
                er_code |= ER_EXIST;
        }
        else //server 입장에서 file 이 없다면
        {
                int temp = sfd;  
//                while(temp != EOF) //받아서 씀
  //              {
		//	printf("while 까진 들어옴");
                        read(temp,inmsg,MAXLINE);
                        /*if(!(strcmp(inmsg, SC_DOWN)))
                        {
                                break;
                        }*/
                        fprintf(fp,"%s", inmsg); //파일에 써버림
                 
    //            }
        }
        fclose(fp);
        return er_code;
}

int cl_up(int sfd, char *inmsg)
{
	char input[MAXLINE];
	strcpy(input, inmsg);
//	printf("clup 진입 %s \n",input);
	write(sfd, (char *)&SC_UP, strlen(SC_UP) + 1); //sync
	int er_code = ER_GOOD;
        char outmsg[MAXLINE];
        readLine(sfd, inmsg); //Ack
//	printf("ack %s\n",inmsg);
        if(!(strcmp(inmsg, SC_UP))) // inmsg
        {
                er_code = ER_GOOD;
        }
//       readLine(sfd, inmsg); //recv file name
//        printf("%s server file name 받음 \n",inmsg);
	write(sfd, input, strlen(input)+1); //file name 전송 
//       printf("file name : %s\n",input);
	FILE *fp = fopen(input, "r+b"); //읽기전용
        if(fp == NULL) //Server 입장 파일 존재하지 않다면 에러
        {
                er_code |= ER_EXIST; //파일이 존재하지 않습니다.
        }
        else //server 입장에서 file 이 있다면
        {
//                printf("파일 존재확인\n");
                 int temp = sfd;
       //         while(temp != EOF) //읽고 보냄
         //       {

                        fgets(outmsg, MAXLINE, fp);
                        //read(fp,outmsg,MAXLINE); //읽고
                        /*if(!(strcmp(outmsg, SC_DOWN)))
                        {
                                break;
                        }*/
//                      printf("%s",outmsg);
//                        printf("1\n");
			 write(temp, outmsg, MAXLINE); //보냄
                        //fprintf(fp,"%s", outmsg);
//			printf("2\n");
           //     }
        }
        fclose(fp);
//	printf("3\n");
        return er_code;
 
}

void input_case(int *sfd)
{
	char ID[MAXLINE] = {0,};
	char PW[MAXLINE] = {0,};
	int lo_cnt = 0; 
	int flag = 1;
	int er_code = 0;
	char er_msg[MAXLINE];
	int in_case = 0;
	char inmsg[MAXLINE];
	char outmsg[MAXLINE];
	do{
		write(*sfd, (char *)&SC_LOGIN, sizeof(SC_LOGIN));
		if(lo_cnt > 4)
		{
			fprintf(stderr, "로그인 횟수 초과.\n");
			printf("연결을 종료합니다.\n\n");
			close(*sfd);
			exit(0);
		}
		if(username(ID,PW))
		{
			printf("ID/PW를 잘못 입력하셨습니다. \n");
			lo_cnt++;
			continue;
		}
		if(write(*sfd, ID, strlen(ID)+1) != -1)
		{
			if(write(*sfd, PW, strlen(PW)+1) != -1)
			{
				//write(*sfd, (char *)&ID, strlen(ID) + 1);
				//sleep(1);
				//write(*sfd, (char *)&PW, strlen(ID) + 1);
				sleep(1);
				flag = readLine(*sfd, inmsg);
//				printf("inmsg = %s\n",inmsg);
				flag = strcmp(inmsg, SC_LOGIN);
				//break;
			}
		}
		if(flag)
		{
			printf("잘못 입력하셨습니다. \n");
		}
		
	}while(flag);
	printf("로그인 성공\n");
	

	printf("\n----------------------\n");
	printf("1. 파일 다운로드\n");
	printf("2. 파일 업로드\n");
	printf("3. 파일 목록 확인\n");
	printf("4. 연결 종료\n");
	printf("----------------------\n\n");
	printf("메뉴 입력 : ");
	scanf("%d",&in_case);
	
	while(1)
	{
		if(er_code)
		{
			er_checking(er_code, er_msg);
		}
		if(!(in_case>0 && in_case <5)) //menu 입력이 잘못
		{
			if(!in_case)
			{
				printf("\n----------------------\n");
				printf("1. 파일 다운로드\n");
				printf("2. 파일 업로드\n");
				printf("3. 파일 목록 확인\n");
				printf("4. 연결 종료\n");
				printf("----------------------\n\n");
				printf("메뉴 입력 : ");
				scanf("%d",&in_case);
				
				continue;
			}
			in_case = er_checking(ER_INPUT, er_msg);
		}
			
	
		if(in_case == 1)
		{ //cl_down
			printf("다운로드할 파일 입력 : ");
		        scanf("%s", inmsg);
			do{
		        	er_code |= cl_down(*sfd, inmsg);
				if((er_code & ER_GOOD) == ER_GOOD)
				{
					er_code = ER_GOOD;
					break;
				}else
				{
					er_code = er_checking(er_code, inmsg);
				}
			}while(!er_code);
			printf("download 성공\n");	
		
		}
		else if(in_case == 2)
		{//서버 입장에서 down이니, client는 up
			printf("업로드할 파일 입력 : ");
			scanf("%s", inmsg);
			do{
		        	er_code |= cl_up(*sfd, inmsg);
				if((er_code & ER_GOOD) == ER_GOOD)
				{
					er_code = ER_GOOD;
					break;
				}
				else
				{
					er_code = er_checking(er_code, inmsg);
				}
			}while(!er_code);
			printf("Upload 성공\n");
		}
		else if(in_case == 3)
		{
			printf("파일 목록 확인\n");
			write(*sfd, (char *)&SC_LS, strlen(SC_LS) +1);
			//printf("ls 읽기 준비\n");
			
			int temp = *sfd;	
			while(temp != EOF)
			{
				read(temp,outmsg,MAXLINE);
				if(!(strcmp(outmsg, SC_LS)))
				{
					break;
				}
        			printf("%s", outmsg);
				//printf("while 도는중\n");
				//printf("%d",temp);
			}
			//getchar();
		}
		else if(in_case == 4)
		{
			write(*sfd, (char *)&SC_FIN, strlen(SC_FIN)+1);
			printf("연결을 종료합니다.\n\n");
			close(*sfd);
			exit(0);
		}
		in_case = 0;	
	
	}
	
}

int username(char *ID, char *PW)
{
	int n = 0;
	do{
		printf("ID : ");
		scanf("%s",ID);
		
		while( (ID[n] >= 'A' && ID[n] <= 'Z' ) || 
			(ID[n] >= 'a' && ID[n] <= 'z') || 
			(ID[n] >= '0'&& ID[n] <= '9') )
		{
			
			n++;
		}

	//printf("여기까지는 될까");

	}while(!n);
	
	if(strlen(ID) != n)
	{
		return -1;
	}
	
	n = 0;
	do{
		printf("PW : ");
		scanf("%s",PW);
		
		while( (PW[n] >= 'A' && PW[n] <= 'Z' ) || 
			(PW[n] >= 'a' && PW[n] <= 'z') || 
			(PW[n] >= '0'&& PW[n] <= '9') )
		{
			n++;
		}		
	
	}while(!n);

	if(strlen(PW) != n)
	{
		return -1;
	}
	
	return 0;
}	
//egmentation fault (core dumped)

