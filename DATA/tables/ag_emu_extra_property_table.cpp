#include "ag_emu_extra_property_table.h"

#include "mysql_table.h"
#include <QDateTime>
#include <QSqlQuery>
#include <QVariant>
const QString ag_emu_extra_property_table::c_field_emu_cid = "emu_cid";
const QString ag_emu_extra_property_table::c_field_func = "countercurrent";
const QString ag_emu_extra_property_table::c_field_sys_time = "sys_time";

ag_emu_extra_property_table::ag_emu_extra_property_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_emu_extra_property_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field = QString(
                "(%1 VARCHAR(8) NOT NULL,"
                "%2 VARCHAR(200),"
                "%3 DATETIME NOT NULL,"
                "primary key(%4))")
                .arg(c_field_emu_cid,c_field_func,c_field_sys_time,c_field_emu_cid);

    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_emu_extra_property_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_emu_extra_property_table::w_countercurrent(QSqlDatabase &m_database, QString emu_cid, QString countercurrent)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cmd = QString("INSERT INTO %1 (%2,%3,%4) values(?,?,?) ON DUPLICATE KEY UPDATE %5='%6',%7='%8'")
            .arg(m_name,c_field_emu_cid,c_field_func,c_field_sys_time,
                 c_field_func,countercurrent,c_field_sys_time,time);

    QSqlQuery query(m_database);
    query.prepare(cmd);

    query.addBindValue(emu_cid);
    query.addBindValue(countercurrent);
    query.addBindValue(time);

    if(query.exec())
    {

    }
    else
    {
        //QLOG_WARN() << QString("写入 微逆额外属性表 临时功率 失败");
    }
}

bool ag_emu_extra_property_table::r_countercurrent(QSqlDatabase &m_database, QString emu_cid, QString &countercurrent)
{
    QString tmp_cmd = QString("SELECT %1 FROM %2 WHERE %3='%4' ")
            .arg(c_field_func,m_name,c_field_emu_cid,emu_cid);
    QSqlQuery query(m_database);

    query.prepare(tmp_cmd);
    if(query.exec())
    {
        if(query.size() == 1)
        {
            query.next();
            countercurrent = query.value(c_field_func).toString();
            return true;
        }
    }

    return false;
}
