#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define F (50)         
#define H (9)
struct HANDAN {
    char handan_name[F];
    int kaisuu;
    long time;
    bool count;
};

struct LOG {
    long time;
    char server_name[F];
    char result[F];
};

void printTime(long kikan)
{
    char kikan2[14];
    sprintf(kikan2, "%ld", kikan);
    int len = strlen(kikan2);
    long sec, min, hour, day, month, year;
    year = kikan / 10000000000;
    month = (kikan / 1000000) % 100;
    day = (kikan / 10000) % 100;
    hour = (kikan / 100) % 100;
    min = kikan / 100;
    sec = kikan % 100;

    if (sec >= 60) {
        sec = sec - 60;
        min = min + 1;
    }
    if (min >= 60) {
        sec = min - 60;
        hour = hour + 1;
    }
    //printf("%ld年%ld月%ld日%ld時%ld分です\n",year,month,day,hour,min);
    //printf("kikan is %ld\n",kikan);
if (len <= 2) {
        printf("故障期間は%ld秒です\n", sec);
    } else if (len <= 4) {
        printf("故障期間は%ld分%ld秒です\n", min, sec);
    } else if (len <= 6) {
        printf("故障期間は%ld時%ld分%ld秒です\n", hour, min, sec);
    } else if (len <= 8) {
        printf("故障期間は%ld日%ld時%ld分%ld秒です\n", day, hour, min,
               sec);
    } else if (len <= 10) {
        printf("故障期間は%ld月%ld日%ld時%ld分%ld秒です\n", month, day,
               hour, min, sec);
    } else if (len <= 14) {
        printf("故障期間は%ld年%ld月%ld日%ld時%ld分%ld秒です\n", year,
               month, day, hour, min, sec);
    }
}
int main()
{
    FILE *fp;
    struct HANDAN han[H];
    struct LOG log;
    char s[F], *cp;
    const char *sikiri = ",";
    long kikan;
    bool find;
    int i = 0,  h = 0;
    int han_ctr = 0;                                
    fp = fopen("log.txt", "r");

    while (fgets(s, F, fp) != NULL) {
        cp = strtok(s, sikiri);
        if (cp == NULL) continue;
        log.time = atol(cp);                    
        cp = strtok(NULL, sikiri);
        if (cp == NULL) continue;
        strcpy(log.server_name, cp);            
        cp = strtok(NULL, sikiri);
        if (cp == NULL) continue;
        strcpy(log.result, cp);               
        if (strncmp(log.result, "-",1) == 0) {    
            find = false;
            for (h = 0; h < han_ctr; h++){     
                if (strcmp(log.server_name, han[h].handan_name) == 0){
                    find = true;
                    break;
                }
            }
          
            if (find == false){
                if (han_ctr >= H){ 
                    printf("判断テーブル上限:%d オーバー |%ld|%s|の故障監視をスキップ\n",H,log.time,log.server_name);
                    continue;
                }
               
                h = han_ctr;
                strcpy(han[h].handan_name,log.server_name);
                han[h].count = false;
                han[h].time = 0;
                han[h].kaisuu = 0;
                han_ctr++;
            }
            han[h].kaisuu++; 
            if (han[h].count == false){ 
                                       
                han[h].count = true;
                han[h].time = log.time;
printf("サーバー%sが故障状態です\n",
                               han[h].handan_name);
                        printf("log.time is %ld\nhandan is %ld\n",
                               log.time, han[h].time);
                        printTime(kikan);
            }
        }else{
            
            for (h = 0; h < han_ctr; h++) {
                if (strcmp(log.server_name, han[h].handan_name) == 0) {
                    if (han[h].count == true){   
                        kikan = log.time - han[h].time;
                        printf("サーバー%sが故障状態です\n",
                               han[h].handan_name);
                        printf("log.time is %ld\nhandan is %ld\n",
                               log.time, han[h].time);
                        printTime(kikan);
                        han[h].count = false;
                    }
                    continue;   
                }
            }
        }
    }

    fclose(fp);
    return 0;
}
