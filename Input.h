//
//  Input.h
//  数据库
//
//  Created by Mac on 2020/5/9.
//  Copyright © 2020 Mac. All rights reserved.
//

#ifndef Input_h
#define Input_h

void GUIofInputOrder(sqlite3* db);
void GUIofInput(sqlite3* db);
int AddNewBooktoBookinfo(sqlite3* db,int Mount);
void viewInputOrder(sqlite3* db);//查询现有进货清单
void InputBook(sqlite3* db);//进货
void CommodityBack(sqlite3*db);//退货
void payment(sqlite3* db);//付款
void checkin(sqlite3* db);//到货确认，添加图书入库
void viewallOrder(sqlite3* db);//查看账单


#endif /* ___h */

