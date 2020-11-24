//
//  Book.cpp
//  数据库
//
//  Created by Mac on 2020/5/10.
//  Copyright © 2020 Mac. All rights reserved.
//

#include <stdio.h>
#import <sqlite3.h>
#include <stdlib.h>
#include "Book.h"
#include "view.h"
//书籍查询
void print_Bookinfo(sqlite3* db, const char* sql, sqlite3_stmt* stmt)
{
    int flag = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            flag++;
            const unsigned char* pTmp;
            int num;
            num = sqlite3_column_int(stmt, 0);
            printf("BookID: %d\n", num);
            pTmp = sqlite3_column_text(stmt, 4);
            printf("Bookisbn: %s\n", pTmp);
            pTmp = sqlite3_column_text(stmt, 5);
            printf("BookName: %s\n", pTmp);
            pTmp = sqlite3_column_text(stmt, 6);
            printf("BookPress: %s\n", pTmp);
            pTmp = sqlite3_column_text(stmt, 7);
            printf("BookPubDate: %s\n", pTmp);
            pTmp = sqlite3_column_text(stmt, 8);
            printf("BookAuthor: %s\n", pTmp);
            num = sqlite3_column_int(stmt, 1);
            printf("BookStoreMount: %d\n", num);
            pTmp = sqlite3_column_text(stmt, 2);
            printf("BookStoretime: %s\n", pTmp);
            pTmp = sqlite3_column_text(stmt, 3);
            printf("BookMPrice: %s\n", pTmp);
            printf("---------------------------------\n");
        }
        if (flag == 0)
        {
            printf("库存中无该书籍\n");
        }
        sqlite3_finalize(stmt);
}
void Book_search(sqlite3* db)
{
    const char* sql = nullptr;
    int rc;
    sqlite3_stmt* stmt;
    int op;
    char info[25] = {};
    int bookID;

    while (1)
    {
        Book_search_Options();
        printf("请输入选项：");
        scanf("%d", &op);

        switch (op) {
        case 1:
            sql = "select * from tb_Storage natural join tb_Book  where BookID=?";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK)
            {
                printf("请输入ID：");
                scanf("%d", &bookID);
                sqlite3_bind_int(stmt, 1, bookID);
                print_Bookinfo(db, sql, stmt);
            }
            break;
        case 2:
            sql = "select * from tb_Storage natural join tb_Book  where Bookisbn=?";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK)
            {
                printf("请输入ISBN：");
                scanf("%s", info);
                sqlite3_bind_text(stmt, 1, info, -1, 0);
                print_Bookinfo(db, sql, stmt);
            }
            break;
        case 3:
            sql = "select * from tb_Storage natural join tb_Book  where BookName=?";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK)
            {
                printf("请输入名称：");
                scanf("%s", info);
                sqlite3_bind_text(stmt, 1, info, -1, 0);
                print_Bookinfo(db, sql, stmt);
            }
            break;
        case 4:
            sql = "select * from tb_Storage natural join tb_Book  where BookAuthor=?";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK)
            {
                printf("请输入作者：");
                scanf("%s", info);
                sqlite3_bind_text(stmt, 1, info, -1, 0);
                print_Bookinfo(db, sql, stmt);
            }
            break;
        case 5:
            sql = "select * from tb_Storage natural join tb_Book  where BookPress=?";
            rc = sqlite3_prepare_v2(db, sql,-1, &stmt, NULL);
            if (rc == SQLITE_OK)
            {
                printf("请输入出版社：");
                scanf("%s", info);
                sqlite3_bind_text(stmt, 1, info, -1, 0);
                print_Bookinfo(db, sql, stmt);
            }
            break;
        default:
            printf("请输入正确选项！");
            continue;
        }
        break;
    }
}

//图书信息修改
void updatefunc(sqlite3* db, sqlite3_stmt* stmt, const char* sql,int bookID,char * newinfo)
{
    int rc;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, newinfo, -1, 0);
        sqlite3_bind_int(stmt, 2, bookID);
        rc = sqlite3_step(stmt);
        rc = sqlite3_finalize(stmt);
        printf("修改成功！\n");
    }
}
int updateBookinfo(sqlite3* db)
{
    const char* sql = nullptr;
    int rc;
    int op;
    char newinfo[25] = {};
    int bookID = 0;
    sqlite3_stmt* stmt = NULL;

    while (1)
    {
        printf("请输入想要修改书籍的ID：");
        scanf("%d", &bookID);
        //判断库存中是否有这本书
        sql = "select* from tb_Storage where BookID=?";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 1, bookID);
            int flag = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW)
                flag++;
            if (flag == 0)
            {
                printf("库存中无该书籍,请重新输入\n");
                sqlite3_finalize(stmt);
                continue;
            }
            sqlite3_finalize(stmt);
            break;
        }
    }
    

    while (1) {
        update_bookinfo_Options();
        printf("请输入选项：");
        scanf("%d", &op);

        switch (op) {
        case 1:
            sql = "update tb_Book set Bookisbn=? where BookID=?";
            printf("请输入新的ISBN：");
            scanf("%s", newinfo);
            updatefunc(db, stmt, sql, bookID, newinfo);
            break;
        case 2:
            sql = "update tb_Book set BookName=? where BookID=?";
            printf("请输入新的名称：");
            scanf("%s", newinfo);
            updatefunc(db, stmt, sql, bookID, newinfo);
            break;
        case 3:
            sql = "update tb_Book set BookPress=? where BookID=?";
            printf("请输入新的出版社：");
            scanf("%s", newinfo);
            updatefunc(db, stmt, sql, bookID, newinfo);
            break;
        case 4:
            sql = "update tb_Book set BookPubDate=? where BookID=?";
            printf("请输入新的出版日期（按照格式yyyy/mm/dd输入)：");
            scanf("%s", newinfo);
            updatefunc(db, stmt, sql, bookID, newinfo);
            break;
        case 5:
            sql = "update tb_Book set BookAuthor=? where BookID=?";
            printf("请输入新的作者：");
            scanf("%s", newinfo);
            updatefunc(db, stmt, sql, bookID, newinfo);
            break;
        case 6:
            sql = "update tb_Storage set BookMPrice=? where BookID=?";
            printf("请输入新的售价：");
            scanf("%s", newinfo);
            updatefunc(db, stmt, sql, bookID, newinfo);
            break;
        case 7:
            return 0;
        default:
            printf("请输入正确选项！");
            continue;
        }
        break;
    }
    return 0;
}

//书籍信息修改选项
void update_bookinfo_Options() {
    printf("请选择：\n");
    printf("1.修改ISBN\n");
    printf("2.修改名称\n");
    printf("3.修改出版社\n");
    printf("4.修改出版日期\n");
    printf("5.修改作者\n");
    printf("6.修改售价\n");
    printf("7.退出\n");
}
//书籍查询方式选项
void Book_search_Options() {
    printf("请选择查询方式：\n");
    printf("1.ID\n");
    printf("2.ISBN\n");
    printf("3.名称\n");
    printf("4.作者\n");
    printf("5.出版社\n");
}
