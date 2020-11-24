//
//  Shop.h
//  数据库
//
//  Created by Mac on 2020/5/10.
//  Copyright © 2020 Mac. All rights reserved.
//

#ifndef Shop_h
#define Shop_h
typedef struct searchreturn {
    int BookID = 0;
    int BookStoreMount = 0;
    float price = 0.0f;
}sr;

typedef struct basket {
    int BookID = 0;
    int num = 0;
    float price = 0.0f;
    int flag = 0;
}bk; //临时购物车

int selectBook(sqlite3* db);//选购书籍
int show_shopbasket(sqlite3* db);//查看购物车
void addintoOrder(sqlite3* db);//计入账单，改变库存
void shopbasket_Options();//购物车子页
int addbook_Options(sqlite3* db);//书籍查询方式选项

#endif /* Shop_h */
