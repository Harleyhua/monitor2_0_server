#include "ag_user_act_table.h"

#include <QSqlQuery>

#include "mysql_table.h"

const QString ag_user_act_table::c_field_user = "user";
const QString ag_user_act_table::c_field_total_station = "total_station";  //绑定的总站
const QString ag_user_act_table::c_field_act = "act";
const QString ag_user_act_table::c_field_act_desc = "act_desc";
const QString ag_user_act_table::c_field_act_time = "act_time";

ag_user_act_table::ag_user_act_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_user_act_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString(
                "(%1 VARCHAR(30) NOT NULL,"
                "%2 VARCHAR(30) NOT NULL,"
                "%3 SMALLINT NOT NULL,"
                "%4 TEXT,"
                "%5 DATETIME NOT NULL,"
                "primary key(%6,%7,%8))")
            .arg(c_field_user,c_field_total_station,c_field_act,
                 c_field_act_desc,c_field_act_time,c_field_user,
                 c_field_act,c_field_act_time);

    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_user_act_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

bool ag_user_act_table::read_datas(QSqlDatabase &m_database,const QJsonObject &s_data, QJsonObject &r_data)
{
    QString select_cmd = QString("SELECT %1,%2,%3,%4,%5 FROM %6 ")
            .arg(c_field_user,c_field_total_station,c_field_act,c_field_act_desc,
                 c_field_act_time,m_name);
    QString select_tail;
    QSqlQuery query(m_database);
    QString condition;
    condition += QString(" WHERE %1='%2' ").arg(c_field_total_station,s_data.value("total_station").toString());

    if(s_data.value("user").toString() != "")
    {
        condition += QString(" AND %1='%2' ").arg(c_field_user,s_data.value("user").toString());
    }

    if(s_data.value("act").toInt() != -1)
    {
        condition += QString(" AND %1=%2 ").arg(c_field_act,QString::number(s_data.value("act").toInt()));
    }
    if(s_data.value("start_date").toString() != "" && s_data.value("stop_date").toString() != "")
    {
        condition += QString(" AND %1 BETWEEN '%2' AND '%3' ")
                .arg(c_field_act_time,s_data.value("start_date").toString(),
                     s_data.value("stop_date").toString());
    }

    select_tail += QString(" ORDER BY %1 DESC ").arg(c_field_act_time);

    if(s_data.value("nums").toString() != "-1")
    {
        select_tail += QString(" LIMIT %1,%2 ").arg(s_data.value("start_num").toString(),
                                                   s_data.value("nums").toString());
    }

    QString select_size_head = QString("SELECT COUNT(1) FROM %1").arg(m_name);
    uint64_t data_size = 0;
    if(query.exec(select_size_head + condition))
    {
        while (query.next()) {
            data_size += query.value("COUNT(1)").toUInt();
        }
    }

    r_data.insert("all_size",QString::number(data_size));

    if(query.exec(select_cmd + condition + select_tail))
    {
        QJsonArray datas_array;
        while (query.next()) {
            QJsonObject data;
            data.insert(c_field_user,query.value(c_field_user).toString());
            data.insert(c_field_total_station,query.value(c_field_total_station).toString());
            data.insert(c_field_act,query.value(c_field_act).toInt());
            data.insert(c_field_act_desc,query.value(c_field_act_desc).toString());
            data.insert(c_field_act_time,query.value(c_field_act_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            datas_array.append(data);
        }
        r_data.insert("datas",datas_array);

        return true;
    }

    return false;
}

bool ag_user_act_table::write_datas(QSqlDatabase &m_database,const QJsonObject &s_data)
{
    QString cmd = QString("INSERT INTO %1 (%2,%3,%4,%5,%6) VALUES (?,?,?,?,?)")
            .arg(m_name,c_field_user,c_field_total_station,c_field_act,
                 c_field_act_desc,c_field_act_time);
    QSqlQuery query(m_database);

    query.prepare(cmd);
    query.addBindValue(s_data.value("user").toString());
    query.addBindValue(s_data.value("total_station").toString());
    query.addBindValue(s_data.value("act").toInt());
    query.addBindValue(s_data.value("act_desc").toString());
    query.addBindValue(s_data.value("act_time").toString());

    if(query.exec())
    {
        return true;
    }
    else
    {
        return false;
    }
}


