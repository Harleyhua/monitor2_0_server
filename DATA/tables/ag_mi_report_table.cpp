#include "ag_mi_report_table.h"
#include "mysql_table.h"

const QString ag_mi_report_table::c_field_mi_cid = "mi_cid";
const QString ag_mi_report_table::c_field_start_time = "start_time";
const QString ag_mi_report_table::c_field_stop_time = "stop_time";
const QString ag_mi_report_table::c_field_alg = "alg";  //老化算法字段
const QString ag_mi_report_table::c_field_aging_report = "aging_report";
const QString ag_mi_report_table::c_field_update_time = "update_time";





ag_mi_report_table::ag_mi_report_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_mi_report_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString(
                "(%1 VARCHAR(8) NOT NULL,"
                "%2 DATETIME NOT NULL,"
                "%3 DATETIME NOT NULL,"
                "%4 TEXT,"
                "%5 TEXT,"
                "%6 DATETIME NOT NULL,"
                "primary key(%7,%8,%9,%10))")
            .arg(c_field_mi_cid,c_field_start_time,c_field_stop_time,c_field_alg,
                 c_field_aging_report,c_field_update_time,
                 c_field_mi_cid,c_field_start_time,c_field_stop_time,c_field_update_time);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_mi_report_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

bool ag_mi_report_table::read_mi_report(QSqlDatabase &m_database, QString mi, QString start_time, QString stop_time, QVector<aging_report_strc> &rt_data)
{
    QString cmd = QString("SELECT %1,%2,%3,%4,%5,%6 FROM %7 WHERE %8='%9' AND %10='%11' AND %12='%13'")
            .arg(c_field_mi_cid,c_field_start_time,c_field_stop_time,c_field_alg,c_field_aging_report,c_field_update_time,
                 m_name,c_field_mi_cid,mi,c_field_start_time,start_time,c_field_stop_time,stop_time);
    QSqlQuery query(m_database);

    if(query.exec(cmd))
    {

        while(query.next())
        {
            aging_report_strc one_report;
            one_report.mi_cid = query.value(c_field_mi_cid).toString();
            one_report.start_time = query.value(c_field_start_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            one_report.stop_time = query.value(c_field_stop_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            one_report.alg = query.value(c_field_alg).toString();
            one_report.report = query.value(c_field_aging_report).toString();
            one_report.update_time = query.value(c_field_update_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            rt_data.append(one_report);
        }
        if(query.size()>0)
            return true;
    }

    return false;
}

void ag_mi_report_table::write_mi_report(QSqlDatabase &m_database, aging_report_strc &s_data)
{
    QString cmd = QString("INSERT INTO %1 (%2,%3,%4,%5,%6,%7) VALUES (?,?,?,?,?,?)")
            .arg(m_name,c_field_mi_cid,c_field_start_time,c_field_stop_time,
                 c_field_alg,c_field_aging_report,c_field_update_time);
    QSqlQuery query(m_database);

    query.prepare(cmd);
    query.addBindValue(s_data.mi_cid);
    query.addBindValue(s_data.start_time);
    query.addBindValue(s_data.stop_time);
    query.addBindValue(s_data.alg);
    query.addBindValue(s_data.report);
    query.addBindValue(s_data.update_time);
    if(query.exec())
    {

    }
    else{

    }
}






