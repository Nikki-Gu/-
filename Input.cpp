//
//  进货.cpp
//  数据库
//
//  Created by Mac on 2020/5/9.
//  Copyright © 2020 Mac. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#import <sqlite3.h>
#include "Input.h"
#include "Basic function.h"
using namespace std;


void GUIofInputOrder(sqlite3* db)//图书进货管理二级页面
{
    viewInputOrder(db);
    printf("目前的进货清单如上所示:\n");
    int num;
    while(1)
    {
        printf("---------------------------------\n");
        printf("1.付款\t2.退货\t3.确认到货\t4.再次查看进货清单\n5.返回上层页面\n");
        printf("请输入操作编号：");
        cin >> num;
        switch(num)
        {
            case 1: payment(db); break;
            case 2: CommodityBack(db);break;
            case 3: checkin(db);break;
            case 4:
            {
                viewInputOrder(db);
                printf("目前的进货清单如上所示:\n");
                break;
            }
            case 5: return;
            default: printf("请输入正确的操作编号\n");
        }
    }
}

void GUIofInput(sqlite3* db)//图书进货管理一级页面
{
    int num;
    while(1)
    {
        printf("---------------------------------\n");
        printf("欢迎进入图书进货管理功能:\n");
        printf("1.添加图书进货\n2.进货清单页面（可以进行查询，付款，退货，确认到货)\n3.返回上层页面\n");
        printf("请输入操作编号：");
        cin >> num;
        switch(num)
        {
            case 1: InputBook(db);break;
            case 2: GUIofInputOrder(db); break;
            case 3: return;
            default: printf("请输入正确的操作编号\n");
        }
    }
}



//进货
void InputBook(sqlite3* db)
{
    int row = 0,rc = 0; //row 用于获取元组数目，需要及时更新置为0， rc用于判断sql语句执行情况
    float TotalInputPrice = 0,BookInPrice = 0,Mount = 0;;//进货总价,进货单价，进货数量
    char *sql = nullptr;//存储sql语句
    sqlite3_stmt *stmt;
    char *Bookisbn = (char*)malloc(sizeof(char)*30); //存储图书的isbn号
    char *s[5];//新书信息存储
    for(int i = 0; i < 5; i++)
        s[i] = (char*)malloc(sizeof(char)*20) ;
    printf("请输入需要进货图书的Bookisbn和进货数量：");//测试数据isbn 9787506365437/9787108063106
    cin >> Bookisbn >> Mount;
//执行sql语句判断系统中是否有该图书信息，如果有，获取其进货单价
    sql = "select BookInPrice from tb_Book where Bookisbn = ?";
    sqlite3_prepare(db,sql,-1,&stmt,NULL);
    sqlite3_bind_text(stmt, 1, Bookisbn, -1, 0);
    rc = sqlite3_step(stmt);
    while(rc == SQLITE_ROW)
    {
        BookInPrice = sqlite3_column_double(stmt,0);
        row++;
        rc = sqlite3_step(stmt);
    }
    if(rc != 101 && rc != 0)//未完成执行 101-finish successfully, 0-SQLITE_OK
    {
        printf("SQL语句执行发生未知错误,结束本次操作\n");
        return;
    }
    else if(row != 0 && row != 1)
    {
        printf("Bookisbn号存在不唯一情况，数据库约束发生错误, 结束本次操作！\n");//本次SQL语句执行未产生需要输出的结果
        return;
    }
    rc = sqlite3_finalize(stmt);
    
//有该图书信息，计算出进货总价
    if(row == 1)
    {
        printf("系统中存在该图书信息,");
        TotalInputPrice = Mount * BookInPrice;
    }
//系统中不存在该图书信息，手动输入该图书信息
    else
    {
        printf("系统中不存在该图书信息，请手动输入该图书信息：\n");
        printf("图书isbn号（唯一）：");    cin >> Bookisbn;    cin.sync();
        printf("图书名称：");    cin >> s[0];        cin.clear();cin.sync();
        printf("出版社：");   cin >> s[1];        cin.sync();
        printf("出版日期："); cin >> s[2];        cin.sync();
        printf("作者：");  cin >> s[3];        cin.sync();
        printf("进货价格：");    cin >> BookInPrice; cin.sync();
        TotalInputPrice = Mount * BookInPrice;
        //添加图书信息到系统图书信息tb_Book中
        sql = "insert into tb_Book (Bookisbn,BookName,BookPress,BookPubDate,BookAuthor,BookInPrice)" \
              "values(?,?,?,?,?,?)";
        sqlite3_prepare(db,sql,-1,&stmt,NULL);
        sqlite3_bind_text(stmt, 1, Bookisbn, -1, 0);
        sqlite3_bind_text(stmt, 2, s[0], -1, 0);
        sqlite3_bind_text(stmt, 3, s[1], -1, 0);
        sqlite3_bind_text(stmt, 4, s[2], -1, 0);
        sqlite3_bind_text(stmt, 5, s[3], -1, 0);
        sqlite3_bind_double(stmt, 6, BookInPrice);
        rc = sqlite3_step(stmt);
        if(rc != 101 && rc != 0)
        {
            printf("SQL语句执行发生未知错误,请检查输入信息是否满足约束条件,结束本次操作\n");
            return;
        }
        sqlite3_finalize(stmt);
        printf("已将该图书信息收录到系统中\n");
    }
    
//将需要进货书籍加入进货订单tb_InOrder中
    sql ="insert into tb_InOrder (InputSum) values(?);";
    sqlite3_prepare(db,sql,-1,&stmt,NULL);
    sqlite3_bind_double(stmt,1, TotalInputPrice);
    rc = sqlite3_step(stmt);
    if(rc != 101 && rc != 0)
    {
        printf("SQL语句执行发生未知错误,结束本次操作\n");
        return;
    }
    sqlite3_finalize(stmt);
    
//将需要进货书籍加入进货购物车tb_Inputbasket中
    sql = "insert into tb_Inputbasket (InputID,BookID,InputPrice,InputMount)"\
          "select max(InputID),BookID,BookInPrice,? from tb_InOrder,tb_Book "\
          "where Bookisbn = ?";
    sqlite3_prepare(db,sql,-1,&stmt,NULL);
    sqlite3_bind_int(stmt, 1, (int)Mount);
    sqlite3_bind_text(stmt,2, Bookisbn, -1, 0);
    rc = sqlite3_step(stmt);
    if(rc != 101 && rc != 0)
    {
        printf("SQL语句执行发生未知错误,结束本次操作\n");
        return;
    }
    sqlite3_finalize(stmt);
    printf("已将该图书添加到进货清单，请前往进货清单查看\n\n");
}
    

