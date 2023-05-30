#include <stdio.h>
#include <time.h>
#include <string.h>

#define CUR_PATH "./"

// 년, 월, 일, 시간을 입력하면 unix time으로 바꿔주는 함수
time_t convertToUnixTime(int year, int month, int day, int hour){
    struct tm timeInfo;
    timeInfo.tm_year = year - 1900;
    timeInfo.tm_mon = month - 1;
    timeInfo.tm_mday = day;
    timeInfo.tm_hour = hour;
    timeInfo.tm_min = 0;
    timeInfo.tm_sec = 0;
    timeInfo.tm_isdst = -1;

    time_t unixTime = mktime(&timeInfo);

    return unixTime;
}

// unix time 을 입력하면 년, 월, 일, 시간 int 배열을 리턴해주는 함수
void convertToRealTime(int unixTime, int* timeArr){
    time_t t = unixTime;
    struct tm* timeinfo;
    timeinfo = localtime(&t);

    timeArr[0] = timeinfo->tm_year + 1900;  // 년
    timeArr[1] = timeinfo->tm_mon + 1;      // 월
    timeArr[2] = timeinfo->tm_mday;         // 일
    timeArr[3] = timeinfo->tm_hour;         // 시간
}


// 현재 시간과 비교하는 함수
int compareCurrentTime(time_t unixTime){
    time_t currentTime = time(NULL);

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



// 새로운 task 파일 만들기
void createNewTaskText(int type, int year, int month, int day, int hour, char* title, char* content, char* username) {
    struct tm *timeInfo;
    char saveAdddress[100];
    time_t taskTime;
    int compareResult;
    taskTime = convertToUnixTime(year, month, day, hour);

    compareResult = compareCurrentTime(taskTime);

    char timeString[20];
    sprintf(timeString, "%ld", (long)taskTime);
    printf("%s", timeString);

    FILE *file;

    // 현재 유닉스 시간 가져오기
    time_t curTime;
    time(&curTime);
    timeInfo = localtime(&curTime);
    if(compareResult == 2){
        strcat(saveAdddress, CUR_PATH);
        strcat(saveAdddress, "Scheduled/");
        if(type == 0){
            strcat(saveAdddress, "Family/");
        }
        else if(type == 1){
            strcat(saveAdddress, "Friend/");
        }
        else if(type == 2){
            strcat(saveAdddress, "School/");
        }
    }
    else{
        strcat(saveAdddress, CUR_PATH);
        strcat(saveAdddress, "Today/");
        if(type == 0){
            strcat(saveAdddress, "Family/");
        }
        else if(type == 1){
            strcat(saveAdddress, "Friend/");
        }
        else if(type == 2){
            strcat(saveAdddress, "School/");;
        }
    }

    strcat(saveAdddress, title);
    strcat(saveAdddress, ".txt");

    file = fopen(saveAdddress, "w");
    fprintf(file, "%s\n", timeString); 

    char* flagString = "0";
    fprintf(file, "%s\n", flagString);

    // 두 번째 줄에 문자열 저장
    fprintf(file, "%s", content);

    fclose(file);

    // create_link(username, saveAddress);
}

int main(){
    char* title;
    title = "task1";
    char* content;
    content = "test\nteststse\ntest\n";
    char *usr_name = "choi";
    createNewTaskText(1, 2022, 5, 20, 4, title, content, usr_name);
}