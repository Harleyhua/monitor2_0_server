#include "ag_device_control_table.h"

#include "mysql_table.h"
#include "QsLog.h"
#include <QVariant>
#include <QDateTime>
const QString ag_device_control_table::c_field_emu_cid = "emu_cid";

const QString ag_device_control_table::c_field_server_cmd = "server_cmd";
const QString ag_device_control_table::c_field_cmd_time = "cmd_time";

const QString ag_device_control_table::c_field_emu_cmd = "emu_cmd";
const QString ag_device_control_table::c_field_is_cmd_send = "is_cmd_send";
const QString ag_device_control_table::c_field_cmd_send_time = "cmd_send_time";

const QString ag_device_control_table::c_field_send_data = "send_data";
const QString ag_device_control_table::c_field_is_data_send = "is_data_send";
const QString ag_device_control_table::c_field_data_send_time = "send_time";

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
                "%5 TINYINT UNSIGNED NOT NULL,"
                "%6 DATETIME,"
                "%7 TEXT,"
                "%8 TINYINT UNSIGNED NOT NULL,"
                "%9 DATETIME,"
                "primary key(%10,%11),"
                "INDEX(%12),"
                "INDEX(%13),"
                "INDEX(%14),"
                "INDEX(%15))"
                )
            .arg(c_field_emu_cid,c_field_server_cmd,c_field_cmd_time,
                 c_field_emu_cmd,c_field_is_cmd_send,c_field_cmd_send_time,
                 c_field_send_data,c_field_is_data_send,c_field_data_send_time,
                 c_field_emu_cid,c_field_cmd_time,
                 c_field_server_cmd,c_field_emu_cmd,c_field_is_cmd_send,c_field_is_data_send);

    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_device_control_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

bool ag_device_control_table::w_data(QSqlDatabase &m_database, dev_ctl_strc data)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4,%5,%6,%7,%8) VALUES (?,?,?,?,?,?,?)")
            .arg(m_name,c_field_emu_cid,c_field_server_cmd,c_field_cmd_time,
                 c_field_emu_cmd,c_field_is_cmd_send,
                 c_field_send_data,c_field_is_data_send);
    query.prepare(tmp_cmd);
    query.addBindValue(data.emu_cid);
    query.addBindValue(data.server_cmd);
    query.addBindValue(data.cmd_time);

    query.addBindValue(data.emu_cmd);
    query.addBindValue(data.is_cmd_send);
    //query.addBindValue(data.cmd_send_time);

    query.addBindValue(data.send_data);
    query.addBindValue(data.is_data_send);
    //query.addBindValue(data.data_send_time);

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
        //QLOG_WARN() << "写入 设备控制表 失败";
    }
    return false;
}

//bool ag_device_control_table::r_data(QSqlDatabase &m_database, QString emu_cid, dev_ctl_strc &rt_data)
//{
//    QString tmp_cmd = QString("SELECT %1,%2,%3,%4,%5,%6,%7 FROM %8 WHERE %9='%10' ORDER BY %11 ASC LIMIT 1")
//            .arg(c_field_emu_cid,c_field_server_cmd,c_field_cmd_time,c_field_emu_cmd,
//                 c_field_send_data,c_field_is_send,c_field_send_time,m_name,
//                 c_field_emu_cid,emu_cid,c_field_cmd_time);
//    QSqlQuery query(m_database);

//    if(query.exec(tmp_cmd))
//    {
//        if(query.size() == 1)
//        {
//            query.next();
//            rt_data.emu_cid = query.value(c_field_emu_cid).toString();
//            rt_data.server_cmd = query.value(c_field_server_cmd).toUInt();
//            rt_data.cmd_time = query.value(c_field_cmd_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
//            rt_data.emu_cmd = query.value(c_field_emu_cmd).toUInt();
//            rt_data.send_data = query.value(c_field_send_data).toString();
//            rt_data.is_send = query.value(c_field_is_send).toUInt();
//            rt_data.send_time = query.value(c_field_send_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
//            return true;
//        }
//    }
//    return false;
//}

bool ag_device_control_table::r_first_cmd_nosend(QSqlDatabase &m_database, QString emu_cid, dev_ctl_strc &rt_data)
{
    QString tmp_cmd = QString("SELECT %1,%2,%3,%4,%5,%6,%7,%8,%9 FROM %10 WHERE %11='%12' AND %13=%14 ORDER BY %15 ASC LIMIT 1")
            .arg(c_field_emu_cid,c_field_server_cmd,c_field_cmd_time,c_field_emu_cmd,c_field_is_cmd_send,
                 c_field_cmd_send_time,c_field_send_data,c_field_is_data_send,c_field_data_send_time,m_name,
                 c_field_emu_cid,emu_cid,c_field_is_cmd_send,"0",c_field_cmd_time);
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
            rt_data.is_cmd_send = query.value(c_field_is_cmd_send).toUInt();
            rt_data.cmd_send_time = query.value(c_field_cmd_send_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

            rt_data.send_data = query.value(c_field_send_data).toString();
            rt_data.is_data_send = query.value(c_field_is_data_send).toUInt();
            rt_data.data_send_time = query.value(c_field_data_send_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            return true;
        }
    }
    //QLOG_WARN() << "查询 设备控制表 失败";
    return false;
}