//对未付款订单进行退货ok
void CommodityBack(sqlite3* db)
{
    char *sql = nullptr;
    sqlite3_stmt *stmt;
    int rc = 0 ,InputID = 0,row = 0;
    printf("---------------------------------\n");
    printf("请输入要进行退货的订单号：");
    cin >> InputID;
//检查是否是未付款状态
    sql = "select InputID from tb_InOrder where (InputID =? and InputState = '未付款')";
    sqlite3_prepare(db,sql,-1,&stmt,NULL);
    sqlite3_bind_int(stmt, 1,InputID);
    rc = sqlite3_step(stmt);
    while(rc == SQLITE_ROW)
    {
        row++;
        rc = sqlite3_step(stmt);
    }
    if(rc != 101 && rc != 0)//未完成执行
    {
        printf("SQL语句执行发生未知错误,结束本次操作\n");
        return;
    }
    else if(row != 0 && row != 1)
    {
        printf("InputID号存在不唯一情况，数据库约束发生错误, 结束本次操作！\n");//本次SQL语句执行未产生需要输出的结果
        return;
    }
    rc = sqlite3_finalize(stmt);
    if(row == 0)
    {
        printf("该订单不存在或该订单不能退货（已付款/已到货）\n");
        return;
    }
    char choice;
    printf("你确定退货吗？[y/n]");
    cin >> choice;
    if(choice == 'n')
    {
        printf("退货已取消.\n");
        return;
    }
//update tb_InOrder的订单状态为已退货;
   sql ="update tb_InOrder set InputState = '已退货' where InputID = ?";
   sqlite3_prepare(db,sql,-1,&stmt,NULL);
   sqlite3_bind_int(stmt, 1, InputID);
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE)
    {
        printf("SQL语句执行发生未知错误,结束本次操作,退货失败\n");
        return;
    }
    sqlite3_finalize(stmt);
   printf("已退货，可以前往进货清单核查\n");
}

    
//对未付款订单进行付款ok
void payment(sqlite3* db)
{
//检查是否是未付款状态
    char *sql;
    char choice;
    char Time[30];
    sqlite3_stmt *stmt;
    int rc,InputID,row = 0;
    float TotalPrice = 0;
    printf("---------------------------------\n");
    printf("请输入要进行付款的订单号：");
    cin >> InputID;
    sql = "select InputSum from tb_InOrder where InputState = '未付款' and InputID = ?";
    rc = sqlite3_prepare(db,sql,-1,&stmt,NULL);
    sqlite3_prepare(db,sql,-1,&stmt,NULL);
    sqlite3_bind_int(stmt, 1,InputID);
    rc = sqlite3_step(stmt);
    while(rc == SQLITE_ROW)
    {
        TotalPrice = sqlite3_column_double(stmt, 0);
        row++;
        rc = sqlite3_step(stmt);
    }
    if(rc != 101 && rc != 0)//未完成执行
    {
        printf("SQL语句执行发生未知错误,结束本次操作\n");
        return;
    }
    else if(row != 0 && row != 1)
    {
        printf("InputID号存在不唯一情况，数据库约束发生错误, 结束本次操作！\n");//本次SQL语句执行未产生需要输出的结果
        return;
    }
    rc = sqlite3_finalize(stmt);
    if(row == 0)
    {
        printf("该订单不存在或该订单不用进行付款（已退货/已付款/已到货）\n");
        return;
    }
    
//显示价格
    printf("该笔订单总价为 %.2f:\n",TotalPrice);
    printf("你确定现在付款吗？[y/n]");
    cin >> choice;
    if(choice == 'n')
    {
        printf("付款已取消\n");
        return;
    }

//updatetb_InOrder的订单状态为已付款
    sql ="update tb_InOrder set InputState = '已付款' where InputID = ?";
    sqlite3_prepare(db,sql,-1,&stmt,NULL);
    sqlite3_bind_int(stmt, 1, InputID);
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE)
    {
        printf("SQL语句执行发生未知错误,结束本次操作,付款失败\n");
        return;
    }
    sqlite3_finalize(stmt);
    printf("已付款，可以前往进货清单核查\n");
    
