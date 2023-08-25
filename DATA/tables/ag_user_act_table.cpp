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
                "primary key(%6,%7))")
            .arg(c_field_user,c_field_total_station,c_field_act,
                 c_field_act_desc,c_field_act_time,c_field_user,
                 c_field_act_time);

    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_user_act_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

bool ag_user_act_table::read_datas(QSqlDatabase &m_database, QJsonObject &s_data, QJsonObject &r_data)
{
    QString select_cmd = QString("SELECT %1,%2,%3,%4,%5 FROM %6 ")
            .arg(c_field_user,c_field_total_station,c_field_act,c_field_act_desc,
                 c_field_act_time,m_name);
    QSqlQuery query(m_database);

    select_cmd += QString("WHERE %1='%2' ").arg(c_field_act,s_data.value("total_station").toString());


}


