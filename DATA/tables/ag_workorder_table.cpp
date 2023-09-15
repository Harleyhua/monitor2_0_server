#include "ag_workorder_table.h"
#include "QsLog.h"
#include "mysql_table.h"

const QString ag_workorder_table::c_field_workorder_id = "workorder_id";
const QString ag_workorder_table::c_field_mi_series_id = "mi_series_id";
const QString ag_workorder_table::c_field_extra_id = "extra_id";
const QString ag_workorder_table::c_field_vacant_id = "vacant_id";
const QString ag_workorder_table::c_field_update_record = "update_record";


ag_workorder_table::ag_workorder_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_workorder_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field = QString("(%1 VARCHAR(50) NOT NULL primary key,"
                                "%2 TEXT,"
                                "%3 TEXT,"
                                "%4 TEXT,"
                                "%5 TEXT)")
            .arg(c_field_workorder_id,c_field_mi_series_id,c_field_extra_id,
                 c_field_vacant_id,c_field_update_record);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_workorder_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_workorder_table::write_workorder(QSqlDatabase &m_database, QJsonObject &w_data)
{
    QJsonArray data_array = w_data.value("datas").toArray();

    QString para = QString(" (%1,%2,%3,%4,%5) values(?,?,?,?,?) ")
            .arg(c_field_workorder_id,c_field_mi_series_id,c_field_extra_id,
                 c_field_vacant_id,c_field_update_record);
    QSqlQuery query(m_database);

    for(int i=0;i<data_array.size();i++)
    {
        QJsonObject data_obj = data_array[i].toObject();
        //查找索引表下的不同编号的MI数量
        QString tmp_cmd = QString("INSERT INTO %1 ") .arg(m_name) + para +
                QString(" ON DUPLICATE KEY UPDATE %1='%2',%3='%4',%5='%6',%7='%8'")
                .arg(c_field_mi_series_id,data_obj.value(c_field_mi_series_id).toString(),
                     c_field_extra_id,data_obj.value(c_field_extra_id).toString(),
                     c_field_vacant_id,data_obj.value(c_field_vacant_id).toString(),
                     c_field_update_record,data_obj.value(c_field_update_record).toString());
        query.prepare(tmp_cmd);
        query.addBindValue(data_obj.value(c_field_workorder_id).toString());
        query.addBindValue(data_obj.value(c_field_mi_series_id).toString());
        query.addBindValue(data_obj.value(c_field_extra_id).toString());
        query.addBindValue(data_obj.value(c_field_vacant_id).toString());
        query.addBindValue(data_obj.value(c_field_update_record).toString());

        if(query.exec())
        {
            //QLOG_INFO() << QString("写入工单数据表成功");
        }
        else {
            QLOG_WARN() << QString("写入工单数据表失败");
        }
    }
}

void ag_workorder_table::read_workorder(QSqlDatabase &m_database, QJsonObject &r_data, QJsonObject &data)
{
    QString tmp_cmd = QString("SELECT %1,%2,%3,%4 FROM %5 WHERE %6='%7' ")
            .arg(c_field_mi_series_id,c_field_extra_id,c_field_vacant_id,c_field_update_record,
                 m_name,c_field_workorder_id,r_data.value(c_field_workorder_id).toString());
    QSqlQuery query(m_database);
    QJsonObject data_obj;
    query.prepare(tmp_cmd);
    if(query.exec())
    {
        if(query.next())
        {
            data_obj.insert(c_field_workorder_id,r_data.value(c_field_workorder_id).toString());
            data_obj.insert(c_field_mi_series_id,query.value(c_field_mi_series_id).toString());
            data_obj.insert(c_field_extra_id,query.value(c_field_extra_id).toString());
            data_obj.insert(c_field_vacant_id,query.value(c_field_vacant_id).toString());
            data_obj.insert(c_field_update_record,query.value(c_field_update_record).toString());

            data.insert("datas",data_obj);
            QLOG_INFO() << QString("写入工单数据表成功");
            return;
        }
    }
    QLOG_WARN() << QString("读工单数据表失败");
}