//获取时间
    gettime(Time);
//insert in tb_Order添加账单
    sql = "insert into tb_Order (OrderTime,OrderType,OrderTotal) values(?,'-',?)";
    sqlite3_prepare(db,sql,-1,&stmt,NULL);
    sqlite3_bind_text(stmt, 1, Time,-1,0);
    sqlite3_bind_double(stmt,2, TotalPrice);
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE)
    {
        printf("SQL语句执行发生未知错误,结束本次操作\n");
        return;
    }
    sqlite3_finalize(stmt);
    printf("账单已更新\n");
}



//到货确认，添加图书入库
void checkin(sqlite3* db)
{
    char *sql;
    char choice;
    sqlite3_stmt *stmt;
    int rc,InputID,OriginalStoreMount = 0,ShopMount = 0,BookID = 0,row = 0;
    float BookMPrice;
    printf("---------------------------------\n");
    printf("请输入要进行确认收货的订单号：");
    cin >> InputID;

//检查是否是已付款状态
    sql = "select * from tb_InOrder where (InputID =? and InputState = '已付款')";
    sqlite3_prepare(db,sql,-1,&stmt,NULL);
    sqlite3_bind_int(stmt, 1,InputID);
    rc = sqlite3_step(stmt);
    while(rc == SQLITE_ROW)
    {
        row++;
        rc = sqlite3_step(stmt);
    }
    if(rc != 101 && rc != 0)//未完成执行
    {
        printf("SQL语句执行发生未知错误,结束本次操作\n");
        return;
    }
    else if(row != 0 && row != 1)
    {
        printf("InputID号存在不唯一情况，数据库约束发生错误, 结束本次操作！\n");//本次SQL语句执行未产生需要输出的结果
        return;
    }
    rc = sqlite3_finalize(stmt);
    if(row == 0)
    {
        printf("该订单不存在或该订单不用进行确认收货操作（已退货/已到货）\n");
        return;
    }
    
    printf("确认图书已经到货吗？[y/n]");
    cin >> choice;
    if(choice == 'n')
    {
        printf("退出确认到货");
        return;
    }


/*检查库存里面是否有这本书，有就更新BookStoreMount,没有就插入insert in tb_Storage*/
    //获取BookID 和 InputMount
    sql = "select BookID,InputMount from tb_Book natural join tb_Inputbasket where InputID = ? ";
    rc = sqlite3_prepare(db,sql,-1,&stmt,NULL);
    rc = sqlite3_bind_int(stmt, 1, InputID);
    rc = sqlite3_step(stmt);
    row = 0;
    while(rc == SQLITE_ROW)
    {
        BookID = sqlite3_column_int(stmt, 0);
        ShopMount = sqlite3_column_int(stmt, 1);
        rc = sqlite3_step(stmt);
        row++;
    }
    if(rc != 101 && rc != 0)//未完成执行
    {
        printf("SQL语句执行发生未知错误,结束本次操作\n");
        return;
    }
    if(row == 0)
    {
        printf("something wrong!");
        return;
    }
    rc=sqlite3_finalize(stmt);
    
    
    sql = "select BookStoreMount from tb_Storage where BookID = ?";
    rc = sqlite3_prepare(db,sql,-1,&stmt,NULL);
    rc = sqlite3_bind_int(stmt, 1, BookID);
    row = 0;
    rc = sqlite3_step(stmt);
    while(rc == SQLITE_ROW)
    {
        OriginalStoreMount = sqlite3_column_int(stmt, 0);
        rc = sqlite3_step(stmt);
        row++;
    }
    if(rc != 101 && rc != 0)//未完成执行
    {
        printf("SQL语句执行发生未知错误,结束本次操作\n");
        return;
    }
    sqlite3_finalize(stmt);

    if(row == 1)//有就更新tb_Storage中的BookStoreMount
    {
        sql = "update tb_Storage set BookStoreMount = ? where BookID = ?";
        sqlite3_prepare(db,sql,-1,&stmt,NULL);
        sqlite3_bind_int(stmt, 1, OriginalStoreMount+ShopMount);
        sqlite3_bind_int(stmt,2, BookID);
        rc = sqlite3_step(stmt);
        if(rc != 101 && rc != 0)//未完成执行
        {
            printf("SQL语句执行发生未知错误,结束本次操作\n");
            return;
        }
        sqlite3_finalize(stmt);
    }
    else //没有就插入insert in tb_Storage
    {
        //获取时间
        char Time[30];
        gettime(Time);
        //获取售价
        printf("请输入该图书售价：");
        cin >> BookMPrice;
        sql = "insert into tb_Storage (BookID,BookStoreMount,BookStoreTime,BookMPrice) values(?,?,?,?)";
        sqlite3_prepare(db,sql,-1,&stmt,NULL);
        rc = sqlite3_bind_int(stmt, 1, BookID);
        rc = sqlite3_bind_int(stmt,2, ShopMount);
        rc = sqlite3_bind_text(stmt,3, Time, -1, 0);
        rc = sqlite3_bind_double(stmt,4, BookMPrice);
        rc = sqlite3_step(stmt);
        if(rc != 101 && rc != 0)//未完成执行
        {
            printf("SQL语句执行发生未知错误,结束本次操作\n");
            return;
        }
        sqlite3_finalize(stmt);
    }
//updatetb_InOrder的订单状态为订单完成
    sql ="update tb_InOrder set InputState = '订单完成' where InputID = ?";
    sqlite3_prepare(db,sql,-1,&stmt,NULL);
    sqlite3_bind_int(stmt, 1, InputID);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if(rc != 101 && rc != 0)//未完成执行
    {
        printf("SQL语句执行发生未知错误,结束本次操作\n");
        return;
    }
    printf("订单已完成，可以前往进货清单核查\n");
    printf("已将该图书添加入库，可以前往库存查询该图书信息\n");
}

