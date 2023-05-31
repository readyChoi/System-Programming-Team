#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int api_login_fun(char*);
int api_logout_fun(char*);

int main(void){
    char *id = "testtest123";
    api_login_fun(id);

    char *id1 = "test";
    api_logout_fun(id1);
}


int api_login_fun(char* ID){
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

    if((write_check = write(fd, "\n", 1)) == -1){ // write
        perror("cannot write");
        exit(1);
    } 

    close(fd);
    printf("got id\n");
    
    return 0;
}


int api_logout_fun(char* search_string) {
    FILE* file = fopen("api_login.txt", "r");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }

    FILE* temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        perror("fopen");
        exit(1);
    }

    char line[1024];
    int line_number = 1;
    int deleted = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, search_string) == NULL) {
            fputs(line, temp_file);
        } else {
            deleted = 1;
            printf("Line %d deleted.\n", line_number);
        }
        line_number++;
    }

    fclose(file);
    fclose(temp_file);

    if (deleted) {
        if (remove("api_login.txt") != 0) {
            perror("remove");
            exit(1);
        }

        if (rename("temp.txt", "api_login.txt") != 0) {
            perror("rename");
            exit(1);
        }

        printf("Deletion complete.\n");
    } else {
        remove("temp.txt");
        printf("No lines containing the search string found.\n");
    }

    return 0;
}