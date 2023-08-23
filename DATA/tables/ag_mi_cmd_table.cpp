#include "ag_mi_cmd_table.h"

//#include "QsLog.h"
#include "mysql_table.h"

const QString ag_mi_cmd_table::c_field_id = "mi_cid";
const QString ag_mi_cmd_table::c_field_cmd = "cmd";
const QString ag_mi_cmd_table::c_field_data = "data";
const QString ag_mi_cmd_table::c_field_date = "date";


ag_mi_cmd_table::ag_mi_cmd_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_mi_cmd_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString("(%1 VARCHAR(8) NOT NULL,"
                                "%2 TINYINT UNSIGNED NOT NULL,"
                                "%3 TEXT NOT NULL,"
                                "%4 DATETIME NOT NULL,"
                                "primary key(%5,%6))")
                        .arg(c_field_id,c_field_cmd,c_field_data,
                             c_field_date,c_field_id,c_field_date);

    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_mi_cmd_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_mi_cmd_table::write_data(QSqlDatabase &m_database, QJsonObject &w_data)
{
    QJsonObject d_obj = w_data.value("datas").toObject();

    QString tmp_cmd = QString("insert into %1 (%2,%3,%4,%5) values(?,?,?,?)")
            .arg(m_name,c_field_id,c_field_cmd,c_field_data,c_field_date);
    QSqlQuery query(m_database);
    query.prepare(tmp_cmd);
    query.addBindValue(d_obj.value(c_field_id).toString());
    query.addBindValue(d_obj.value(c_field_cmd).toInt());
    query.addBindValue(d_obj.value(c_field_data).toString());
    query.addBindValue(d_obj.value(c_field_date).toString());

    if(query.exec())
    {

    }
    else
    {

    }

}

bool ag_mi_cmd_table::read_data(QSqlDatabase &m_database, QJsonObject &r_data, QJsonObject &data)
{
    QJsonObject tmp_param = r_data.value("params").toObject();
    QString select_cmd = QString("SELECT %1,%2,%3,%4 FROM %5 WHERE %6='%7' ORDER BY %8 ASC LIMIT 1")
                            .arg(c_field_id,c_field_cmd,c_field_data,c_field_date,m_name,c_field_id,
                                 tmp_param.value(c_field_id).toString(),c_field_date);
    QSqlQuery query(m_database);

    if(query.exec(select_cmd))
    {
        if(query.next())
        {
            QJsonObject datas_js;
            datas_js.insert(c_field_id,query.value(c_field_id).toString());
            datas_js.insert(c_field_cmd,query.value(c_field_cmd).toString());
            datas_js.insert(c_field_data,query.value(c_field_data).toString());
            datas_js.insert(c_field_date,query.value(c_field_date).toString());
            data.insert("datas",datas_js);
            return true;
        }
    }

    return false;
}

void ag_mi_cmd_table::delete_data(QSqlDatabase &m_database, QJsonObject &r_data)
{
    QJsonObject tmp_datas = r_data.value("datas").toObject();
    QString del_cmd = QString("DELETE FROM %1 WHERE %2='%3' AND %4='%5' ")
            .arg(m_name,c_field_id,tmp_datas.value(c_field_id).toString(),
                 c_field_date,tmp_datas.value(c_field_date).toString());
    QSqlQuery query(m_database);
    if(query.exec(del_cmd))
    {

    }
}

