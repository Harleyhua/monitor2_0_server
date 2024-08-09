#include "ag_rack_extra_data_table.h"

#include <QJsonArray>
#include <QSqlQuery>

#include "QsLog.h"
#include "mysql_table.h"

const QString ag_rack_extra_data_table::c_field_mi_cid = "mi_cid";
const QString ag_rack_extra_data_table::c_field_start_time = "start_time";
const QString ag_rack_extra_data_table::c_field_aging_time = "age_time";
const QString ag_rack_extra_data_table::c_field_data_table_name = "data_table_name";

const QString ag_rack_extra_data_table::c_field_date = "data";
const QString ag_rack_extra_data_table::c_field_room_id = "room_id";

ag_rack_extra_data_table::ag_rack_extra_data_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_rack_extra_data_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString("(%1 VARCHAR(8) NOT NULL,"
                                "%2 DATETIME NOT NULL,"
                                "%3 INT UNSIGNED NOT NULL,"
                                "primary key(%4,%5))")
                        .arg(c_field_mi_cid,c_field_start_time,c_field_aging_time,
                             c_field_mi_cid,c_field_start_time);

    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_rack_extra_data_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

//加room_id字段
// void ag_rack_extra_data_table::write_data(QSqlDatabase &m_database,const QJsonObject &w_data)
// {
//     QJsonArray tmp_datas = w_data.value("datas").toArray();
//     QString cmd = QString("(%1,%2,%3) values(?,?,?)")
//                   .arg(c_field_mi_cid,c_field_start_time,c_field_aging_time);
//     QString head = QString("INSERT IGNORE INTO %1 ").arg(m_name);   //旧老化房  多路微逆会开多个电源 一定要加 IGNORE
//     QSqlQuery query(m_database);

//     QVariantList record[3];

//     for(int i=0;i<tmp_datas.size();i++)
//     {
//         QJsonObject rack_obj = tmp_datas[i].toObject();
//         if(rack_obj.value(c_field_aging_time).toInt() == 0)
//         {
//             continue;
//         }
//         record[0] << rack_obj.value(c_field_mi_cid).toString();
//         record[1] << rack_obj.value(c_field_start_time).toString();
//         record[2] << rack_obj.value(c_field_aging_time).toInt();
//     }

//     query.prepare(head + cmd);
//     query.addBindValue(record[0]);
//     query.addBindValue(record[1]);
//     query.addBindValue(record[2]);

//     if(query.execBatch())
//     {

//     }
//     else
//     {
//         QLOG_WARN() << "写 老化架额外数据 失败";
//     }
// }

void ag_rack_extra_data_table::write_data(QSqlDatabase &m_database,const QJsonObject &w_data)
{
    QJsonArray tmp_datas = w_data.value("datas").toArray();
    QString cmd = QString("(%1,%2,%3,%4) values(?,?,?,?)")
                      .arg(c_field_mi_cid,c_field_room_id,c_field_start_time,c_field_aging_time);
    QString head = QString("INSERT IGNORE INTO %1 ").arg(m_name);   //旧老化房  多路微逆会开多个电源 一定要加 IGNORE
    QSqlQuery query(m_database);

    QVariantList record[4];

    for(int i=0;i<tmp_datas.size();i++)
    {
        QJsonObject rack_obj = tmp_datas[i].toObject();
        if(rack_obj.value(c_field_aging_time).toInt() == 0)
        {
            continue;
        }
        record[0] << rack_obj.value(c_field_mi_cid).toString();
        record[1] << rack_obj.value(c_field_room_id).toString();
        record[2] << rack_obj.value(c_field_start_time).toString();
        record[3] << rack_obj.value(c_field_aging_time).toInt();
    }

    query.prepare(head + cmd);
    query.addBindValue(record[0]);
    query.addBindValue(record[1]);
    query.addBindValue(record[2]);
    query.addBindValue(record[3]);

    if(query.execBatch())
    {
        QLOG_WARN() << "写 老化架额外数据 成功";
    }
    else
    {
        QLOG_WARN() << "写 老化架额外数据 失败";
    }
}

