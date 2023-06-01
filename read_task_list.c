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
        char group_user[51];
        char category[20];
        char schedule_check[20];
        char flag[5];
        int temp;
    } task;

task task_list[30];
task* read_dir(char*);

int main(){
    char *path_ptr="./choi";
    task *Task_list; // Task_list receiving task_list from read_dir
    Task_list = read_dir(path_ptr);
    free(Task_list);
    return 0;
}

int check_file_exists(char* directory_path, char* checkfile) {
    char file_path[256];
    memset(file_path, 0, sizeof(file_path));
    strcat(file_path, directory_path);
    strcat(file_path, checkfile);

    printf("%s\n", file_path);
    FILE* file = fopen(file_path, "r");
    if (file) {
        fclose(file);
        return 1; // 파일이 존재하는 경우
    } else {
        return 0; // 파일이 존재하지 않는 경우
    }
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
            memset(filepath, 0, sizeof(filepath));

            char filetxtname[50];
            memset(filetxtname, 0, sizeof(filetxtname));

            strcat(filetxtname, filelist->d_name);
            strcat(filetxtname, ".txt");
            printf("%s\n", filetxtname);
            fflush(stdout);

            if(strcpy(task_list[i].filename,filelist->d_name) == NULL){ // save filename 
                perror("save filename error");
                exit(1);
            }

            if(check_file_exists("./Today/School/", filetxtname) == 1){
                strcpy(task_list[i].category, "School");
                strcpy(task_list[i].schedule_check, "Today");
            }
            else if(check_file_exists("./Today/Family/", filetxtname) == 1){
                strcpy(task_list[i].category, "Family");
                strcpy(task_list[i].schedule_check, "Today");
            }
            else if(check_file_exists("./Today/Friend/", filetxtname) == 1){
                strcpy(task_list[i].category, "Friend");
                strcpy(task_list[i].schedule_check, "Today");
            }
            else if(check_file_exists("./Finished/School/", filetxtname) == 1){
                strcpy(task_list[i].category, "School");
                strcpy(task_list[i].schedule_check, "Finished");
            }
            else if(check_file_exists("./Finished/Family/", filetxtname) == 1){
                strcpy(task_list[i].category, "Family");
                strcpy(task_list[i].schedule_check, "Finished");
            }
            else if(check_file_exists("./Finished/Friend/", filetxtname) == 1){
                strcpy(task_list[i].category, "Friend");
                strcpy(task_list[i].schedule_check, "Finished");
            }
            else if(check_file_exists("./Scheduled/School/", filetxtname) == 1){
                strcpy(task_list[i].category, "School");
                strcpy(task_list[i].schedule_check, "Scheduled");
            }
            else if(check_file_exists("./Scheduled/Family/", filetxtname) == 1){
                strcpy(task_list[i].category, "Family");
                strcpy(task_list[i].schedule_check, "Scheduled");
            }
            else{
                strcpy(task_list[i].category, "Friend");
                strcpy(task_list[i].schedule_check, "Scheduled");
            }

            printf("%s %s\n", task_list[i].category, task_list[i].schedule_check);


            strcpy(filepath, dir_path);
            strcat(filepath, "/");
            strcat(filepath, filelist->d_name);
            
            FILE* file = fopen(filepath, "r");
            printf("%s", filelist->d_name);
            if (file == NULL) {
                printf("파일을 열 수 없습니다.\n");
            }

            if (fgets(task_list[i].time, sizeof(task_list[i].time), file) == NULL) {
                printf("파일을 읽을 수 없습니다.\n");
                fclose(file);
            }
            printf("%s\n", task_list[i].time);

            if (fgets(task_list[i].flag, sizeof(task_list[i].flag), file) == NULL) {
                printf("파일을 읽을 수 없습니다.\n");
                fclose(file);
            }
            printf("%s\n", task_list[i].flag);
            if (fgets(task_list[i].group_user, sizeof(task_list[i].group_user), file) == NULL) {
                printf("파일을 읽을 수 없습니다.\n");
                fclose(file);
            }
            printf("%s\n", task_list[i].group_user);

            if (fgets(task_list[i].contents, sizeof(task_list[i].contents), file) == NULL) {
                printf("파일을 읽을 수 없습니다.\n");
                fclose(file);
            }
            printf("%s\n", task_list[i].contents);
            
            i++;
        }
    }
     
    closedir(dir);
    return task_list;
}