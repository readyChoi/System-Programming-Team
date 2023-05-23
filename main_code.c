#include <stdio.h>
#include <time.h>

// 년, 월, 일, 시간을 입력하면 unix time으로 바꿔주는 함수
time_t convertToUnixTime(int year, int month, int day, int hour) {
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


// 현재 시간과 비교하는 함수
int compareCurrentTime(time_t unixTime) {
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
void createNewTaskText(int type, int year, int month, int day, int hour, char* title, char* content) {
    time_t curTime;
    struct tm *timeInfo;
    char buffer[80];
    FILE *file;

    // 현재 유닉스 시간 가져오기
    time(&curTime);
    timeInfo = localtime(&curTime);

    // 첫 번째 줄에 현재 유닉스 시간 저장
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    file = fopen("textfile.txt", "w");
    fprintf(file, "%s\n", str(timeInfo));

    // 두 번째 줄에 문자열 저장
    fprintf(file, "Hello, world!");

    fclose(file);
}