//
//  Book.h
//  数据库
//
//  Created by Mac on 2020/5/10.
//  Copyright © 2020 Mac. All rights reserved.
//

#ifndef Book_h
#define Book_h

//书籍查询
void print_Bookinfo(sqlite3* db, const char* sql, sqlite3_stmt* stmt);
void Book_search(sqlite3* db);

//图书信息修改
void updatefunc(sqlite3* db, sqlite3_stmt* stmt, const char* sql,int bookID,char * newinfo);
int updateBookinfo(sqlite3* db);
void update_bookinfo_Options();//书籍信息修改选项
void Book_search_Options();
#endif /* Book_h */
