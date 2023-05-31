#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>

struct ThreadArgs {
    char* arg1;
    char* arg2;
};


void main(int argc, char* argv[]){
    pthread_t today_check_family, today_check_friend, today_check_school;
    pthread_t alarm_family, alarm_friend, alram_school;

    void *today_check(void*);

    struct ThreadArgs today_check_familiy_args;
    today_check_familiy_args.arg1 = "./Scheduled/Family/";
    today_check_familiy_args.arg2 = "/../../Today/Family/";
    pthread_create(&today_check_family, NULL, today_check, (void*)&today_check_familiy_args);


    struct ThreadArgs today_check_friend_args;
    today_check_friend_args.arg1 = "./Scheduled/Friend/";
    today_check_friend_args.arg2 = "/../../Today/Friend/";
    pthread_create(&today_check_friend, NULL, today_check, (void*)&today_check_friend_args);


    struct ThreadArgs today_check_school_args;
    today_check_school_args.arg1 = "./Scheduled/School/";
    today_check_school_args.arg2 = "/../../Today/School/";
    pthread_create(&today_check_school, NULL, today_check, (void*)&today_check_school_args);


    pthread_join(today_check_family, NULL);
    pthread_join(today_check_friend, NULL);
    pthread_join(today_check_school, NULL);
}

int compareCurrentTime(time_t unixTime){
    time_t currentTime = time(NULL);
    printf("%ld, %ld", unixTime, currentTime);

    // 현재 시간보다 이전
    if(unixTime < currentTime) 
    {
        return 0;
    }
    // 현재 시간보다는 후지만 오늘
    else if((unixTime >= currentTime) && (unixTime/86400 == currentTime/86400)) 
    {
        return 1;
    }
    // 현재 시간보다도 후고 오늘도 아님
    else
    {
        return 2;
    }
}

void* today_check(void* arg){
    struct ThreadArgs* thread_args = (struct ThreadArgs*)arg;

    char* dir_path = thread_args->arg1;
    char* next_path = thread_args->arg2;

    DIR *dir;
    char line[100];
    struct dirent *filelist;
    int task_number;
    time_t task_unix_time;
    int flag_num;
    int fd;
    FILE *taskFile = NULL;

    while(1){
        printf("%s\n", dir_path);
        if((dir = opendir(dir_path)) == NULL){ // opendir
            perror("cannot open directory");
            exit(1);
        }

        while((filelist = readdir(dir)) != NULL){ // readdir
            if(filelist->d_type == DT_REG){
                /* make filepath */
                char filepath[256];
                char nextpath[256];
                strcpy(filepath, dir_path);
                strcat(filepath, filelist->d_name);
                strcpy(nextpath, dir_path);
                strcat(nextpath, next_path);

                printf("%s\n", filepath);

                if((taskFile = fopen(filepath, "r")) == NULL){ // open file
                    perror("cannot open file");
                    exit(1);
                }

                if(fgets(line, 100, taskFile) != NULL){
                    printf("%s\n", line);
                    task_number = atoi(line);
                    task_unix_time = (time_t)task_number;
                    printf("First number: %d\n", task_number);
                }

                if(fgets(line, 100, taskFile) != NULL){
                    flag_num = atoi(line);
                    printf("First number: %d\n", flag_num);
                }

                fclose(taskFile);

                if(compareCurrentTime(task_unix_time) != 2){
                    if(rename(filepath, strcat(nextpath, filelist->d_name)) == -1){
                        printf("error");
                    }
                    printf("ddddd %s\n", nextpath);
                }
            }
        }

        sleep(1);
    }

    return NULL;
}