bool ag_device_control_table::r_last_data_nosend(QSqlDatabase &m_database, QString emu_cid,uint8_t cmd, dev_ctl_strc &rt_data)
{
    QString tmp_cmd = QString("SELECT %1,%2,%3,%4,%5,%6,%7,%8,%9 FROM %10 WHERE %11='%12' AND %13=%14 AND %15=%16 ORDER BY %17 DESC LIMIT 1")
            .arg(c_field_emu_cid,c_field_server_cmd,c_field_cmd_time,c_field_emu_cmd,c_field_is_cmd_send,
                 c_field_cmd_send_time,c_field_send_data,c_field_is_data_send,c_field_data_send_time,m_name,
                 c_field_emu_cid,emu_cid,c_field_is_data_send,"0",c_field_emu_cmd,QString::number(cmd),c_field_cmd_time);
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
            rt_data.is_cmd_send = query.value(c_field_is_cmd_send).toUInt();
            rt_data.cmd_send_time = query.value(c_field_cmd_send_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

            rt_data.send_data = query.value(c_field_send_data).toString();
            rt_data.is_data_send = query.value(c_field_is_data_send).toUInt();
            rt_data.data_send_time = query.value(c_field_data_send_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            return true;
        }
    }
    //QLOG_WARN() << "查询 设备控制表 失败";
    return false;
}

bool ag_device_control_table::update_first_data(QSqlDatabase &m_database, dev_ctl_strc data)
{
    dev_ctl_strc sql_first_ctl = {"",0,"",0,"",0,"",0,""};

    //读取未发送的最前面的指令
    if(r_first_cmd_nosend(m_database,data.emu_cid,sql_first_ctl))
    {
        //如果是同一个服务器命令  直接覆盖最新的
        if(sql_first_ctl.server_cmd == data.server_cmd)
        {
            QString tmp_cmd = QString("UPDATE %1 SET %2='%3',%4=%5,%6='%7',%8=%9,%10=%11,%12='%13',%14='%15',%16=%17 WHERE %18='%19' AND %20='%21'")
                    .arg(m_name,c_field_emu_cid,data.emu_cid,c_field_server_cmd,QString::number(data.server_cmd),c_field_cmd_time,data.cmd_time,
                         c_field_emu_cmd,QString::number(data.emu_cmd),c_field_is_cmd_send,QString::number(data.is_cmd_send),
                         c_field_cmd_time,data.cmd_send_time,c_field_send_data,data.send_data,
                         c_field_is_data_send,QString::number(data.is_data_send),c_field_emu_cid,sql_first_ctl.emu_cid,
                         c_field_cmd_time,sql_first_ctl.cmd_time);
            QSqlQuery query(m_database);
            if(query.exec(tmp_cmd))
            {
                return true;
            }
        }
    }

    //QLOG_WARN() << "更新 设备控制表 第一条数据 失败";
    return false;
}

bool ag_device_control_table::is_data_send(QSqlDatabase &m_database, QString emu_cid, QString ctl_time)
{
    QString tmp_cmd = QString("SELECT %1 FROM %2 WHERE %3='%4' AND %5='%6'")
            .arg(c_field_is_data_send,m_name,c_field_emu_cid,emu_cid,c_field_cmd_time,ctl_time);

    QSqlQuery query(m_database);

    if(query.exec(tmp_cmd))
    {
        if(query.next())
        {
            //已经完成跟设备的数据通讯
            if(query.value(c_field_is_data_send).toInt() == 1)
            {
                return true;
            }
        }
    }

    return false;
}

void ag_device_control_table::update_cmd_send_flag(QSqlDatabase &m_database, dev_ctl_strc s_data)
{
    QString tmp_cmd = QString("UPDATE %1 SET %2=%3,%4='%5' WHERE %6='%7' AND %8='%9'")
            .arg(m_name,c_field_is_cmd_send,QString::number(s_data.is_cmd_send),c_field_cmd_send_time,s_data.cmd_send_time,
                 c_field_emu_cid,s_data.emu_cid,c_field_cmd_time,s_data.cmd_time);
    QSqlQuery query(m_database);

    if(query.exec(tmp_cmd))
    {
        return;
    }

    //QLOG_WARN() << "更新 设备控制表 数据 失败";

}

void ag_device_control_table::update_data_send_flag(QSqlDatabase &m_database, dev_ctl_strc s_data)
{
    QString tmp_cmd = QString("UPDATE %1 SET %2=%3,%4='%5' WHERE %6='%7' AND %8='%9'")
            .arg(m_name,c_field_is_data_send,QString::number(s_data.is_data_send),c_field_data_send_time,s_data.data_send_time,
                 c_field_emu_cid,s_data.emu_cid,c_field_cmd_time,s_data.cmd_time);
    QSqlQuery query(m_database);

    if(query.exec(tmp_cmd))
    {
        return;
    }

    //QLOG_WARN() << "更新 设备控制表 数据 失败";
}


