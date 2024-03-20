#include "database.h"

/***************************************************************************
类名:Database
头文件:"database.h"
功能:通过使用第三方ODBC实现MySQL数据库连接
调用:在需要使用数据库的类中包含"database.h"，实现MySQL连接
***************************************************************************/


Database::Database(QObject *parent) : QObject(parent)
{
    /*******************数据库链接***********************/
    this->db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("VM");
    db.setUserName("root");
    db.setPassword("123456");
    bool ok = db.open();
    if (ok){
        qDebug() << "database connect success";
    }
    else {
        qDebug()<<"error open database because"<<db.lastError().text();
        exit(1);
    }
}
