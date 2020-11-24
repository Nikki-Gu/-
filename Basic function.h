//
//  Basic function.h
//  数据库
//
//  Created by Mac on 2020/5/9.
//  Copyright © 2020 Mac. All rights reserved.
//

#ifndef Basic_function_h
#define Basic_function_h

int callback(void *NotUsed, int argc, char **argv, char **azColName);//回调函数
int printInputOrder(void *NotUsed, int argc, char **argv, char **azColName);//回调函数专门用于打印进货清单
int printOrder(void *NotUsed, int argc, char **argv, char **azColName);//回调函数专门用于打印账单
void gettime(char Time[]);//获取时间

#endif /* Basic_function_h */
