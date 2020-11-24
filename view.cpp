#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"sqlite3.h"
#include <iostream>
#include "view.h"
#include "ConsoleApplication1.h"
#include "Shop.h"
#include "Book.h"
#include "Input.h"
//#include <windows.h>
//入口界面选项
int Enter_Options(sqlite3* db) {
    int op;
    while (1) //操作选项不正确进入循环
    {
        printf("*************************************\n");
        printf("* 欢迎来到图书管理与查询系统!\n");
        printf("* 1.书籍购买\n");
        printf("* 2.管理员登录\n");
        printf("* 3.退出系统\n");
        printf("*************************************\n");

        printf("请输入选项：");
        scanf("%d", &op);

        switch (op) {
        case 1:
            return op;
        case 2:
            return op;
        case 3:
            return op;
        default:
            printf("请输入正确选项！\n");
            continue;
        }
    }
}

//超级管理员选项
int Super_Man_Options(sqlite3* db) {
    int op;
    while (1) //操作选项不正确进入循环
    {
        printf("请选择操作：\n");
        printf("1.创建账户\n");
        printf("2.查看用户资料\n");
        printf("3.修改用户信息\n");
        printf("4.书籍查询\n");
        printf("5.图书信息修改\n");
        printf("6.进货管理\n");
        printf("7.查看账单\n");
        printf("8.返回登录界面\n");

        printf("请输入选项：");
        scanf("%d", &op);
        switch (op) {
        case 1:
            //创建账户 
            Guidein(db);
            break;
        case 2:
            //查看用户资料 
            viewManinfo(db, 2);
            break;
        case 3:
            //修改用户信息
            updateManinfo(db, 2);
            break;
        case 4:
            //书籍查询
            Book_search(db);
            break;
        case 5:
            //图书信息修改
            updateBookinfo(db);
            break;
        case 6:
            //图书进货
            GUIofInput(db);
            break;
        case 7:
            //查看账单
            viewallOrder(db);
            break;
        case 8:
            //返回登录界面
            return op;
        default:
            printf("请输入正确选项！\n");
            continue;
        }
    }
}

//普通管理员选项
int Common_Man_Options(sqlite3* db) {
    int op;
    while (1) //操作选项不正确进入循环
    {
        printf("请选择操作：\n");
        printf("1.查看个人资料\n");
        printf("2.修改个人信息\n");
        printf("3.书籍查询\n");
        printf("4.图书信息修改\n");
        printf("5.图书进货\n");
        printf("6.查看账单\n");
        printf("7.返回登录界面\n");

        printf("请输入选项：");
        scanf("%d", &op);
        switch (op) {
        case 1:
            //查看个人资料 
            viewManinfo(db, 1);
            break;
        case 2:
            //修改个人信息
            updateManinfo(db, 1);
            break;
        case 3:
            //书籍查询
            Book_search(db);
            break;
        case 4:
            //图书信息修改
            updateBookinfo(db);
            break;
        case 5:
            //图书进货
            GUIofInput(db);
            break;
        case 6:
            //查看账单
            viewallOrder(db);
            break;
        case 7:
            //返回登录界面
            return op;
        default:
            printf("请输入正确选项！\n");
            continue;
        }
    }
}

//用户信息修改选项
void update_userinfo_Options() {
    printf("请选择：\n");
    printf("1.修改用户名\n");
    printf("2.修改密码\n");
    printf("3.修改姓名\n");
    printf("4.修改性别\n");
    printf("5.修改年龄\n");
    printf("6.退出\n");
}

























