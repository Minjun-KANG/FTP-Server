/*
	2017707007 Minjun-KANG
	only used by client.c
	Myclient_func.h


*/


#include "CMN_header.h"

int cl_down(int, char*); //server se_up과 호환;
int cl_up(int, char *); //server se_down과 호환;

void input_case(int *);
/*
	pre - 연결완료 후, 실행
	post - 파일 업, 다운, 목록확인, 프로그램 종료를 여기서 실행함

*/

int username(char *, char *);
/* //error 시 -1 정상, 0
	pre - 연결 성공후 실행되는 함수, char *ID와 char *PW를 받는다
	post - user의 ID, PW를 받아온다


*/
