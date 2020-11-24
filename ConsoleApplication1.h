//
//  ConsoleApplication1.h
//  数据库
//
//  Created by Mac on 2020/5/10.
//  Copyright © 2020 Mac. All rights reserved.
//

#ifndef ConsoleApplication1_h
#define ConsoleApplication1_h

typedef struct checkreturn {
    int ID = 0;
    int flag = 0;
}cr;

cr Man_Login(sqlite3* db);//登录
void Guidein(sqlite3* db);//创建账户
void viewManinfo(sqlite3* db, int flag);//查看用户资料
int updateManinfo(sqlite3* db, int flag);//修改用户信息





#endif /* ConsoleApplication1_h */
