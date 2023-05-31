#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <ncurses.h>
#include <sys/wait.h>

struct ThreadArgs {
    char* arg1;
    char* arg2;
};

struct ThreadArgs_alarm {
    char* arg1;
};


void main(int argc, char* argv[]){
    pthread_t today_check_family, today_check_friend, today_check_school;
    pthread_t alarm_family, alarm_friend, alarm_school;

    void *today_check(void*);
    void *alarm_check(void*);

    struct ThreadArgs today_check_familiy_args;
    today_check_familiy_args.arg1 = "./Scheduled/Family/";
    today_check_familiy_args.arg2 = "/../../Today/Family/";
    //pthread_create(&today_check_family, NULL, today_check, (void*)&today_check_familiy_args);

    struct ThreadArgs today_check_friend_args;
    today_check_friend_args.arg1 = "./Scheduled/Friend/";
    today_check_friend_args.arg2 = "/../../Today/Friend/";
    //pthread_create(&today_check_friend, NULL, today_check, (void*)&today_check_friend_args);

    struct ThreadArgs today_check_school_args;
    today_check_school_args.arg1 = "./Scheduled/School/";
    today_check_school_args.arg2 = "/../../Today/School/";
    //pthread_create(&today_check_school, NULL, today_check, (void*)&today_check_school_args);


    struct ThreadArgs_alarm alarm_check_familiy_args;
    alarm_check_familiy_args.arg1 = "./Today/Family/";
    pthread_create(&alarm_family, NULL, alarm_check, (void*)&alarm_check_familiy_args);

    struct ThreadArgs_alarm alarm_check_friend_args;
    alarm_check_friend_args.arg1 = "./Today/Friend/";
    pthread_create(&alarm_friend, NULL, alarm_check, (void*)&alarm_check_friend_args);

    struct ThreadArgs_alarm alarm_check_school_args;
    alarm_check_school_args.arg1 = "./Today/School/";
    pthread_create(&alarm_school, NULL, alarm_check, (void*)&alarm_check_school_args);

    //pthread_join(today_check_family, NULL);
    //pthread_join(today_check_friend, NULL);
    //pthread_join(today_check_school, NULL);
    pthread_join(alarm_family, NULL);
    pthread_join(alarm_friend, NULL);
    pthread_join(alarm_school, NULL);
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

                printf("\n%s\n", filepath);

                if((taskFile = fopen(filepath, "r")) == NULL){ // open file
                    perror("cannot open file");
                    exit(1);
                }

                if(fgets(line, 100, taskFile) != NULL){
                    task_number = atoi(line);
                    task_unix_time = (time_t)task_number;
                    printf("task number: %d\n", task_number);
                }

                if(fgets(line, 100, taskFile) != NULL){
                    flag_num = atoi(line);
                    printf("flag number: %d\n", flag_num);
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


void* alarm_check(void* arg){
    struct ThreadArgs* thread_args = (struct ThreadArgs*)arg;

    char* dir_path = thread_args->arg1;

    DIR *dir;
    char line[100];
    char buffer[1000];
    struct dirent *filelist;
    int task_number;
    time_t task_unix_time;
    int flag_num;
    int fd;
    int file;
    char* token;
    char* login_token;
    int line_count;
    const int target_line_number = 2;
    const char* new_line_content = "1";
    FILE *taskFile = NULL;
    FILE *loginFile = NULL;
    int login_check = 0;
    char username[30];
    char terminal_name[30];

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

                strcpy(filepath, dir_path);
                strcat(filepath, filelist->d_name);

                printf("\n%s\n", filepath);

                if((taskFile = fopen(filepath, "r")) == NULL){ // open file
                    perror("cannot open file");
                    exit(1);
                }

                if(fgets(line, 100, taskFile) != NULL){
                    printf("%s\n", line);
                    task_number = atoi(line);
                    task_unix_time = (time_t)task_number;
                    printf("task number: %d\n", task_number);
                }

                if(fgets(line, 100, taskFile) != NULL){
                    flag_num = atoi(line);
                    printf("flag number: %d\n", flag_num);
                }

                if(fgets(line, 100, taskFile) != NULL){
                    printf("\nline: %s\n", line);
                }


                fclose(taskFile);

                if(compareCurrentTime(task_unix_time) == 0){
                    if(flag_num == 0){
                        token = strtok(line, " ");
                        while (token != NULL) {
                            printf("Token: %s\n", token);

                            if((loginFile = fopen("./login.txt", "r")) == NULL){
                                printf("error\n");
                            }
                            if((taskFile = fopen(filepath, "r")) == NULL)
                            {
                                perror("cannot open file");
                                exit(1);
                            }

                            char login_line[1000];
                            while (fgets(login_line, sizeof(login_line), loginFile) != NULL) {
                                if (sscanf(login_line, "%[^:]:%s", username, terminal_name) == 2) {
                                    printf("First string: %s\n", username);
                                    printf("Second string: %s\n", terminal_name);
                                }

                                if(strcmp(token, username) == 0){
                                    printf("%s\n\n", terminal_name);
                                     FILE* terminal_file = fopen(terminal_name, "w");
                                     fprintf(terminal_file, "\033[?5h");
                                     char message[100] = "\n";
                                     strcat(message, filelist->d_name);
                                     strcat(message, " alarm occurs\n");
                                     fprintf(terminal_file, "%s", message);
                                     fflush(terminal_file);
                                     fprintf(terminal_file, "\033[?5l");
                                     fclose(terminal_file);
                                }
                            }

                            // signal 보내야함
                            // signal(token, filelist->d_name);
                            token = strtok(NULL, " ");
                        }
                        token = NULL;

                        line_count = 0;
                        int line_start_pos = 0;
                        int bytes_read;

                        FILE* file = fopen(filepath, "r+");
                        char line[1000];
                        int current_line = 1;

                        while (fgets(line, sizeof(line), file) != NULL) {
                            if (current_line == target_line_number) {
                                fseek(file, -strlen(line), SEEK_CUR);  // 현재 위치에서 이전 줄의 시작 위치로 이동
                                fprintf(file, "%s\n", new_line_content);  // 수정된 줄 내용 쓰기
                                break;
                            }
                            current_line++;
                        }

                        fclose(file);
                    }
                }
            }
        }

        sleep(1);
    }

    return NULL;
}