void ag_rack_extra_data_table::read_data(QSqlDatabase &m_database, QString mi_cid, QString start_time, int &aging_time)
{
    QString cmd = QString("SELECT %1 FROM %2 WHERE %3='%4' AND %5='%6' ")
            .arg(c_field_aging_time,m_name,c_field_mi_cid,mi_cid,c_field_start_time,start_time);
    QSqlQuery query(m_database);

    if(query.exec(cmd))
    {
        if(query.size() == 1)
        {
            query.next();
            aging_time = query.value(c_field_aging_time).toInt();
        }
    }
    else
    {
        QLOG_WARN() << "查询 老化架额外数据 失败";
    }

}

QStringList ag_rack_extra_data_table::read_table_list_from_time(QSqlDatabase &m_database,QString room_id,QString start_time,QString stop_time)
{
    QSqlQuery query(m_database);
    QString head_cmd = QString("SELECT DISTINCT %1 FROM %2").arg(c_field_mi_cid,m_name);
    QString tail_cmd = QString(" WHERE %3='%4' AND %5>='%6'AND %7<='%8'")
                           .arg(c_field_room_id,room_id,c_field_start_time,start_time,c_field_start_time,stop_time);

    QString cmd = head_cmd + tail_cmd;
    if(query.exec(cmd))
    {
        QJsonArray mi_cid_array;
        //遍历查询结果的每一行
        while(query.next())
        {
            //从当前行中获取第一个字段的值（索引从0开始），并将其转换为QString类型
            QString mi_cid = query.value(0).toString();
            mi_cid_array.append(mi_cid);
        }
    }
    else
    {
        QLOG_WARN() << "查询 微逆批次编号 失败";
    }
}


//读取微逆批次
void ag_rack_extra_data_table::read_list_data(QSqlDatabase &m_database,QString room_id, QString start_time, QString stop_time,QStringList &batch_list)
{
    QSqlQuery query(m_database);
    QString head_cmd = QString("SELECT DISTINCT %1 FROM %2").arg(c_field_start_time,m_name);
    QString tail_cmd = QString(" WHERE %3='%4' AND %5>='%6'AND %7<='%8'")
                           .arg(c_field_room_id,room_id,c_field_start_time,start_time,c_field_start_time,stop_time);

    QString cmd = head_cmd + tail_cmd + QString(" ORDER BY %1 DESC").arg(c_field_start_time);
    if(query.exec(cmd))
    {
        QJsonArray mi_cid_array;
        //遍历查询结果的每一行
        while(query.next())
        {
            //从当前行中获取第一个字段的值（索引从0开始），并将其转换为QString类型
            batch_list.append(query.value(c_field_start_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        }
    }
    else
    {
        QLOG_WARN() << "查询 微逆批次 失败";
    }
}

//读取到微逆编号
void ag_rack_extra_data_table::read_mi_list(QSqlDatabase &m_database,QString room_id, QString start_time, QStringList &mi_list)
{
    QSqlQuery query(m_database);
    QString head_cmd = QString("SELECT DISTINCT %1 FROM %2").arg(c_field_mi_cid,m_name);
    QString tail_cmd = QString(" WHERE %3='%4' AND %5='%6'")
                           .arg(c_field_room_id,room_id,c_field_start_time,start_time);

    QString cmd = head_cmd + tail_cmd + QString(" ORDER BY %1 ASC ").arg(c_field_mi_cid);

    if(query.exec(cmd))
    {
        while(query.next())
        {
            mi_list.append(query.value(c_field_mi_cid).toString());
        }

    }
    else
    {
        QLOG_WARN() << "查询 微逆编号(根据批次查询) 失败";
    }
}

