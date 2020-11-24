//
//  view.h
//  数据库
//
//  Created by Mac on 2020/5/10.
//  Copyright © 2020 Mac. All rights reserved.
//

#ifndef view_h
#define view_h


int Enter_Options(sqlite3* db);//入口界面选项
int Super_Man_Options(sqlite3* db);//超级管理员选项
int Common_Man_Options(sqlite3* db);//普通管理员选项
void update_userinfo_Options();//用户信息修改选项





#endif /* view_h */
