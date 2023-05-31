#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
    char *id = "testtest123";
    get_id(id);
}


int get_id(char* ID){
    int fd, lseek_check, read_check, write_check; 
    char buf[BUFSIZ]; 
    size_t len;

    len = strlen(ID);
    if (len > 25){
        printf("Your ID can be up to 25 characters long!\n");
        exit(1);
    }

    if((fd = open("api_login.txt", O_RDWR)) == -1){ // open login.txt
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