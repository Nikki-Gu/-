//
//  main.cpp
//  数据库
//
//  Created by Mac on 2020/5/7.
//  Copyright © 2020 Mac. All rights reserved.
//
#include <iostream>
#import <sqlite3.h>
#include "ConsoleApplication1.h"
#include "view.h"
#include "Shop.h"
#include "Basic function.h"

using namespace std;
#define scanf_s scanf
int main()
{
    sqlite3* db = NULL;
    int rc;
    int op = 0;
    cr rt;
    char Time[20];
    gettime(Time);
    /* Open database */
    rc = sqlite3_open("/Users/mac/Desktop/Book.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }
    //界面
    while (1)
    {
        op = Enter_Options(db);
        switch (op)
        {
        case 1:
            //选书
            selectBook(db);
            break;
        case 2:
            //登录
            rt = Man_Login(db);
            if (rt.flag == 0);
            else if (rt.flag == 1)
                Common_Man_Options(db);
            else
                Super_Man_Options(db);
            break;
        case 3:
            sqlite3_close(db);
            return 0;
        }
    }
    return 0;
}

//int main(int argc, char* argv[])
//{
//    sqlite3 *db;
//    int rc;
//    /* Open database */
//    rc = sqlite3_open("/Users/mac/Desktop/Book.db", &db);
//    if( rc )
//    {
//      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//      exit(0);
//    }
//    else
//    {
//      fprintf(stderr, "Opened database successfully\n");//测试用，之后可以删掉
//    }
//
//    while(1)
//    {
//        //主界面
//        int num; //操作编号
//        printf("*************************************\n");
//        printf("           图书管理系统\n");
//        printf("*************************************\n");
//        printf("1:用户管理\t2.书籍查询\t3.图书信息修改\n4.进货管理\t5.图书购买\t");
//        printf("6:查看账单\n7.退出系统\n请输入操作编号：");
//        cin >> num;
//        switch(num)
//        {
//            case 1: break;
//            case 2:
//            {
//                char *Bookisbn = (char*)malloc(sizeof(char)*15);
//                printf("请输入需要查询的Bookisbn：");//第一本书的isbn9787506365437
//                scanf("%s",Bookisbn);
//                cin.clear();cin.sync();
//                searchBook(db,Bookisbn);
//                  Man_Login(db);
//                  break;
//            }
//            case 3: break;
//            case 4: GUIofInput(db);break;
//            case 5: break;
//            case 6: viewallOrder(db);break;
//            case 7:
//            {
//                /* Close database */
//                sqlite3_close(db);
//                printf("已退出系统\n");
//                return 0;
//            }
//            default: printf("请输入正确的操作编号\n");
//        }
//    }
//}

