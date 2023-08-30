#include "ag_user_station_table.h"

#include "mysql_table.h"

const QString ag_user_station_table::c_field_account = "total_stations";
const QString ag_user_station_table::c_field_station = "station";


ag_user_station_table::ag_user_station_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_user_station_table::create_table(QSqlDatabase &m_datebase)
{
    QString tmp_field = QString(
                "(%1 VARCHAR(30) NOT NULL,"
                "%2 VARCHAR(30) NOT NULL,"
                "primary key(%3,%4))")
            .arg(c_field_account,c_field_station,c_field_account,c_field_station);
    return mysql_table::create_table(m_datebase,m_name,tmp_field);
}

bool ag_user_station_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

bool ag_user_station_table::is_station_exist(QSqlDatabase &m_database, QString total_station, QString station)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("SELECT %1 FROM %2 WHERE %3='%4' AND %5='%6'")
            .arg(c_field_station,m_name,c_field_account,total_station,c_field_station,station);
    if(query.exec(tmp_cmd))
    {
        if(query.size() == 1)
        {
            return true;
        }
    }
    return false;
}

void ag_user_station_table::write_station(QSqlDatabase &m_database, QString total_station, QString station)
{
    QSqlQuery query(m_database);
    //QJsonArray tmp_datas = w_data.value("params").toObject().value("station").toArray();

    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3) VALUES (?,?)")
            .arg(m_name,c_field_account,c_field_station);
    query.prepare(tmp_cmd);

    query.addBindValue(total_station);
    query.addBindValue(station);

    if(query.exec())
    {

    }
    else
    {

    }

}

//void ag_user_station_table::write_station(QSqlDatabase &m_database, QJsonObject &w_data)
//{
//    QSqlQuery query(m_database);
//    QJsonObject tmp_data = w_data.value("params").toObject();
//    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3) VALUES (?,?)")
//            .arg(m_name,c_field_account,c_field_station);
//    query.prepare(tmp_cmd);

//    query.addBindValue(tmp_data.value(c_field_account).toString());
//    query.addBindValue(tmp_data.value(c_field_station).toString());

//    if(query.exec())
//    {

//    }
//    else
//    {

//    }

//}

void ag_user_station_table::write_station(QSqlDatabase &m_database, QString total_station, QJsonObject &w_data)
{
    QSqlQuery query(m_database);
    QJsonArray tmp_datas = w_data.value("params").toObject().value("station").toArray();


    for(int i=0;i<tmp_datas.size();i++)
    {
        QString tmp_cmd = QString("INSERT INTO %1 (%2,%3) VALUES (?,?)")
                .arg(m_name,c_field_account,c_field_station);
        query.prepare(tmp_cmd);

        query.addBindValue(total_station);
        query.addBindValue(tmp_datas[i].toString());

        if(query.exec())
        {

        }
        else
        {

        }
    }

}

//void ag_user_station_table::read_station(QSqlDatabase &m_database, QJsonObject &r_data, QJsonObject &data)
//{
//    QSqlQuery query(m_database);
//    QString total_station = r_data.value("params").toObject().value("").toString();
//    QString tmp_cmd = QString("SELECT DISTINCT %1 FROM %2 WHERE %3='%4'")
//            .arg(c_field_station,m_name,c_field_account,account);
//    if(query.exec(tmp_cmd))
//    {
//        QJsonArray station_datas;
//        while(query.next())
//        {
//            station_datas.append(query.value(c_field_station).toString());
//        }

//        QJsonObject datas;
//        datas.insert(c_field_account,account);
//        datas.insert(c_field_station,station_datas);

//        data.insert("datas",datas);
//    }
//}

void ag_user_station_table::read_station(QSqlDatabase &m_database, QString total_station, QJsonObject &data)
{
    QSqlQuery query(m_database);
    //QString account = r_data.value("params").toObject().value(c_field_account).toString();
    QString tmp_cmd = QString("SELECT DISTINCT %1 FROM %2 WHERE %3='%4' ORDER BY %5")
            .arg(c_field_station,m_name,c_field_account,total_station,c_field_station);
    if(query.exec(tmp_cmd))
    {
        QJsonArray station_datas;
        while(query.next())
        {
            station_datas.append(query.value(c_field_station).toString());
        }

        QJsonObject datas;
        datas.insert(c_field_account,total_station);
        datas.insert(c_field_station,station_datas);

        data.insert("datas",datas);
    }
}

void ag_user_station_table::del_station_by_station(QSqlDatabase &m_database, QString total_station)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("DELETE FROM %1 WHERE %2='%3'")
            .arg(m_name,c_field_station,total_station);
    if(query.exec(tmp_cmd))
    {

    }
    else
    {

    }


}



