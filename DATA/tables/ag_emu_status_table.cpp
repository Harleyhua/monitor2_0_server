#include "ag_emu_status_table.h"

#include "mysql_table.h"
#include "QsLog.h"

#include <QVariant>
#include <QSqlQuery>
#include <QDateTime>
#include <QJsonArray>
const QString ag_emu_status_table::c_field_emu_cid = "emu_cid";
const QString ag_emu_status_table::c_field_status = "status";
const QString ag_emu_status_table::c_field_status_reserve = "status_reserve";
const QString ag_emu_status_table::c_field_run_mode = "run_mode";
const QString ag_emu_status_table::c_field_sign = "sign";
const QString ag_emu_status_table::c_field_func = "func";
const QString ag_emu_status_table::c_field_sys_time = "sys_time";
ag_emu_status_table::ag_emu_status_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_emu_status_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field = QString(
                "(%1 VARCHAR(8) NOT NULL,"
                "%2 VARCHAR(8),"
                "%3 VARCHAR(2),"
                "%4 VARCHAR(2),"
                "%5 VARCHAR(2),"
                "%6 VARCHAR(4),"
                "%7 DATETIME NOT NULL,"
                "primary key(%8,%9))"
                ).arg(c_field_emu_cid,c_field_status,c_field_status_reserve,
                      c_field_run_mode,c_field_sign,c_field_func,c_field_sys_time,
                      c_field_emu_cid,c_field_sys_time);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_emu_status_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_emu_status_table::w_data(QSqlDatabase &m_database,QJsonObject data)
{
    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4,%5,%6,%7,%8) VALUES (?,?,?,?,?,?,?)")
            .arg(m_name,c_field_emu_cid,c_field_status,c_field_status_reserve,
                 c_field_run_mode,c_field_sign,c_field_func,c_field_sys_time);
    QSqlQuery query(m_database);

    query.prepare(tmp_cmd);
    query.addBindValue(data.value(c_field_emu_cid));
    query.addBindValue(data.value(c_field_status));
    query.addBindValue(data.value(c_field_status_reserve));
    query.addBindValue(data.value(c_field_run_mode));
    query.addBindValue(data.value(c_field_sign));
    query.addBindValue(data.value(c_field_func));
    query.addBindValue(data.value(c_field_sys_time));
    if(query.exec())
    {
    }
}

void ag_emu_status_table::r_data(QSqlDatabase &m_database, QJsonObject r_data, QJsonObject &data)
{
    QString select_cmd_head = QString("SELECT %1,%2,%3,%4,%5,%6,%7 FROM ")
                            .arg(c_field_emu_cid,c_field_status,c_field_status_reserve,c_field_run_mode,
                                 c_field_sign,c_field_func,c_field_sys_time) + m_name + " ";
    QString select_size_head = QString("SELECT COUNT(1) FROM ") + m_name + " ";
    QSqlQuery query(m_database);
    bool where_flag = false;
    QString condition;

    if(r_data.value(c_field_emu_cid).toString() != "")
    {
        if(!where_flag)
        {
            where_flag = true;
            condition += " WHERE ";
        }
        else
        {
            condition += " AND ";
        }

        condition += c_field_emu_cid + QString("='%1'").arg(r_data.value(c_field_emu_cid).toString());
        //where_flag = true;
    }
    if(r_data.value("start_date").toString() != "" && r_data.value("stop_date").toString() != "")
    {
        if(!where_flag)
        {
            where_flag = true;
            condition += " WHERE ";
        }
        else if(where_flag)
        {
            condition += " AND ";
        }

        condition += QString(" %1 BETWEEN '%2' AND '%3' ").arg(c_field_sys_time,r_data.value("start_date").toString(),
                                                          r_data.value("stop_date").toString());
    }
    uint64_t size = 0;
    if(query.exec(select_size_head + condition))
    {
        query.next();
        size = query.value("COUNT(1)").toUInt();
    }
    data.insert("all_size",QString::number(size));
    condition += QString("ORDER BY %1 DESC").arg(c_field_sys_time);
    condition += QString(" LIMIT %1,%2 ").arg(r_data.value("start_num").toString(),
                                                 r_data.value("nums").toString());

    if(query.exec(select_cmd_head + condition))
    {
        QJsonArray emu_data_array;
        while (query.next()) {
            QJsonObject emu_data_js;
            emu_data_js.insert(c_field_emu_cid,query.value(c_field_emu_cid).toString());
            emu_data_js.insert(c_field_status,query.value(c_field_status).toString());
            emu_data_js.insert(c_field_status_reserve,query.value(c_field_status_reserve).toString());
            emu_data_js.insert(c_field_run_mode,query.value(c_field_run_mode).toString());
            emu_data_js.insert(c_field_sign,query.value(c_field_sign).toString());
            emu_data_js.insert(c_field_func,query.value(c_field_func).toString());
            emu_data_js.insert(c_field_sys_time,query.value(c_field_sys_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

            emu_data_array.append(emu_data_js);
        }
        data.insert("datas",emu_data_array);
        //QLOG_INFO() << "查询 网关状态数据成功";
    }
    QLOG_WARN() << "查询 网关状态数据失败";

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








