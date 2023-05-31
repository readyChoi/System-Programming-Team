#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#define CUR_PATH "./"

char* extract_filename(const char* string) {
    const char* start = strrchr(string, '/') + 1; // 파일 이름 시작 위치
    const char* end = strrchr(string, '.');       // 파일 확장자 위치

    // 파일 이름의 길이 계산
    size_t length = (end != NULL) ? (end - start) : strlen(start);

    // 파일 이름을 새로운 문자열로 복사
    char* result = (char*)malloc(length + 1);
    strncpy(result, start, length);
    result[length] = '\0';

    return result;
}

void remove_last_newline(char* string) {
    size_t length = strlen(string);

    if (length > 0 && string[length - 1] == '\n') {
        string[length - 1] = '\0';
    }
}

int create_link(char *username, char *taskpath) {
    char new_line_content[256];
    char *usr_dir = username;
    char newlink[256];
    char old_path[100];
    strcpy(old_path, taskpath);
    printf("%s", old_path);

    char* extracted_filename = extract_filename(taskpath);
    printf("%s\n", extracted_filename);
    fflush(stdout);

    struct stat st;
    if (stat(username, &st) == -1) {
        // 디렉토리가 존재하지 않는 경우에만 생성
        if (mkdir(username, 0777) == -1) {
            perror("mkdir error");
            exit(EXIT_FAILURE);
        }
    }

    strcpy(newlink, CUR_PATH);
    strcat(newlink, username);
    strcat(newlink, "/");
    strcat(newlink, extracted_filename);

    printf("%s\n", newlink);

    // /* add user to the group */
    FILE *file = fopen(old_path, "r+");
    char line[1000];
    int current_line = 1;

    strcpy(new_line_content, " ");
    strcat(new_line_content, username);

    int target_line_number = 3;

    // 임시 파일을 생성하여 수정된 내용을 기록
    FILE* temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        // 임시 파일 생성 실패 처리
        perror("Failed to create temporary file");
        return -1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (current_line == target_line_number) {
            remove_last_newline(line);
            fprintf(temp_file, "%s%s\n", line, new_line_content); // 수정된 줄 내용 쓰기
        } else {
            fprintf(temp_file, "%s", line); // 그대로 임시 파일에 기록
        }

        current_line++;
    }

    fclose(file);
    fclose(temp_file);

    // 기존 파일을 삭제하고 임시 파일을 이름을 변경하여 원본 파일로 대체
    remove(old_path);
    rename("temp.txt", old_path);


    // /* create hard link */
    if (link(old_path, newlink) != 0) {
       perror("Error creating hard link");
    }
    return 0;
}

int main() {
    char *usr_name = "js";
    char *task_path = "./Today/Friend/test.txt";
    create_link(usr_name, task_path);
    return 0;
}