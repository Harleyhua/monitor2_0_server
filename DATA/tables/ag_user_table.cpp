#include "ag_user_table.h"

//#include "QsLog.h"
#include "mysql_table.h"

const QString ag_user_table::c_field_account = "account";
const QString ag_user_table::c_field_password = "password";
const QString ag_user_table::c_field_total_stations = "total_stations";
const QString ag_user_table::c_field_permission = "permission";
const QString ag_user_table::c_field_algorithm = "algorithm";
const QString ag_user_table::c_field_login_date = "login_date";

ag_user_table::ag_user_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_user_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString(
                "(%1 VARCHAR(30) NOT NULL,"
                "%2 VARCHAR(15) NOT NULL,"
                "%3 VARCHAR(15) NOT NULL,"
                "%4 VARCHAR(100) NOT NULL,"
                "%5 VARCHAR(500) NOT NULL,"
                "%6 DATETIME NOT NULL,"
                "primary key(%7,%8))")
            .arg(c_field_account,c_field_password,c_field_total_stations,
                 c_field_permission,c_field_algorithm,c_field_login_date,
                 c_field_account,c_field_total_stations);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_user_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_user_table::write_account(QSqlDatabase &m_database, QJsonObject &w_data)
{


}

bool ag_user_table::read_account(QSqlDatabase &m_database,const QJsonObject &r_data, QJsonObject &data)
{
    QString tmp_cmd = QString("SELECT %1,%2,%3,%4,%5,%6 FROM %7 WHERE %8='%9'")
            .arg(c_field_account,c_field_password,c_field_total_stations,c_field_permission,
                 c_field_algorithm,c_field_login_date,m_name,c_field_account,
                 r_data.value(c_field_account).toString());
    QSqlQuery query(m_database);

    query.prepare(tmp_cmd);
    if(query.exec())
    {
        QJsonObject datas;
        if(query.next())
        {
            datas.insert(c_field_account,query.value(c_field_account).toString());
            datas.insert(c_field_password,query.value(c_field_password).toString());
            datas.insert(c_field_total_stations,query.value(c_field_total_stations).toString());
            datas.insert(c_field_permission,query.value(c_field_permission).toString());
            datas.insert(c_field_algorithm,query.value(c_field_algorithm).toString());
            datas.insert(c_field_login_date,query.value(c_field_login_date).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        }
        else
        {
            return false;
        }
        data.insert("datas",datas);
    }

    return true;
}

bool ag_user_table::update_algorithm(QSqlDatabase &m_database, QString account, QJsonObject w_data)
{
    QJsonDocument jDoc;
    jDoc.setObject(w_data);
    QString cmd = QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg(m_name,c_field_algorithm,QString(jDoc.toJson(QJsonDocument::Compact)),c_field_account,account);
    QSqlQuery query(m_database);

    if(query.exec(cmd))
    {
        return true;
    }

    return false;
}

//void ag_user_table::write_algorithm(QSqlDatabase &m_database, QString account, QJsonObject w_data)
//{
//    QString cmd = QString("INSERT INTO %1 (%2,%3,%4,%5,%6) VALUES (?,?,?,?,?) ON DUPLICATE KEY UPDATE %7='%8',%9='%10',%11='%12',%13='%14' ");




//}

bool ag_user_table::read_algorithm(QSqlDatabase &m_database, QString account, QJsonObject &r_data)
{
    QString cmd = QString("SELECT %1 FROM %2 WHERE %3='%4'").arg(c_field_algorithm,m_name,c_field_account,account);
    QSqlQuery query(m_database);

    if(query.exec(cmd))
    {
        if(query.next())
        {
            QJsonDocument js_doc = QJsonDocument::fromJson(query.value(c_field_algorithm).toString().toUtf8());
            r_data = js_doc.object();
            return true;
        }
    }

    return false;

}

bool ag_user_table::read_total_stations(QSqlDatabase &m_database, QString account, QString &total_station)
{
    QString cmd = QString("SELECT %1 FROM %2 WHERE %3='%4'").arg(c_field_total_stations,m_name,c_field_account,account);
    QSqlQuery query(m_database);

    if(query.exec(cmd))
    {
        if(query.next())
        {
            total_station = query.value(c_field_total_stations).toString();
            return true;
        }
    }

    return false;
}

bool ag_user_table::read_all_total_station(QSqlDatabase &m_database, QStringList &stations)
{
    QString cmd = QString("SELECT DISTINCT %1 FROM %2").arg(c_field_total_stations,m_name);

    QSqlQuery query(m_database);

    if(query.exec(cmd))
    {
        while(query.next())
        {
           stations.append(query.value(c_field_total_stations).toString());
        }
        return true;
    }

    return false;


}


