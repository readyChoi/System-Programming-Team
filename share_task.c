#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define CUR_PATH "./"

int create_link(char *username, char *taskpath) {
    char new_line_content[256];
    char *usr_dir = username;
    char *taskname = strchr(taskpath, '/');
    char newlink[256];
    int target_line_number = 3;

    /* parse taskpath */
    if (taskname != NULL) { // found
        taskname = taskname + 1;
    } else { // no '/' found -> just save taskpath

        *taskname = *taskpath;
    }
    /* make newlink */

    strcpy(newlink, CUR_PATH);
    strcat(newlink, usr_dir);
    strcat(newlink, "/");
    strcat(newlink, taskname); // newlink to save

    /* add user to the group */
    FILE *file = fopen(taskpath, "r+");
    char line[1000];
    int current_line = 1;

    strcpy(new_line_content, " ");
    strcat(new_line_content, username);

    while (fgets(line, sizeof(line), file) != NULL) {
        if (current_line == target_line_number) {
            fseek(file, -strlen(line), SEEK_CUR);
            fprintf(file, "%s", new_line_content); // 수정된 줄 내용 쓰기
            break;
        }
        current_line++;
    }
    fclose(file);

    /* create hard link */
    if (link(taskpath, newlink) != 0) {
        perror("Error creating hard link");
    }
    return 0;
}

int main() {
    char *usr_name = "js";
    char *task_path = "./code";
    create_link(usr_name, task_path);
    return 0;
}