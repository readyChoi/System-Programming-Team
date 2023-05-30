#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define CUR_PATH "./"

int create_link(char *username, char *taskpath){
    char *usr_dir = username;
    char *taskname =strchr(taskpath, '/');
    char newlink[100];
    /* parse taskpath */    
    if(taskname != NULL){ // found
        taskname = taskname+1;
    }else{ // no '/' found -> just save taskpath
        *taskname = *taskpath;
    }

    /* make newlink */
    strcat(newlink, CUR_PATH);
    strcat(newlink, usr_dir);
    strcat(newlink, "/");
    strcat(newlink, taskname);

    if (link(taskpath, newlink) != 0) {
        perror("Error creating hard link");
    }

    return 0;

}


int main() {
    char *usr_name = "js";
    char *task_path = "code"; zz
    
    create_link(usr_name, task_path);
    return 0;
}