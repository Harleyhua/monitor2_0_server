#include "mysql_table.h"

#include <QSqlQuery>
#include "QsLog.h"
mysql_table::mysql_table(QObject *parent)
    : QObject{parent}
{

}

bool mysql_table::create_table(QSqlDatabase &m_database,QString &table_name, QString &field)
{
    QSqlQuery query = QSqlQuery(m_database);

    if(query.exec("CREATE TABLE IF NOT EXISTS " + table_name + field))
    {
        QLOG_INFO() << QString("创建数据表: %1 成功").arg(table_name);
    }
    else {
        QLOG_WARN() << QString("创建数据表: %1 失败").arg(table_name);
        return false;
    }

    return true;
}

bool mysql_table::delete_table(QSqlDatabase &m_database,QString &table_name)
{
    QString tmp_cmd = QString("DROP TABLE IF EXISTS %1").arg(table_name);
    QSqlQuery query(m_database);

    if(query.exec(tmp_cmd))
    {
        QLOG_INFO() << QString("删除数据表: %1 成功").arg(table_name);
    }
    else
    {
        QLOG_WARN() << QString("删除数据表: %1 失败").arg(table_name);
        return false;
    }

    return true;
}
