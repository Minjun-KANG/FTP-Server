#include "CMN_header.h"

int readLine(int fd, char* str)
{
    int n;
    do {
        n = read(fd, str, 1);
    } while (n > 0 && *str++ != '\0');
    return(n > 0);
}

int er_checking(const int error, char p[])
{
	if(error)
	{
		if((error & ER_EXIST) == ER_EXIST) //server send to client error
		{
			strcpy(p,"파일이 존재하지 않습니다.");	
			fprintf(stderr, p, strlen(p));
			return 0;
		}
		else if((error & ER_INPUT) == ER_INPUT) //client send to client error
		{
			int in_case = 0;
			do
			{
				if(!(in_case>0 && in_case <5))
				{
					printf("잘못 입력하셨습니다.\n");
					printf("메뉴 입력 : ");
					scanf("%d",&in_case);
				}
			}
			while(!(in_case > 0 && in_case < 5));
			
			return in_case;
		}
		else if((error & ER_ACCEPT) == ER_ACCEPT)// Accept error
		{
			strcpy(p,"Accept() Error");
			fprintf(stderr, p, strlen(p));
			return 0;
		}
		else if((error & ER_SAME) == ER_SAME)// Accept error
		{
			strcpy(p,"같은 이름의 파일이 존재합니다");
			fprintf(stderr, p, strlen(p));
			return 0;
		}
	}
	return 0;
	
}




