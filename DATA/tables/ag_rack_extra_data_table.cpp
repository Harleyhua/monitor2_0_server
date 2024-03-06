#include "ag_rack_extra_data_table.h"

#include <QJsonArray>
#include <QSqlQuery>

#include "QsLog.h"
#include "mysql_table.h"

const QString ag_rack_extra_data_table::c_field_mi_cid = "mi_cid";
const QString ag_rack_extra_data_table::c_field_start_time = "start_time";
const QString ag_rack_extra_data_table::c_field_aging_time = "age_time";

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

void ag_rack_extra_data_table::write_data(QSqlDatabase &m_database,const QJsonObject &w_data)
{
    QJsonArray tmp_datas = w_data.value("datas").toArray();
    QString cmd = QString("(%1,%2,%3) values(?,?,?)")
                  .arg(c_field_mi_cid,c_field_start_time,c_field_aging_time);
    QString head = QString("INSERT INTO %1 ").arg(m_name);
    QSqlQuery query(m_database);

    QVariantList record[3];

    for(int i=0;i<tmp_datas.size();i++)
    {
        QJsonObject rack_obj = tmp_datas[i].toObject();
        if(rack_obj.value(c_field_aging_time).toInt() == 0)
        {
            continue;
        }
        record[0] << rack_obj.value(c_field_mi_cid).toString();
        record[1] << rack_obj.value(c_field_start_time).toString();
        record[2] << rack_obj.value(c_field_aging_time).toInt();
    }

    query.prepare(head + cmd);
    query.addBindValue(record[0]);
    query.addBindValue(record[1]);
    query.addBindValue(record[2]);

    if(query.execBatch())
    {

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

