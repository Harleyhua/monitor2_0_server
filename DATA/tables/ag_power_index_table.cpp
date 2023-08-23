#include "ag_power_index_table.h"

#include <QSqlQuery>

#include "QsLog.h"
#include "mysql_table.h"

const QString ag_power_index_table::c_field_cid  = "mi_cid";
const QString ag_power_index_table::c_field_date = "date";
const QString ag_power_index_table::c_field_pwtable_name = "datatable_name";


ag_power_index_table::ag_power_index_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_power_index_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString("(%1 VARCHAR(8) NOT NULL,"
            "%2 SMALLINT UNSIGNED NOT NULL,"
            "%3 VARCHAR(20) NOT NULL,"
            "primary key(%4,%5),"
            "INDEX(%6))").arg(c_field_cid,c_field_date,c_field_pwtable_name,c_field_cid,
                              c_field_pwtable_name,c_field_cid);

    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_power_index_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_power_index_table::write_power_index(QSqlDatabase &m_database, QJsonObject &w_data)
{
    QSqlQuery query(m_database);

    QJsonObject tmp_data = w_data.value("datas").toObject();

    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4) VALUES (?,?,?)")
            .arg(m_name,c_field_cid,c_field_date,c_field_pwtable_name);
    query.prepare(tmp_cmd);

    query.addBindValue(tmp_data.value(c_field_cid).toString());
    query.addBindValue(tmp_data.value(c_field_date).toInt());
    query.addBindValue(tmp_data.value(c_field_pwtable_name).toString());

    if(query.exec())
    {
        QLOG_INFO() << "插入微逆下标表 ";
    }
    else
    {
        QLOG_WARN() << QString("插入微逆下标表 %1,%2,%3")
                       .arg(tmp_data.value(c_field_cid).toString(),
                            QString::number(tmp_data.value(c_field_date).toInt()),
                            tmp_data.value(c_field_pwtable_name).toString());
    }

}

void ag_power_index_table::read_power_index(QSqlDatabase &m_database, QJsonObject &r_data, QJsonObject &data)
{
    QString mi_cid = r_data.value("params").toObject().value("mi_cid").toString();
    QString date = QString::number(r_data.value("params").toObject().value("date").toInt());

    //根据cid date 获取表名
    QString tmp_cmd = QString("SELECT DISTINCT %1,%2,%3 FROM %4 WHERE %5= '%6' AND %7=%8 ORDER BY %9 DESC")
            .arg(c_field_cid,c_field_date,c_field_pwtable_name,m_name,c_field_cid,mi_cid,c_field_date,date,
                 c_field_pwtable_name);
    QSqlQuery query(m_database);
    QJsonObject one_data;
    QJsonArray db_name_array;
    query.prepare(tmp_cmd);
    if(query.exec())
    {
        while(query.next())
        {
            db_name_array.append(query.value(c_field_pwtable_name).toString());
        }
        one_data.insert(c_field_cid,mi_cid);
        one_data.insert(c_field_date,date);
        one_data.insert(c_field_pwtable_name,db_name_array);
        data.insert("datas",one_data);
    }
    else
    {
        QLOG_WARN() << QString("读取微逆下标表失败 %1,%2")
                       .arg(r_data.value("params").toObject().value("mi_cid").toString(),
                            QString::number(r_data.value("params").toObject().value("date").toInt()));
    }
}

QStringList ag_power_index_table::read_tablelist_from_mi(QSqlDatabase &m_database,QStringList mi_cid)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("SELECT DISTINCT %1 FROM %2 ")
            .arg(c_field_pwtable_name,m_name);
    QString tmp_condition;
    QStringList tb_list;
    for(int i =0;i<mi_cid.size();i++)
    {
        if(i==0)
        {
            tmp_condition.append(" WHERE ");
        }
        if(i !=0)
        {
            tmp_condition.append(" OR ");
        }
        tmp_condition.append(QString(" %1='%2' ").arg(c_field_cid,mi_cid[i]));
    }

    tmp_cmd = tmp_cmd + tmp_condition;


    if(query.exec(tmp_cmd))
    {
        while(query.next())
        {
            tb_list.append(query.value(c_field_pwtable_name).toString());
        }
    }

    return tb_list;
}

