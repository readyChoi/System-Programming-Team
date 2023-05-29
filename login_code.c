#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFERSIZE 4096
/*
 login 기능 구현 - /etc/passwd 양식 참고/etc/passwd 양식 참고함
 https://reakwon.tistory.com/137  
 

<ID>:<터미널번호> 형태로 저장, ID 25자 제한
프로그램 켜서 로그인, 자기 아이디 입력하면 -> 접속한 터미널 번호, 아이디 저장

txt 파일: vscode, vi로 직접 만드는 건 ㄱㅊ, touch나 creat으로 생성 시 permission denied 오류 발생
*/

int get_id();
int get_term_num();

int main(){ 
    /*
    const char *filename = "login.txt";
    int fd = creat(filename, 777); // permission denied problem
    
    if(fd != -1){
        printf("login.txt created\n");
        get_id();
        get_term_num();
    }
    else printf("error in creating login.txt. manually create login.txt\n"); */

    get_id();
    get_term_num();
    return 0;
}

int get_id(){
    int fd, lseek_check, read_check, write_check; 
    char ID[50];
    char buf[BUFSIZ]; 
    size_t len;

    printf("Enter an ID. Your ID can be any combination of numbers and characters up to 25 characters long.\n");
    printf("ID: ");
    fgets(ID, sizeof(ID), stdin);
    len = strlen(ID);
    if (len > 25){
        printf("Your ID can be up to 25 characters long!\n");
        exit(1);
    }
    if(len > 0 && ID[len-1] == '\n'){ // change \n to :
        ID[len-1] = ':';
    }

    if((fd = open("login.txt", O_RDWR)) == -1){ // open login.txt
        perror("cannot open login file");
        exit(1);
    }

    if((lseek_check = lseek(fd, 0, SEEK_END)) == -1){ // lseek to EOF
        perror("cannot lseek");
        exit(1);
    }

    if(( read_check = read(fd, buf, BUFSIZ))== -1){ // read
        perror("cannot read");
        exit(1);
    }

    if((write_check = write(fd, ID, len)) == -1){ // write
        perror("cannot write");
        exit(1);
    } 

    close(fd);
    printf("got id\n");
    
    return 0;
}

int get_term_num(){
    
    int fd, lseek_check, read_check, write_check; 
    char buf[BUFSIZ];
    char *pts_name = ttyname(STDOUT_FILENO);
    char len = strlen(pts_name);

    if((fd = open("login.txt", O_RDWR)) == -1){ // open login.txt
        perror("cannot open login file");
        exit(1);
    }

    if((lseek_check = lseek(fd, 0, SEEK_END)) == -1){ // lseek
        perror("cannot lseek");
        exit(1);
    }    

    if(( read_check = read(fd, buf, BUFSIZ))== -1){ // read
        perror("cannot read");
        exit(1);
    }

    if((write_check = write(fd, pts_name, len)) == -1){ // write
        perror("cannot write");
        exit(1);
    }
    write(fd, "\n", 1);
    close(fd);
    printf("got terminal number\n"); // for \n insert
    return 0;
}