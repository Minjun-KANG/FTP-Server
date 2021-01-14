#include "Myserver_func.h"

int se_down(int cfd, char *inmsg)
{
	int er_code = ER_GOOD;
	char outmsg[MAXLINE];
	//readLine(cfd, inmsg); //Ack
	/*if(!(strcmp(inmsg, SC_DOWN))) // inmsg
	{
		er_code = ER_GOOD;
	}*/
	readLine(cfd, inmsg); //recv file name
	printf("%s server file name 받음 \n",inmsg);
	FILE *fp = fopen(inmsg, "r+b"); //읽기전용
	if(fp == NULL) //Server 입장 파일 존재하지 않다면 에러
        {
                er_code |= ER_EXIST; //파일이 존재하지 않습니다.
        }
        else //server 입장에서 file 이 있다면
        {
		printf("파일 존재확인\n");
                int temp = cfd;
       //         while(temp != EOF) //읽고 보냄
         //       {
			
                        fgets(outmsg, MAXLINE, fp);
			//read(fp,outmsg,MAXLINE); //읽고
                        /*if(!(strcmp(outmsg, SC_DOWN)))
                        {
                                break;
                        }*/
			printf("%s",outmsg);
			write(temp, outmsg, MAXLINE); //보냄
                        //fprintf(fp,"%s", outmsg);

           //     }
        }
        fclose(fp);
	return er_code;

}

int se_up(int cfd, char *inmsg)
{
	write(cfd, (char *)&SC_UP, strlen(SC_UP) + 1); //파일 전송시작을 알림
        FILE *fp;
        int er_code = 0;
        char input[MAXLINE];
        //readLine(sfd, inmsg);

        readLine(cfd, inmsg); //socket에서 파일 이름을 읽고, size를 len에 저장
        //printf("파일 이름 : ");
        //scanf("%s",input);
        printf("%s 까지는 입력 잘 받음 \n",inmsg);
        //write(cfd, inmsg, strlen(inmsg)+1); //file 이름 전송

        fp = fopen(inmsg, "w+b");
        //수신측 입장에서는 파일을 새로 생성해야함.

        if(fp == NULL) //파일 생성실패
        {
                er_code |= ER_EXIST;
        }
        else //server 입장에서 file 이 없다면
        {
		printf("file open suc\n");
                int temp = cfd;
//                while(temp != EOF) //받아서 씀
  //              {
                //      printf("while 까진 들어옴");
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

int Mylogin(const char* ID, const char *PW)
{
	//printf("Mylogin func\n");
	//printf("%s %d\t%s %d\n",ID,sizeof(ID),PW,sizeof(PW));
	FILE *fp;
	//int er_code = 0;
	char buf[MAXLINE];
	
	fp = fopen("./login.txt", "r");
	if(fp == NULL)
	{
		fprintf(stderr,"login file open error");
	}else
	{
		//printf("file open 성공\n");
	
		while(!feof(fp))
		{
			fscanf(fp,"%s",buf);
			//fgets(buf, sizeof(buf), fp);
			//printf("buf : %s\n",buf);	
			//printf("strcmp 여부 : %d\n",strcmp(ID,buf));
			if(!(strcmp(ID,buf)))
			{
				//printf("ID 비교성공\n");
				fscanf(fp,"%s",buf);
				//fgets(buf, sizeof(buf), fp);
				
				if(!(strcmp(PW,buf)))
				{
					//printf("PW 비교성공\n");
					printf("login success\n");
					//er_code = 0;
					int er_code = mk_dir(ID);
					if(er_code)
						return er_code;	
					else return 0;
				}
				//fprintf(stderr, "login success");
				//mk_dir(ID);
			}
			else
			{
				return ER_NOID;
			}
		}
	}
	return ER_NOID;
}

int mk_dir(const char *ID)
{
	printf("mk_dir 함수 진입\n");
	int er_code = 0;

	if (mkdir(ID, 0777))
	{	
		er_code |= ER_EXIST;
	}
	//working directory 변경
	char path[MAXLINE];
	getcwd(path, MAXLINE);
	strcat(path, "/");
	strcat(path,ID);
	chdir(path);
	system("pwd");

	return er_code;

}

int sock_case(int cfd, char *inmsg)
{
	printf("sock_case 진입 성공\n");
	char ID[MAXLINE];
	char PW[MAXLINE];
	int er_code = 0;
	char outmsg[MAXLINE];
	while(1){
	while(!readLine(cfd, inmsg));
	printf("inmsg = %s\n",inmsg);	
	er_code = 0;
	

	
	if(!(strcmp( inmsg, SC_LOGIN)))
	{
		printf("SC_LOGIN 진입성공\n");
		readLine(cfd, inmsg);
		strcpy(ID, inmsg);
		printf("ID 받아옴 : %s\n",ID);
		readLine(cfd, PW);
		printf("PW 받아옴 : %s\n",PW);
		er_code |= Mylogin(ID,PW);
		if((er_code & ER_NOID) == ER_NOID)
		{
			fprintf(stdout,"login 정보 없음");
			write(cfd, (char*)&SC_FALSE, strlen(SC_FALSE)+1);
			
		}else
		{
			write(cfd, (char *)&SC_LOGIN, strlen(SC_LOGIN)+1);
		}
	}
	else if(!(strcmp(inmsg, SC_DOWN)))
	{
		//server 입장에서 download
		do{
			er_code |= se_down(cfd, inmsg);
			if((er_code & ER_GOOD) == ER_GOOD)
			{
				break;
			}else
			{
				er_code = er_checking(er_code, inmsg);
			}
		}while(!er_code);
		
	}
	else if(!(strcmp( inmsg, SC_UP)))
	{
		//server 입장에서 upload
		do{
			er_code |= se_up(cfd, inmsg);
			if((er_code & ER_GOOD) != ER_GOOD)
			{
				er_code = ER_GOOD;
			}else
			{
				er_code = er_checking(er_code, inmsg);
			}
		}while(!er_code);
	}
	else if(!(strcmp( inmsg, SC_FIN)))
	{
		//fprintf(stderr,"%S : client 연결 종료",ID);
		system("clear");
		printf("client 연결종료 - ID : %s\n",ID);
		chdir("..");
		printf("폴더 재 변경\n");
		system("pwd");
		close(cfd);
	}
	else if(!(strcmp(inmsg, SC_LS)))
	{
		printf("SC_LS 진입\n");
		
		system("ls -l > ../ls_test");
		FILE *fp = fopen("../ls_test", "r");
		if(fp == NULL)
		{
			er_code |= ER_EXIST;
		}

		
		while(!feof(fp))
		{
			//fscanf(fp, "%s",outmsg);
			fgets(outmsg, MAXLINE, fp);
			printf("%s",outmsg);
			write(cfd, outmsg, MAXLINE);
		}
		fclose(fp);
		system("rm ../ls_test");
		write(cfd, (char *)&SC_LS, strlen(SC_LS) + 1);

	}	
	printf("sock_case 진행중 \n");	
	}//while((er_code & ER_END) == ER_END);
	//printf("sock_종료\n");
	return er_code;


}
