#include "ag_temp_table.h"

#include <QSqlQuery>

#include "QsLog.h"
#include "mysql_table.h"

const QString ag_temp_table::c_field_room_id = "room_id";
const QString ag_temp_table::c_field_run_status = "run_status";
const QString ag_temp_table::c_field_cur_temp = "cur_temp";
const QString ag_temp_table::c_field_set_temp = "set_temp";
const QString ag_temp_table::c_field_cur_time = "cur_time";


ag_temp_table::ag_temp_table(QObject *parent)
    : QObject{parent}
{

}

ag_temp_table::~ag_temp_table()
{

}

bool ag_temp_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString(
                "(%1 VARCHAR(8) NOT NULL,"
                "%2 VARCHAR(8) NOT NULL,"
                "%3 SMALLINT NOT NULL,"
                "%4 SMALLINT NOT NULL,"
                "%5 DATETIME NOT NULL,"
                "primary key(%6,%7))")
            .arg(c_field_room_id,c_field_run_status,c_field_cur_temp,
                 c_field_set_temp,c_field_cur_time,c_field_room_id,
                 c_field_cur_time);

    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_temp_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_temp_table::write_temp(QSqlDatabase &m_database,QString room,bool runstatus,int curtemp,int settemp,QString curtime)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4,%5,%6) VALUES (?,?,?,?,?) ON DUPLICATE KEY UPDATE %7='%8',%9='%10',%11='%12' ")
            .arg(m_name,c_field_room_id,c_field_run_status,c_field_cur_temp,c_field_set_temp,c_field_cur_time,
                 c_field_run_status,QString::number(runstatus),
                 c_field_cur_temp,QString::number(curtemp),
                 c_field_set_temp,QString::number(settemp));
    query.prepare(tmp_cmd);

    query.addBindValue(room);
    query.addBindValue(runstatus);
    query.addBindValue(curtemp);
    query.addBindValue(settemp);
    query.addBindValue(curtime);

    if(query.exec())
    {
        //新建温度记录项成功
        QLOG_INFO() << "插入新的温度时间段成功 ";
    }
    else
    {
        QLOG_WARN() << QString("插入新的温度时间段失败 %1,%2,%3,%4,%5").arg(
                   room,QString::number(runstatus),QString::number(curtemp),
                   QString::number(settemp),curtime);
    }

}

void ag_temp_table::write_temp(QSqlDatabase &m_database,const QJsonObject &w_data)
{
    QSqlQuery query(m_database);
    quint16 data_count = 0;

    data_count = w_data.value("datas").toArray().size();

    for(int i=0;i<data_count;i++)
    {
        QJsonObject tmp_temp_data = w_data.value("datas").toArray()[i].toObject();

        QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4,%5,%6) VALUES (?,?,?,?,?) ON DUPLICATE KEY UPDATE %7='%8',%9='%10',%11='%12' ")
                .arg(m_name,c_field_room_id,c_field_run_status,c_field_cur_temp,c_field_set_temp,c_field_cur_time,
                     c_field_run_status,QString::number(tmp_temp_data.value(c_field_run_status).toBool(false)),
                     c_field_cur_temp,QString::number(tmp_temp_data.value(c_field_cur_temp).toInt(0)),
                     c_field_set_temp,QString::number(tmp_temp_data.value(c_field_set_temp).toInt(0)));
        query.prepare(tmp_cmd);

        query.addBindValue(tmp_temp_data.value(c_field_room_id).toString(""));
        query.addBindValue(tmp_temp_data.value(c_field_run_status).toBool(false));
        query.addBindValue(tmp_temp_data.value(c_field_cur_temp).toInt(0));
        query.addBindValue(tmp_temp_data.value(c_field_set_temp).toInt(0));
        query.addBindValue(tmp_temp_data.value(c_field_cur_time).toString(""));

        if(query.exec())
        {
            //新建温度记录项成功
            QLOG_INFO() << "插入新的温度时间段成功 ";
        }
        else
        {
            QLOG_WARN() << QString("插入新的温度时间段失败 %1,%2,%3,%4,%5").arg(
                       tmp_temp_data.value(c_field_room_id).toString(""),
                       QString::number(tmp_temp_data.value(c_field_run_status).toBool(false)),
                       QString::number(tmp_temp_data.value(c_field_cur_temp).toInt(0)),
                       QString::number(tmp_temp_data.value(c_field_set_temp).toInt(0)),
                       tmp_temp_data.value(c_field_cur_time).toString("")
                     );
        }
    }
}

