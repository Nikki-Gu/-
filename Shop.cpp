//
//  Shop.cpp
//  数据库
//
//  Created by Mac on 2020/5/10.
//  Copyright © 2020 Mac. All rights reserved.
//

#include <stdio.h>
#import <sqlite3.h>
#include <stdlib.h>
#include <string>
#include "Basic function.h"
#include "Shop.h"
#include "view.h"
/**************** 购书界面操作 ****************/
//选书
bk b[20];
int cnt = 0;

int selectBook(sqlite3* db)
{
    char isbn[25] = {};
    sr r;
    int op = 0;
    int rc;
    sqlite3_stmt* stmt;
    const char* sql = nullptr;
    int leave = 0;

    while (1)
    {
        while (1) //选到库存中没有的书进入循环
        {
            printf("请输入选购书籍ISBN：");
            scanf("%s", isbn);
            //判断库中有无这本书
            sql = "select * from tb_Storage natural join tb_Book where Bookisbn =?";
            rc = sqlite3_prepare(db, sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK) {
                int flag = 0;
                sqlite3_bind_text(stmt, 1, isbn, -1, 0);
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    const unsigned char* pTmp;
                    int num;
                    num = sqlite3_column_int(stmt, 0);
                    printf("BookID: %d\n", num);
                    r.BookID = num;
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
                    r.BookStoreMount = num;
                    pTmp = sqlite3_column_text(stmt, 3);
                    printf("BookMPrice: %s\n", pTmp);
                    r.price = atof((char*)pTmp);
                    printf("---------------------------------\n");
                    flag++;
                }
                if (flag == 0)
                {
                    printf("本书库暂无该书库存\n");
                    sqlite3_finalize(stmt);
                    int choice = 0;
                    while (1)
                    {
                        printf("请选择：\n");
                        printf("1.继续选书\n");
                        printf("2.查看购物车（可放弃购买）\n");
                        printf("请输入选项：");
                        scanf("%d", &choice);
                        if (choice == 1 or choice == 2)
                            break;
                        else
                            printf("请输入正确选项！\n");
                    }
                    if (choice == 1)
                        continue;
                    else if (choice == 2)
                    {
                        leave=show_shopbasket(db);
                        if (leave == 1)
                        {
                            cnt = 0;
                            memset(b, 0, sizeof(b));
                            return 0;
                        }
                    }
                }
                sqlite3_finalize(stmt);
                break;
            }
        }
        printf("这是否为你想要的购买的书籍？\n");
        printf("请选择：\n");
        printf("1.是\n");
        printf("2.不是\n");
        int choice = 0;
        printf("请输入选项：");
        scanf("%d", &choice);
        if (choice == 1)
        {
            while (1) //购买数量超过库存进入循环
            {
                printf("请输入购买数量（不得超过当前库存量）：\n");
                scanf("%d", &op);
                if(op <= 0)
                {
                    printf("请输入正确的购买数量！\n");
                    continue;
                }
                if (op > r.BookStoreMount)
                {
                    printf("库存不足！请调整购买数量！\n");
                    continue;
                }
                else {
                    if (op == r.BookStoreMount)
                        b[cnt].flag = 1;
                    b[cnt].BookID = r.BookID;
                    b[cnt].num = op;
                    b[cnt].price = r.price;
                    break;
                }
            }
            

            printf("成功加入购物车！\n");
            cnt++;


            op = addbook_Options(db);
           
            if (op == 2)
                continue;
            else
                break;
        }
        else
        {
            op = addbook_Options(db);

            if (op == 2)
                continue;
            else
                break;
        }
    }
    return 0;
}

