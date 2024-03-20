#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QSqlQuery>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);

private:
    /*******************数据库操作变量名***********************/
    QSqlDatabase db;

signals:

};

#endif // DATABASE_H
