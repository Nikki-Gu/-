#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "sqlite3.h"
#include "md5.h"
#include "ConsoleApplication1.h"
#include "Basic function.h"
#include "view.h"
//#include <windows.h>
using namespace std;
cr c;

//登陆
cr Man_Login(sqlite3* db) {
    char a[25] = { 0 };
    string pw;
    printf("请输入用户名：");
    scanf("%s", a);
    printf("请输入密码：");
    cin >> pw;
    pw = MD5(pw).toStr();//md5加密后的密码
    const char* sql = nullptr;
    int rc;
    sqlite3_stmt* stmt;
    int f = 0;

    sql = "select * from tb_Admin where AdminName=?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        rc = sqlite3_bind_text(stmt, 1, a, -1, 0);
        rc = sqlite3_step(stmt);
        while (rc == SQLITE_ROW) {
            f = 1;
            const unsigned char* pTmp;
            pTmp = sqlite3_column_text(stmt, 2);
            if (strcmp((const char*)(char*)pTmp, pw.data()) == 0)
            {
                printf("登录成功！\n");
                c.ID = sqlite3_column_int(stmt, 0);
                if (sqlite3_column_int(stmt, 6) == 1)
                    c.flag = 2;
                else
                    c.flag = 1;
                break;
            }
            else
            {
                printf("密码错误，登录失败\n");
                break;
            }
        }
        rc = sqlite3_finalize(stmt);
        if (f == 0)
        {
            printf("不存在该用户\n");
        }

    }

    return c;

    //flag为1为普通管理员（表中flag为0），为2为超级管理员（表中flag为1），为0登录失败
}

//创建账户
void Guidein(sqlite3* db) {
    int rc;
    const char* sql = nullptr;
    sqlite3_stmt* stmt;
    char nm[25],tnm[25],sx[25];
    string pw ;
    int age = 0;
    printf("请设置用户名：");
    scanf("%s", nm);
    printf("请设置密码：");
    cin >> pw;
    //md5加密后的密码
    pw = MD5(pw).toStr();
    printf("请输入员工姓名：");
    scanf("%s", tnm);
    printf("请输入员工性别(取男/女)：");
    scanf("%s", sx);
    printf("请输入员工年龄:");
    scanf("%d", &age);
    sql = "insert into tb_Admin (AdminName,AdminPwd,AdminTruename,AdminSex,AdminAge,AdminFlag)"\
        " values (?,?,?,?,?,0)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        rc = sqlite3_bind_text(stmt, 1, nm, -1, 0);
        rc = sqlite3_bind_text(stmt, 2, pw.data(), -1, 0);
        rc = sqlite3_bind_text(stmt, 3, tnm, -1, 0);
        rc = sqlite3_bind_text(stmt, 4, sx, -1, 0);
        rc = sqlite3_bind_int(stmt, 5, age);
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE)
        {
            printf("成功创建新账户！\n");
        }
        sqlite3_finalize(stmt);
    }
}

//查看用户资料
void viewManinfo(sqlite3* db, int flag)
{
    char* zErrMsg = 0;
    const char* sql;
    int rc;
    sqlite3_stmt* stmt;
    if (flag == 2)
    {
        sql = "select AdminID,AdminName,AdminTruename,AdminSex,AdminAge"\
            " from tb_Admin";
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
        /*if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        else {
            fprintf(stdout, "search being done successfully\n");
        }*/
    }
    else if (flag == 1)
    {
        sql = "select AdminID,AdminName,AdminTruename,AdminSex,AdminAge"\
            " from tb_Admin where AdminID=?";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK)
        {
            rc = sqlite3_bind_int(stmt, 1, c.ID);
            rc = sqlite3_step(stmt);
            while (rc == SQLITE_ROW) {
                const unsigned char* pTmp;
                int nCol = 0, num;
                num = sqlite3_column_int(stmt, nCol++);
                printf("AdminID: %d\n", num);
                pTmp = sqlite3_column_text(stmt, nCol++);
                printf("AdminName: %s\n", pTmp);
                pTmp = sqlite3_column_text(stmt, nCol++);
                printf("AdminTruename: %s\n", pTmp);
                pTmp = sqlite3_column_text(stmt, nCol++);
                printf("AdminSex: %s\n", pTmp);
                num = sqlite3_column_int(stmt, nCol++);
                printf("AdminAge: %d\n", num);
                printf("---------------------------------\n");
                rc = sqlite3_finalize(stmt);
            }
        }
    }
}