//显示购物车
int show_shopbasket(sqlite3* db)
{
    int op = 0;
    int rc;
    sqlite3_stmt* stmt;
    const char* sql = nullptr;
    float sum = 0.0f;
    printf("------------------------------------------------\n");
    printf("书籍名称\t购买数量\t单价\t\n");
    for (int i = 0; i < cnt; i++)
    {
        sum += b[i].price * b[i].num;
        sql = "select * from tb_Storage natural join tb_Book where BookID =?";
        rc = sqlite3_prepare(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1,b[i].BookID);
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                printf("%-16s%-16d%-16.2f\n", sqlite3_column_text(stmt, 5), b[i].num, b[i].price);
            }
            sqlite3_finalize(stmt);
        }
    }
    printf("总价：%.2f\n", sum);
    printf("------------------------------------------------\n");

    shopbasket_Options();
    while (1)
    {
        printf("请输入选项：");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            //提交订单
            addintoOrder(db);
            break;
        case 2:
            //继续选书
            selectBook(db);
            break;
        case 3:
            //放弃购买
            return 1;
        default:
            printf("请输入正确选项！");
            continue;
        }
        break;
    }
    return 0;
}


//计入账单，改变库存
void addintoOrder(sqlite3* db)
{
    int rc = 0;
    sqlite3_stmt* stmt;
    const char* sql = nullptr;
    float sum = 0;

    for (int i = 0; i < cnt; i++)
    {
        sum += b[i].price * b[i].num;
    }
    if(cnt == 0)
    {
        printf("当前订单为空，不能下单！");
        return;
    }
//在tb_Order中添加记录
    //获取时间
    char Time[30];
    gettime(Time);
    sql = "insert into tb_Order (OrderTime,OrderType,OrderTotal) values(?,'+',?)";
    sqlite3_prepare(db,sql,-1,&stmt,NULL);
    sqlite3_bind_text(stmt, 1, Time,-1,0);
    sqlite3_bind_double(stmt,2, sum);
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE)
    {
        printf("SQL语句执行发生未知错误,结束本次操作\n");
        //取消上次操作？？？
        return;
    }
    sqlite3_finalize(stmt);
    printf("下单成功！\n");
    
    //改变库存
    for (int i = 0; i < cnt; i++)
        {
            //printf("b[i].flag=%d\t", b[i].flag);debug
            if (b[i].flag == 1)
            {
                sql = "delete from tb_Storage where BookID=?";
                rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                printf("rc=%d\n", rc);
                if (rc == SQLITE_OK)
                {
                    sqlite3_bind_int(stmt, 1, b[i].BookID);
                    rc = sqlite3_step(stmt);
                    printf("step() return %s\n", rc == SQLITE_DONE ? "SQLITE_DONE" : rc == SQLITE_ROW ? "SQLITE_ROW" : "SQLITE_ERROR");//DEBUG

                    rc = sqlite3_finalize(stmt);
                }
            }
            else
            sql = "update tb_Storage set BookStoreMount=BookStoreMount-? where BookID=?";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK)
            {
                sqlite3_bind_int(stmt, 1, b[i].num);
                sqlite3_bind_int(stmt, 2, b[i].BookID);
                rc = sqlite3_step(stmt);
                //printf("step() return %s\n", rc == SQLITE_DONE ? "SQLITE_DONE" : rc == SQLITE_ROW ? "SQLITE_ROW" : "SQLITE_ERROR");//DEBUG

                rc = sqlite3_finalize(stmt);
            }
        }

    cnt = 0;
    memset(b, 0, sizeof(b));
}

//购物车子页
void shopbasket_Options() {
    printf("下一步操作，请选择：\n");
    printf("1.提交订单\n");
    printf("2.继续选书\n");
    printf("3.放弃购买\n");
}

//添加书籍子页
int addbook_Options(sqlite3* db) {
    int op;
    while (1) //操作选项不正确进入循环
    {
        printf("请选择：\n");
        printf("1.提交订单\n");
        printf("2.继续选书\n");
        printf("3.查看购物车\n");

        printf("请输入选项：");
        scanf("%d", &op);
        switch (op) {
        case 1:
            //提交订单
            addintoOrder(db);
            return op;
        case 2:
            //继续选书
            return op;
        case 3:
            //查看购物车
            show_shopbasket(db);
            return op;
        default:
            printf("请输入正确选项！\n");
            continue;
        }
    }
}
