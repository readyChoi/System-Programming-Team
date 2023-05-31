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
        char f_path[256];
        char contents[201];
        char group_user[51];
        int flag;
        int temp;
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
    FILE* temp;
    struct dirent *filelist;
    struct stat info;
    int fd;
    int i=0;
    int grouplen;

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
                exit(1);
            }

            if(strcpy(task_list[i].f_path,filepath) == NULL){ // save filepath
                perror("cannot save filepath");
                exit(1);
            }

            if(read(fd, task_list[i].time, 10) == -1){ // first line -> save time
                perror("save time error");
                exit(1);
            }
            
            lseek(fd, 1, SEEK_CUR);

            if(read(fd, &(task_list[i].flag), 1) == -1){ // second line -> save 
                perror("save flag error");
                exit(1);
            }

            if(read(fd, task_list[i].contents, 200) == -1){ // get contents first, will abort fd
                perror("save contents error");
                exit(1);
            }

            lseek(fd, 2, SEEK_CUR);

            temp = fdopen(fd, "r"); // to change int fd to FILE* type

            fgets(task_list[i].group_user, 30, temp); // get line
            
            if(i==29){
                printf("you can only read 30 tasks");
                exit(1);
            }
            i++;
    
        printf("%d: filename: %s \n", i, task_list[i-1].filename);
        printf("%d: flag %c \n", i, task_list[i-1].flag);
        printf("%d: group: %s \n", i, task_list[i-1].group_user);
        printf("%d: %s \n", i, task_list[i-1].contents);
        
        }
    }
     
    closedir(dir);
    return task_list;
}