//修改用户信息,超级管理员修改所有用户信息，普通管理员只能修改自己
int updateManinfo(sqlite3* db, int flag)
{
    const char* sql = nullptr;
    int rc;
    int op;
    char newinfo[25] = {};
    int newage;
    int user_ID = 0;
    sqlite3_stmt* stmt;

    if (flag == 2)
    {
        printf("请输入被修改用户ID：");
        scanf("%d", &user_ID);
    }
    else if (flag == 1)
    {
        user_ID = c.ID;
    }

    while (1)
    {
        update_userinfo_Options();
        printf("请输入选项：");
        scanf("%d", &op);

        switch (op) {
        case 1:
            sql = "update tb_Admin set AdminName=? where AdminID=?";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            /*printf("rc=%d\n", rc);*/
            if (rc == SQLITE_OK)
            {
                sqlite3_bind_int(stmt, 2, user_ID);
                printf("请输入新的用户名：");
                scanf("%s", newinfo);
                sqlite3_bind_text(stmt, 1, newinfo, -1, 0);
                rc = sqlite3_step(stmt);
                //printf("step() return %s\n", rc == SQLITE_DONE ? "SQLITE_DONE" : rc == SQLITE_ROW ? "SQLITE_ROW" : "SQLITE_ERROR");//DEBUG
                rc = sqlite3_finalize(stmt);
            }
            break;
        case 2:
            sql = "update tb_Admin set AdminPwd=? where AdminID=?";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            /*printf("rc=%d\n", rc);*/
            if (rc == SQLITE_OK)
            {
                string newpwd;
                printf("请输入新的密码：");
                cin >> newpwd;
                newpwd = MD5(newpwd).toStr();
                sqlite3_bind_text(stmt, 1, newpwd.data(), -1, 0);
                sqlite3_bind_int(stmt, 2, user_ID);
                rc = sqlite3_step(stmt);
                rc = sqlite3_finalize(stmt);
            }
            break;
        case 3:
            sql = "update tb_Admin set AdminTruename=? where AdminID=?";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            /*printf("rc=%d\n", rc);*/
            if (rc == SQLITE_OK)
            {
                sqlite3_bind_int(stmt, 2, user_ID);
                printf("请输入新的姓名：");
                scanf("%s", newinfo);
                sqlite3_bind_text(stmt, 1, newinfo, -1, 0);
                rc = sqlite3_step(stmt);
                //printf("step() return %s\n", rc == SQLITE_DONE ? "SQLITE_DONE" : rc == SQLITE_ROW ? "SQLITE_ROW" : "SQLITE_ERROR");//DEBUG
                rc = sqlite3_finalize(stmt);
            }
            break;
        case 4:
            sql = "update tb_Admin set AdminSex=? where AdminID=?";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            /*printf("rc=%d\n", rc);*/
            if (rc == SQLITE_OK)
            {
                sqlite3_bind_int(stmt, 2, user_ID);
                printf("请输入新的性别：");
                scanf("%s", newinfo);
                sqlite3_bind_text(stmt, 1, newinfo, -1, 0);
                rc = sqlite3_step(stmt);
                //printf("step() return %s\n", rc == SQLITE_DONE ? "SQLITE_DONE" : rc == SQLITE_ROW ? "SQLITE_ROW" : "SQLITE_ERROR");//DEBUG
                rc = sqlite3_finalize(stmt);
            }
            break;
        case 5:
            sql = "update tb_Admin set AdminAge=? where AdminID=?";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            /*printf("rc=%d\n", rc);*/
            if (rc == SQLITE_OK)
            {
                sqlite3_bind_int(stmt, 2, user_ID);
                printf("请输入新的年龄：");
                scanf("%d", &newage);
                sqlite3_bind_int(stmt, 1, newage);
                rc = sqlite3_step(stmt);
                //printf("step() return %s\n", rc == SQLITE_DONE ? "SQLITE_DONE" : rc == SQLITE_ROW ? "SQLITE_ROW" : "SQLITE_ERROR");//DEBUG
                rc = sqlite3_finalize(stmt);
            }
            break;
        case 6:
            return 0;
        default:
            printf("请输入正确选项！");
            continue;
        }
    }

}