//查询进货清单
void viewInputOrder(sqlite3* db)
{
    printf("---------------------------------\n");
    char *zErrMsg = 0;
    char *sql;
    int rc;
    sql ="select tb_InOrder.InputID,tb_Book.BookID,BookName,InputPrice,InputMount,InputSum,InputState from tb_InOrder join tb_Inputbasket join tb_Book where tb_InOrder.InputID = tb_Inputbasket.InputID and tb_Inputbasket.BookID = tb_Book.BookID";
    //Bookisbn,BookName
    rc = sqlite3_exec(db, sql, printInputOrder, 0, &zErrMsg);
//    if( rc != SQLITE_OK ){
//    fprintf(stderr, "SQL error: %s\n", zErrMsg);
//       sqlite3_free(zErrMsg);
//    }else{
//       fprintf(stdout, "search being done successfully\n");//之后可以删掉
//    }
}

//查看账单ok
void viewallOrder(sqlite3* db)
{
    printf("---------------------------------\n");
    char *zErrMsg = 0;
    char *sql;
    int rc;
    sql ="select * from tb_Order";
    rc = sqlite3_exec(db, sql, printOrder, 0, &zErrMsg);
//    if( rc != SQLITE_OK ){
//    fprintf(stderr, "SQL error: %s\n", zErrMsg);
//       sqlite3_free(zErrMsg);
//    }else{
//       fprintf(stdout, "search being done successfully\n");//之后可以删掉
//    }
    printf("以上为目前账单内容\n");
    printf("---------------------------------\n");
}