bool ag_temp_table::read_temp(QSqlDatabase &m_database,const QJsonObject &r_data, QJsonObject &data)
{
    //QString s_time = time.toString("yyyy-MM-dd hh:mm:ss");
    bool where_flag = false;
    //找到该时间段 的温度值
    QString tmp_cmd = QString("SELECT %1,%2,%3,%4,%5 FROM %6 ").arg(c_field_room_id,c_field_run_status,c_field_set_temp,
                                                                    c_field_cur_temp,c_field_cur_time,m_name);
    QString select_size_head = QString("SELECT COUNT(1) FROM %1").arg(m_name);

    QString condition;

    QString condition_tail;
    QSqlQuery query(m_database);
    //WHERE %7 <= '%8' AND %9 >= '%10' ORDER BY %11 DESC
//    c_field_cur_time,r_data.value("params").toObject().value("stop_time").toString(),
//    c_field_cur_time,r_data.value("params").toObject().value("start_time").toString(),
//    c_field_cur_time);


    if(r_data.value("room_id").toString() != "")
    {
        if(!where_flag)
        {
            where_flag = true;
            condition += " WHERE ";
        }
        else {
            condition += " AND ";
        }

        condition += c_field_room_id + QString("='%1' ").arg(r_data.value("room_id").toString());
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
            condition+= " AND ";
        }

        condition += QString(" %1 BETWEEN '%2' AND '%3' ").arg(c_field_cur_time,r_data.value("start_date").toString(),
                                                          r_data.value("stop_date").toString());
    }

    condition_tail += QString(" ORDER BY %1 DESC ").arg(c_field_cur_time);

    if(r_data.value("nums").toString() != "-1")
    {
        condition_tail += QString(" LIMIT %1,%2 ").arg(r_data.value("start_num").toString(),
                                                r_data.value("nums").toString());
    }
    uint64_t data_size = 0;
    if(query.exec(select_size_head + condition))
    {
        while (query.next()) {
            data_size += query.value("COUNT(1)").toUInt();
        }
    }
    data.insert("all_size",QString::number(data_size));

    QJsonArray datas;
    if(query.exec(tmp_cmd + condition + condition_tail))
    {

        while(query.next())
        {
            QJsonObject one_datas;
            one_datas.insert(c_field_room_id,query.value(c_field_room_id).toString());
            one_datas.insert(c_field_run_status,query.value(c_field_run_status).toInt());
            one_datas.insert(c_field_set_temp,query.value(c_field_set_temp).toInt());
            one_datas.insert(c_field_cur_temp,query.value(c_field_cur_temp).toInt());
            one_datas.insert(c_field_cur_time,query.value(c_field_cur_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            datas.append(one_datas);
        }
        QLOG_INFO() <<"读取" + m_name + " 温度数据成功";
        data.insert("datas",datas);
        return true;
    }
    else
    {
        data.insert("datas",datas);
        QLOG_WARN() << "读取" + m_name+ " 温度数据失败";
        return false;
    }
}

void ag_temp_table::read_temp(QSqlDatabase &m_database, QString room_name, QString start_time, QString stop_time, QHash<QString, uint16_t> &temp)
{
    //QString s_time = time.toString("yyyy-MM-dd hh:mm:ss");
    //找到该时间段 的温度值
    QString tmp_cmd = QString("SELECT %1,%2 FROM %3 WHERE %4 <= '%5' AND %6 >= '%7' ORDER BY %8 DESC")
            .arg(c_field_cur_temp,c_field_cur_time,m_name,
                 c_field_cur_time,stop_time,c_field_cur_time,start_time,c_field_cur_time);
    QSqlQuery query(m_database);

    query.prepare(tmp_cmd);
    if(query.exec())
    {
        while(query.next())
        {
            temp.insert(query.value(c_field_cur_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"),query.value(c_field_cur_temp).toInt());
        }

        QLOG_INFO() <<"读取" + m_name + " 温度数据成功";
    }
    else
    {
        QLOG_WARN() << "读取" + m_name+ " 温度数据失败";
    }
}



