#include "ag_emu_status_table.h"

#include "mysql_table.h"
#include <QVariant>
#include <QSqlQuery>
#include <QDateTime>
const QString ag_emu_status_table::c_field_emu_cid = "emu_cid";
const QString ag_emu_status_table::c_field_status = "status";
const QString ag_emu_status_table::c_field_status_reserve = "status_reserve";
const QString ag_emu_status_table::c_field_run_mode = "run_mode";
const QString ag_emu_status_table::c_field_sys_time = "sys_time";
ag_emu_status_table::ag_emu_status_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_emu_status_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field = QString(
                "(%1 VARCHAR(8) NOT NULL,"
                "%2 VARCHAR(8) NOT NULL,"
                "%3 VARCHAR(2) NOT NULL,"
                "%4 VARCHAR(2) NOT NULL,"
                "%5 DATETIME NOT NULL,"
                "primary key(%6,%7))"
                ).arg(c_field_emu_cid,c_field_status,c_field_status_reserve,
                      c_field_run_mode,c_field_sys_time,c_field_emu_cid,
                      c_field_sys_time);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_emu_status_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_emu_status_table::w_data(QSqlDatabase &m_database, QString emu_cid, QString status,
                                 QString reserve, QString run_mode, QString sys_time)
{
    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4,%5,%6) VALUES (?,?,?,?,?)")
            .arg(m_name,c_field_emu_cid,c_field_status,c_field_status_reserve,
                 c_field_run_mode,c_field_sys_time);
    QSqlQuery query(m_database);

    query.prepare(tmp_cmd);
    query.addBindValue(emu_cid);
    query.addBindValue(status);
    query.addBindValue(reserve);
    query.addBindValue(run_mode);
    query.addBindValue(sys_time);

    if(query.exec())
    {
    }


}

bool ag_emu_status_table::r_last_data(QSqlDatabase &m_database, QString emu_cid, QString &status, QString &reserve, QString &run_mode, QString &sys_time)
{
    QString tmp_cmd = QString("SELECT %1,%2,%3,%4 FROM %5 WHERE %6='%7' ORDER BY %8 DESC LIMIT 1")
            .arg(c_field_status,c_field_status_reserve,c_field_run_mode,c_field_sys_time,m_name,
                 c_field_emu_cid,emu_cid,c_field_sys_time);
    QSqlQuery query(m_database);


    if(query.exec(tmp_cmd))
    {
        if(query.size() == 1)
        {
            query.next();
            status = query.value(c_field_status).toString();
            reserve = query.value(c_field_status_reserve).toString();
            run_mode = query.value(c_field_run_mode).toString();
            sys_time = query.value(c_field_sys_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            return true;
        }
    }
    return false;
}








