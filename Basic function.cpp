//
//  Basic function.cpp
//  数据库
//
//  Created by Mac on 2020/5/9.
//  Copyright © 2020 Mac. All rights reserved.
//

#include "Basic function.h"
#include <stdio.h>
#include <time.h>


int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int printInputOrder(void *NotUsed, int argc, char **argv, char **azColName)
{
    printf("%s = %s\n", "订单编号", argv[0] ? argv[0] : "NULL");
    printf("%s = %s\n", "图书编号", argv[1] ? argv[1] : "NULL");
    printf("%s = %s\n", "图书名称", argv[2] ? argv[2] : "NULL");
    printf("%s = %s\n", "进货单价", argv[3] ? argv[3] : "NULL");
    printf("%s = %s\n", "进货数量", argv[4] ? argv[4] : "NULL");
    printf("%s = %s\n", "订单总价", argv[5] ? argv[5] : "NULL");
    printf("%s = %s\n", "订单状态", argv[6] ? argv[6] : "NULL");
   printf("\n");
   return 0;
}

int printOrder(void *NotUsed, int argc, char **argv, char **azColName)
{
    printf("%s = %s\n", "账单编号", argv[0] ? argv[0] : "NULL");
    printf("%s = %s\n", "操作时间", argv[1] ? argv[1] : "NULL");
    printf("%s = %s\n", "收支类型", argv[2] ? argv[2] : "NULL");
    printf("%s = %s\n", "数额", argv[3] ? argv[3] : "NULL");
   printf("\n");
   return 0;
}

void gettime(char Time[])
{
    struct tm *newtime;
    //char tmpbuf[128];
    time_t lt1;
    time(&lt1 );
    newtime=localtime(&lt1);
    strftime(Time, 128, "%F %T", newtime);
}



