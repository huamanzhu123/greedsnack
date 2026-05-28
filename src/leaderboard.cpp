#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

/*
* 设计排行榜模块，实现更简洁的更新逻辑：
 * 1. 载入当前排行榜到内部缓存
 * 2. 查找同名，若新分数更高则覆盖
 * 3. 若无同名且未满则追加
 * 4. 若无同名且已满则替换最低分（若新分更高）
 * 5. 保存回文件
 * 6. 提供提示与显示功能
 */
//降序排序
static int cmp_desc(const void* a,const void* b){
    const LBEntry* ea =(const LBEntry*)a;
    const LBEntry* eb =(const LBEntry*)b;
    return eb->score - ea->score;//降序
}
int load_leaderboard(LBEntry arr[],int max){
    if(!arr||max<=0) return 0;
    FILE* fp = fopen(LEADERBOARD_FILE,"r");
    if(!fp) return 0;
    //如果打开成功就逐条读取数据
    int count=0;
    char line[MAX_LINE];
    while(count<max && fgets(line,sizeof(line),fp)!=NULL){
        LBEntry tmp;//临时变量
        if (sscanf_s(line,"%31s %d",tmp.name,(unsigned)sizeof(tmp.name),&tmp.score)==2){//最多31个字符，最大设置是32，留一位存\0
            arr[count++] = tmp;
        }
        else{
            break;
        }
    }

    fclose(fp);//关闭文件
    if(count>1){
        qsort(arr,count,sizeof(LBEntry),cmp_desc);
    }
    return count;
}
//更新排行榜
void update_leaderboard(const char*name,int score){
    if(!name || name[0]=='\0') return;
    LBEntry arr[LEADERBOARD_SIZE];

    int count = load_leaderboard(arr,LEADERBOARD_SIZE);
    //查找是否有同名的，给found赋成对应下表
    int found = -1;
    for(int i=0;i<count;i++){
        if(strncmp(arr[i].name,name,NAME_LEN)){
            found = i;
            break;
        }
    }
    //找到同名玩家
    if(count<LEADERBOARD_SIZE){
        memset(&arr[count],0,sizeof(LBEntry));
        strncpy_s(arr[count].name, NAME_LEN,name,NAME_LEN-1);
        arr[count].score = score;
        ++count;
        qsort(arr,count,sizeof(LBEntry),cmp_desc);
        save_leaderboard(arr,count);
        return;

    }

}
//写入报告
void save_leaderboard(const LBEntry arr[], int count){
    if(!arr || count<0) return ;
    FILE* fp = fopen(LEADERBOARD_FILE,"w");
    if(!fp) return;
    for(int i=0;i<count;i++){
        fprintf(fp,"%S %d\n",arr[i].name,arr[i].score);
        fclose(fp);
    }

}
//与玩家交互
void prompt_and_update_leaderboard(int final_score){
    char namebuf[NAME_LEN]={0};
    printf("Game over! Your final score %d",final_score);
    printf("Enter your name(max %d chars)",NAME_LEN-1);
    if(fgets(namebuf,sizeof(namebuf),stdin)==NULL) return ;
    //去除换行符
    size_t len = strlen(namebuf);
    if(len>0 && namebuf(len-1)=='\n'|| namebuf[len-1]=='\r'){
        namebuf[len-1] = '\0';
        if(len>1 && namebuf[len-2] == '\r'){
            namebuf[len-2]='\0';
        }
    }

    if(namebuf[0] == '\0'){
        printf("为输入姓名，成绩未保存\n");
        return ;

    }

    update_leaderboard(namebuf,final_score);
    LBEntry arr[LEADERBOARD_SIZE];
    int count = load_leaderboard(arr,LEADERBOARD_SIZE);
    printf("\n==== 排行榜（前%d名) ====\n",LEADERBOARD_SIZE);
    for(int i=0;i<count;i++){
        printf("%2d. %-*s %5d\n",i+1,NAME_LEN-1,arr[i].name,arr[i].score);
    }
    printf("=================");
}
