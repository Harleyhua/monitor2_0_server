#ifndef SQLCONNECTPOOL_H
#define SQLCONNECTPOOL_H

#include <QtSql>
#include <QQueue>
#include <QString>
#include <QMutex>
#include <QMutexLocker>

/*
    数据库连接池
    由于QT框架的线程和sockt 归属问题 搁置

    //目前真正运行的模式是 收-发  ->  创建-关闭
*/


typedef struct
{
    QString hostname;
    uint16_t port;
    QString username;
    QString password;
    QString database_name;
}mysql_login_stc;  //数据库登录参数


class sqlconnectpool
{
public:
    static void release(); // 关闭所有的数据库连接
    static QSqlDatabase openConnection();                 // 获取数据库连接
    static void closeConnection(QSqlDatabase connection); // 释放数据库连接回连接池

    static QSqlDatabase openConnection_clh(QString name);
    static void closeConnection_clh(QSqlDatabase db,QString name); // 释放数据库连接回连接池
    ~sqlconnectpool();

private:
    static sqlconnectpool& getInstance();

    sqlconnectpool();
    sqlconnectpool(const sqlconnectpool &other);
    sqlconnectpool& operator=(const sqlconnectpool &other);
    QSqlDatabase createConnection(const QString &connectionName); // 创建数据库连接

    QQueue<QString> usedConnectionNames;   // 已使用的数据库连接名
    QQueue<QString> unusedConnectionNames; // 未使用的数据库连接名


    // 数据库信息
//    QString hostName;
//    QString databaseName;
//    QString username;
//    QString password;
//    QString databaseType;

    bool    testOnBorrow;    // 取得连接的时候验证连接是否有效
    QString testOnBorrowSql; // 测试访问数据库的 SQL

    int maxWaitTime;  // 获取连接最大等待时间
    int waitInterval; // 尝试获取连接时等待间隔时间
    int maxConnectionCount; // 最大连接数

    static QMutex mutex;
    static QWaitCondition waitConnection;
    static sqlconnectpool *instance;
};

#endif // SQLCONNECTPOOL_H
