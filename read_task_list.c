#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {
        char time[13]; // save year, month, day, hour with concat
        char filename[51];
        char contents[201];
        int flag;
        int temp; //for joon
    } task;

task task_list[30];
task* read_dir(char*);

int main(){
    char *path_ptr="./js/";
    task *Task_list; // Task_list receiving task_list from read_dir
    Task_list = read_dir(path_ptr);
    free(Task_list);
    return 0;
}


task* read_dir(char* dir_path){
    DIR *dir;
    task *task_list = malloc(sizeof(task)*30);    
    struct dirent *filelist;
    struct stat info;
    int fd;
    int i=0;

    if((dir = opendir(dir_path)) == NULL){ // opendir
        perror("cannot open directory");
        exit(1);
    }
    while((filelist = readdir(dir)) != NULL){ // readdir
        if(filelist->d_type == DT_REG){
            /* make filepath */
            char filepath[256];
            strcpy(filepath, dir_path);
            strcat(filepath, filelist->d_name);
            printf("%s\n", filepath);

            if((fd = open(filepath, O_RDONLY)) == -1){ // open file
                perror("cannot open file");
                exit(1);
            }

            if(strcpy(task_list[i].filename,filelist->d_name) == NULL){ // save filename 
                perror("save filename error");
            }

            if(read(fd, task_list[i].time, 13) == -1){ // first line -> save time
                perror("save time error");
                exit(1);
            } 
    
            
            lseek(fd, 1, SEEK_CUR);

            if(read(fd, &(task_list[i].flag), 1) == -1){ // second line -> save 
                perror("save flag error");
                exit(1);
            }

            if (task_list[i].flag == 48){ // 파일에 문자가 포함되어 있어서, flag 저장할 때 문자로 인식함 -> 아침에 수정
                task_list[i].flag = 0;
            } else task_list[i].flag = 1;

            lseek(fd, 1, SEEK_CUR);

            if(read(fd, task_list[i].contents, 200) == -1){ // third line -> save
                perror("save contents error");
                exit(1);
            }
/*
            lseek(fd, 2, SEEK_CUR);

            if(read(fd, &(task_list[i].temp), 1) == -1){
                perror("save temp error");
                exit(1);
            }

*/
            
            if(i==29){
                printf("you can only read 30 tasks");
                exit(1);
            }
            i++;
        /* 안에서 테스트
        printf("%d: filename: %s \n", i, task_list[i-1].filename);
        printf("%d: flag %d \n", i, task_list[i-1].flag);
        printf("%d: %s \n", i, task_list[i-1].contents);
        */
        
        }
    }
    /*
    반복문 밖에서 테스트
        printf("%d: filename: %s \n", i, task_list[i-1].filename);
        printf("%d: flag %d \n", i, task_list[i-1].flag);
        printf("%d: %s \n", i, task_list[i-1].contents);    

    */     
    closedir(dir);
    return task_list;
}