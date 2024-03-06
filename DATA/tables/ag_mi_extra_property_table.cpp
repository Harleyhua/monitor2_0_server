#include "ag_mi_extra_property_table.h"

#include "QsLog.h"
#include "mysql_table.h"
#include <QDateTime>
#include <QSqlQuery>
#include <QVariant>
const QString ag_mi_extra_property_table::c_field_mi_cid = "mi_cid";
const QString ag_mi_extra_property_table::c_field_temporary_power = "temporary_power";
const QString ag_mi_extra_property_table::c_field_max_power = "max_power";
const QString ag_mi_extra_property_table::c_field_grid = "grid";
const QString ag_mi_extra_property_table::c_field_certification = "certification";
const QString ag_mi_extra_property_table::c_field_sys_time = "sys_time";


ag_mi_extra_property_table::ag_mi_extra_property_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_mi_extra_property_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field = QString(
                "(%1 VARCHAR(8) NOT NULL,"
                "%2 VARCHAR(200),"
                "%3 VARCHAR(200),"
                "%4 VARCHAR(200),"
                "%5 VARCHAR(200),"
                "%6 DATETIME NOT NULL,"
                "primary key(%7))")
                .arg(c_field_mi_cid,c_field_temporary_power,c_field_max_power,
                     c_field_grid,c_field_certification,c_field_sys_time,c_field_mi_cid);

    return mysql_table::create_table(m_database,m_name,tmp_field);
}


bool ag_mi_extra_property_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_mi_extra_property_table::w_temporary_power(QSqlDatabase &m_database, QString mi_cid, QString &temporary_power)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cmd = QString("INSERT INTO %1 (%2,%3,%4) values(?,?,?) ON DUPLICATE KEY UPDATE %5='%6',%7='%8'")
            .arg(m_name,c_field_mi_cid,c_field_temporary_power,c_field_sys_time,
                 c_field_temporary_power,temporary_power,
                 c_field_sys_time,time);

    QSqlQuery query(m_database);
    query.prepare(cmd);

    query.addBindValue(mi_cid);
    query.addBindValue(temporary_power);
    query.addBindValue(time);

    if(query.exec())
    {

    }
    else
    {
        //QLOG_WARN() << QString("写入 微逆额外属性表 临时功率 失败");
    }
}

void ag_mi_extra_property_table::r_temporary_power(QSqlDatabase &m_database, QString mi_cid, QString &temporary_power)
{
    QString tmp_cmd = QString("SELECT %1 FROM %2 WHERE %3='%4' ")
            .arg(c_field_temporary_power,m_name,c_field_mi_cid,mi_cid);
    QSqlQuery query(m_database);

    query.prepare(tmp_cmd);
    if(query.exec())
    {
        if(query.size() == 1)
        {
            query.next();
            temporary_power = query.value(c_field_temporary_power).toString();
            return ;
        }

    }

}

void ag_mi_extra_property_table::w_max_power(QSqlDatabase &m_database, QString mi_cid, QString &max_power)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cmd = QString("INSERT INTO %1 (%2,%3,%4) values(?,?,?) ON DUPLICATE KEY UPDATE %5='%6',%7='%8'")
            .arg(m_name,c_field_mi_cid,c_field_max_power,c_field_sys_time,
                 c_field_max_power,max_power,c_field_sys_time,time);

    QSqlQuery query(m_database);
    query.prepare(cmd);

    query.addBindValue(mi_cid);
    query.addBindValue(max_power);
    query.addBindValue(time);

    if(query.exec())
    {

    }
    else
    {
        //QLOG_WARN() << QString("写入 微逆额外属性表 最大 失败");
    }

}

void ag_mi_extra_property_table::r_max_power(QSqlDatabase &m_database, QString mi_cid, QString &max_power)
{
    QString tmp_cmd = QString("SELECT %1 FROM %2 WHERE %3='%4' ")
            .arg(c_field_max_power,m_name,c_field_mi_cid,mi_cid);
    QSqlQuery query(m_database);

    query.prepare(tmp_cmd);
    if(query.exec())
    {
        if(query.size() == 1)
        {
            query.next();
            max_power = query.value(c_field_max_power).toString();
            return ;
        }
    }

}

void ag_mi_extra_property_table::w_grid(QSqlDatabase &m_database, QString mi_cid, QString &grid)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cmd = QString("INSERT INTO %1 (%2,%3,%4) values(?,?,?) ON DUPLICATE KEY UPDATE %5='%6',%7='%8'")
            .arg(m_name,c_field_mi_cid,c_field_grid,c_field_sys_time,
                 c_field_grid,grid,c_field_sys_time,time);

    QSqlQuery query(m_database);
    query.prepare(cmd);

    query.addBindValue(mi_cid);
    query.addBindValue(grid);
    query.addBindValue(time);

    if(query.exec())
    {

    }
    else
    {
        //QLOG_WARN() << QString("写入 微逆额外属性表 并网参数 失败");
    }
}

void ag_mi_extra_property_table::r_grid(QSqlDatabase &m_database, QString mi_cid, QString &grid)
{
    QString tmp_cmd = QString("SELECT %1 FROM %2 WHERE %3='%4' ")
            .arg(c_field_grid,m_name,c_field_mi_cid,mi_cid);
    QSqlQuery query(m_database);

    query.prepare(tmp_cmd);
    if(query.exec())
    {
        if(query.size() == 1)
        {
            query.next();
            grid = query.value(c_field_grid).toString();
            return ;
        }
    }
}

void ag_mi_extra_property_table::w_certification(QSqlDatabase &m_database, QString mi_cid, QString &certification)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cmd = QString("INSERT INTO %1 (%2,%3,%4) values(?,?,?) ON DUPLICATE KEY UPDATE %5='%6',%7='%8'")
            .arg(m_name,c_field_mi_cid,c_field_certification,c_field_sys_time,
                 c_field_certification,certification,c_field_sys_time,time);

    QSqlQuery query(m_database);
    query.prepare(cmd);

    query.addBindValue(mi_cid);
    query.addBindValue(certification);
    query.addBindValue(time);

    if(query.exec())
    {

    }
    else
    {
        //QLOG_WARN() << QString("写入 微逆额外属性表 并网参数 失败");
    }
}

void ag_mi_extra_property_table::r_certification(QSqlDatabase &m_database, QString mi_cid, QString &certification)
{
    QString tmp_cmd = QString("SELECT %1 FROM %2 WHERE %3='%4' ")
            .arg(c_field_certification,m_name,c_field_mi_cid,mi_cid);
    QSqlQuery query(m_database);

    query.prepare(tmp_cmd);
    if(query.exec())
    {
        if(query.size() == 1)
        {
            query.next();
            certification = query.value(c_field_certification).toString();
            return ;
        }
    }
}



