#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFERSIZE 4096
/*
 login 기능 구현
 https://reakwon.tistory.com/137  
 /etc/passwd 양식 참고
    root:x:0:0:root:/root:/bin/bash
    <사용자 계정명>:<패스워드>:<UID>:<GID>:<comment>:<로그인 쉘>

<ID>:<터미널번호> 형태로 저장
프로그램 켜서 로그인, 자기 아이디 입력하면 -> 접속한 터미널 번호, 아이디 저장

*/ 
// ID는 최대 25자까지 받기로 -> 더 짧아져도 ㄱㅊ
/*
int main(){
    int fd, null_check; 
    char* ID = NULL;
    char buf[BUFFERSIZE]; 
    size_t size;

    printf("ID: ");
    getline(&ID, &size, stdin);
    
    if((fd = open("login.txt", O_RDWR)) == -1){
        perror("cannot open login file");
        free(ID);
        exit(1);
    }

    if((null_check = read(fd, buf, BUFFERSIZE))== -1){
        write(fd, ID, size);    
    }
    else{

    }
    

    close(fd);
    printf("got id\n");
    free(ID);

    return 0;
}

*/
