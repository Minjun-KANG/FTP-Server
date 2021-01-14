/*
	2017707007 Minjun-KANG
	only used by server.c
	Myserver_func.h

*/

#include "CMN_header.h"

int mk_dir(const char *);
/* 정상 수행 0, error -1
	pre - username을 입력받음
	post -  file에 working directory를 username폴더로 설정


*/
int sock_case(int , char*);
/*
	pre - cfd와 inmsg를 인자로 넘겨받음
	post - 각 소켓에 대한 FLAG를 넘겨받음
*/

int se_down(int, char *);
int se_up(int, char *);
