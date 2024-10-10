#include "ag_emu_handshake_table.h"
#include "QsLog.h"
#include "mysql_table.h"

const QString ag_emu_handshake_table::c_field_id = "id";
const QString ag_emu_handshake_table::c_field_emu_cid = "emu_cid";
const QString ag_emu_handshake_table::c_field_sys_time = "sys_time";


ag_emu_handshake_table::ag_emu_handshake_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_emu_handshake_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString("(%1 INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,"
                                "%2 VARCHAR(8) NOT NULL,"
                                "%3 DATETIME NOT NULL,"
                                "UNIQUE (%4))")
                            .arg(c_field_id,c_field_emu_cid,c_field_sys_time,c_field_emu_cid);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_emu_handshake_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_emu_handshake_table::write_data(QSqlDatabase &m_database, QJsonObject &w_data)
{
    QJsonObject d_obj = w_data.value("datas").toObject();
    QString emu_id = d_obj.value(c_field_emu_cid).toString();
    QString time = d_obj.value(c_field_sys_time).toString();

    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3) values (?,?) ON DUPLICATE KEY UPDATE %4='%5'")
                          .arg(m_name,c_field_emu_cid,c_field_sys_time,c_field_sys_time,time);

    QSqlQuery query(m_database);
    query.prepare(tmp_cmd);
    query.addBindValue(d_obj.value(c_field_emu_cid).toString());
    query.addBindValue(d_obj.value(c_field_sys_time).toString());

    if(query.exec())
    {
        QLOG_INFO() << QString("网关:%1 写入数据表成功: ").arg(d_obj.value("emu_cid").toString()) + m_name;
    }
    else
    {
        QLOG_WARN() << QString("网关:%1 写入数据表失败: ").arg(d_obj.value("emu_cid").toString()) + m_name;
    }
}

bool ag_emu_handshake_table::read_data(QSqlDatabase &m_database,const QJsonObject &r_data, QJsonObject &data)
{
    QString select_cmd_head = QString("SELECT %1,%2 FROM ").arg(c_field_emu_cid,c_field_sys_time) + m_name + " ";
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
            emu_data_js.insert(c_field_sys_time,query.value(c_field_sys_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

            emu_data_array.append(emu_data_js);
        }
        data.insert("datas",emu_data_array);

        return true;
    }
    QLOG_WARN() << "查询 网关数据失败";
    return false;
}

void ag_emu_handshake_table::read_last_hand_data_time(QSqlDatabase &m_database, QString emu_cid, QString &time)
{
    QString tmp_cmd = QString("SELECT %1 FROM %2 WHERE %3='%4' ORDER BY %5 DESC LIMIT 1")
                          .arg(c_field_sys_time,m_name,c_field_emu_cid,emu_cid,c_field_sys_time);

    QSqlQuery query(m_database);
    if(query.exec(tmp_cmd))
    {
        query.next();
        time = query.value(c_field_sys_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }
}
