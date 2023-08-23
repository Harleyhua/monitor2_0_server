#include "ag_device_control_table.h"

#include "mysql_table.h"
#include <QVariant>
#include <QDateTime>
const QString ag_device_control_table::c_field_emu_cid = "emu_cid";
const QString ag_device_control_table::c_field_server_cmd = "server_cmd";
const QString ag_device_control_table::c_field_cmd_time = "cmd_time";
const QString ag_device_control_table::c_field_emu_cmd = "emu_cmd";
const QString ag_device_control_table::c_field_send_data = "send_data";
const QString ag_device_control_table::c_field_is_send = "is_send";
const QString ag_device_control_table::c_field_send_time = "send_time";

ag_device_control_table::ag_device_control_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_device_control_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field = QString(
                "(%1 VARCHAR(8) NOT NULL,"
                "%2 TINYINT UNSIGNED NOT NULL,"
                "%3 DATETIME NOT NULL,"
                "%4 TINYINT UNSIGNED NOT NULL,"
                "%5 TEXT,"
                "%6 TINYINT UNSIGNED NOT NULL,"
                "%7 DATETIME,"
                "primary key(%8,%9),"
                "INDEX(%10),"
                "INDEX(%11),"
                "INDEX(%12))"
                )
            .arg(c_field_emu_cid,c_field_server_cmd,c_field_cmd_time,c_field_emu_cmd,
                 c_field_send_data,c_field_is_send,c_field_send_time,
                 c_field_emu_cid,c_field_cmd_time,c_field_server_cmd,c_field_emu_cmd,
                 c_field_is_send);

    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_device_control_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

bool ag_device_control_table::w_data(QSqlDatabase &m_database, dev_ctl_strc data)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4,%5,%6,%7) VALUES (?,?,?,?,?,?)")
            .arg(m_name,c_field_emu_cid,c_field_server_cmd,c_field_cmd_time,c_field_emu_cmd,
                 c_field_send_data,c_field_is_send);
    query.prepare(tmp_cmd);
    query.addBindValue(data.emu_cid);
    query.addBindValue(data.server_cmd);
    query.addBindValue(data.cmd_time);
    query.addBindValue(data.emu_cmd);
    query.addBindValue(data.send_data);
    query.addBindValue(data.is_send);


//    if(data.send_time == "")
//    {
//        query.addBindValue(data.send_time);
//    }
//    else
//    {
//        query.addBindValue(NULL);
//    }


    if(query.exec())
    {
        return true;
    }
    else
    {

    }
    return false;
}

bool ag_device_control_table::r_data(QSqlDatabase &m_database, QString emu_cid, dev_ctl_strc &rt_data)
{
    QString tmp_cmd = QString("SELECT %1,%2,%3,%4,%5,%6,%7 FROM %8 WHERE %9='%10' ORDER BY %11 ASC LIMIT 1")
            .arg(c_field_emu_cid,c_field_server_cmd,c_field_cmd_time,c_field_emu_cmd,
                 c_field_send_data,c_field_is_send,c_field_send_time,m_name,
                 c_field_emu_cid,emu_cid,c_field_cmd_time);
    QSqlQuery query(m_database);

    if(query.exec(tmp_cmd))
    {
        if(query.size() == 1)
        {
            query.next();
            rt_data.emu_cid = query.value(c_field_emu_cid).toString();
            rt_data.server_cmd = query.value(c_field_server_cmd).toUInt();
            rt_data.cmd_time = query.value(c_field_cmd_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            rt_data.emu_cmd = query.value(c_field_emu_cmd).toUInt();
            rt_data.send_data = query.value(c_field_send_data).toString();
            rt_data.is_send = query.value(c_field_is_send).toUInt();
            rt_data.send_time = query.value(c_field_send_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            return true;
        }
    }
    return false;
}

bool ag_device_control_table::r_first_data_nosend(QSqlDatabase &m_database, QString emu_cid, dev_ctl_strc &rt_data)
{
    QString tmp_cmd = QString("SELECT %1,%2,%3,%4,%5,%6,%7 FROM %8 WHERE %9='%10' AND %11=%12 ORDER BY %13 DESC LIMIT 1")
            .arg(c_field_emu_cid,c_field_server_cmd,c_field_cmd_time,c_field_emu_cmd,
                 c_field_send_data,c_field_is_send,c_field_send_time,m_name,
                 c_field_emu_cid,emu_cid,c_field_is_send,"0",c_field_cmd_time);
    QSqlQuery query(m_database);

    if(query.exec(tmp_cmd))
    {
        if(query.size() == 1)
        {
            query.next();
            rt_data.emu_cid = query.value(c_field_emu_cid).toString();
            rt_data.server_cmd = query.value(c_field_server_cmd).toUInt();
            rt_data.cmd_time = query.value(c_field_cmd_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            rt_data.emu_cmd = query.value(c_field_emu_cmd).toUInt();
            rt_data.send_data = query.value(c_field_send_data).toString();
            rt_data.is_send = query.value(c_field_is_send).toUInt();
            rt_data.send_time = query.value(c_field_send_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            return true;
        }
    }
    return false;
}

bool ag_device_control_table::update_first_data(QSqlDatabase &m_database, dev_ctl_strc data)
{
    dev_ctl_strc sql_first_ctl = {"",0,"",0,"",0,""};

    //读取未发送的最前面的指令
    if(r_first_data_nosend(m_database,data.emu_cid,sql_first_ctl))
    {
        //如果是同一个服务器命令  直接覆盖最新的
        if(sql_first_ctl.server_cmd == data.server_cmd)
        {
            QString tmp_cmd = QString("UPDATE %1 SET %2='%3',%4=%5,%6='%7',%8=%9,%10='%11',%12=%13 WHERE %16='%17' AND %18='%19'")
                    .arg(m_name,c_field_emu_cid,data.emu_cid,c_field_server_cmd,QString::number(data.server_cmd),c_field_cmd_time,data.cmd_time,
                         c_field_emu_cmd,QString::number(data.emu_cmd),c_field_send_data,data.send_data,
                         c_field_is_send,QString::number(data.is_send),c_field_emu_cid,sql_first_ctl.emu_cid,
                         c_field_cmd_time,sql_first_ctl.cmd_time);
            QSqlQuery query(m_database);
            if(query.exec(tmp_cmd))
            {
                return true;
            }
        }
    }
    return false;
}

void ag_device_control_table::update_send_flag(QSqlDatabase &m_database, dev_ctl_strc s_data)
{
    QString tmp_cmd = QString("UPDATE %1 SET %2=%3,%4='%5' WHERE %6='%7' AND %8='%9'")
            .arg(m_name,c_field_is_send,QString::number(s_data.is_send),c_field_send_time,s_data.send_time,
                 c_field_emu_cid,s_data.emu_cid,c_field_cmd_time,s_data.cmd_time);
    QSqlQuery query(m_database);

    if(query.exec(tmp_cmd))
    {
    }
}


