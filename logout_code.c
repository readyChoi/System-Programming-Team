#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFERSIZE 4096

int logout(char*);

int main(){
    char* username = "testtest123";
    logout(username);
    return 0;
}

int logout(char* ID){
    int fd, lseek_check, read_check, write_check;
    int found = 0; 
    char buf[BUFSIZ]; 
    size_t len;
    FILE *file = fopen("login.txt", "r+");
    
    if (file == NULL) {
        perror("Failed to open the file");
        exit(1);
    }
    
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Failed to create temporary file");
        fclose(file);
        return -1;
    }

    char line[1000];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, ID, strlen(ID)) != 0) {
            fputs(line, tempFile);
        } else {             // ID found, set the flag
            found = 1;
        }
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    if (remove("login.txt") != 0) {     // Remove the original login.txt file
        perror("Failed to remove the original file");
        return -1;
    }

    if (rename("temp.txt", "login.txt") != 0) {     // Rename the temporary file to login.txt
        perror("Failed to rename the temporary file");
        return -1;
    }

    if (found) {
        printf("Logged out successfully\n");
    } else {
        printf("ID not found\n");
    }

    return 0;
}


