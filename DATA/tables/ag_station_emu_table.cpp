#include "ag_station_emu_table.h"

#include "mysql_table.h"

const QString ag_station_emu_table::c_field_station = "station";
const QString ag_station_emu_table::c_field_emu = "emu_cid";
const QString ag_station_emu_table::c_field_emu_desc = "emu_desc";


ag_station_emu_table::ag_station_emu_table(QObject *parent)
    : QObject{parent}
{

}

ag_station_emu_table::~ag_station_emu_table()
{

}

bool ag_station_emu_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString(
                "(%1 VARCHAR(30) NOT NULL,"
                "%2 VARCHAR(8) NOT NULL,"
                "%3 TEXT,"
                "primary key(%4,%5))")
            .arg(c_field_station,c_field_emu,c_field_emu_desc,
                 c_field_station,c_field_emu);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_station_emu_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_station_emu_table::w_emu(QSqlDatabase &m_database, QString station, QJsonObject &w_data)
{
    QSqlQuery query(m_database);
    QJsonArray tmp_datas = w_data.value("params").toObject().value("emus").toArray();


    for(int i=0;i<tmp_datas.size();i++)
    {
        QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4) VALUES (?,?,?) ON DUPLICATE KEY UPDATE %5='%6'")
                .arg(m_name,c_field_station,c_field_emu,c_field_emu_desc,
                     c_field_emu_desc,tmp_datas[i].toObject().value("emu_desc").toString());
        query.prepare(tmp_cmd);

        query.addBindValue(station);
        query.addBindValue(tmp_datas[i].toObject().value("emu_cid").toString());
        query.addBindValue(tmp_datas[i].toObject().value("emu_desc").toString());

        if(query.exec())
        {

        }
        else
        {

        }
    }
}

void ag_station_emu_table::w_one_emu(QSqlDatabase &m_database, QString station, QString emu_cid, QString desc)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4) VALUES (?,?,?) ON DUPLICATE KEY UPDATE %5='%6'")
            .arg(m_name,c_field_station,c_field_emu,c_field_emu_desc,
                 c_field_emu_desc,desc);
    query.prepare(tmp_cmd);

    query.addBindValue(station);
    query.addBindValue(emu_cid);
    query.addBindValue(desc);

    if(query.exec())
    {

    }
    else
    {

    }
}

void ag_station_emu_table::r_emu(QSqlDatabase &m_database, QString station, QStringList &emu_cid, QStringList &emu_desc)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("SELECT %1,%2 FROM %3 WHERE %4='%5'")
            .arg(c_field_emu,c_field_emu_desc,m_name,c_field_station,station);
    if(query.exec(tmp_cmd))
    {
        while(query.next())
        {
            emu_cid.append(query.value(c_field_emu).toString());
            emu_desc.append(query.value(c_field_emu_desc).toString());
        }
    }
}

void ag_station_emu_table::del_emu_by_emucid(QSqlDatabase &m_database, QString station, QString emu)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("DELETE FROM %1 WHERE %2='%3' AND %4='%5'")
            .arg(m_name,c_field_station,station,c_field_emu,emu);
    if(query.exec(tmp_cmd))
    {

    }
    else
    {

    }
}

void ag_station_emu_table::del_emu_by_station(QSqlDatabase &m_database, QString station)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("DELETE FROM %1 WHERE %2='%3' ")
            .arg(m_name,c_field_station,station);
    if(query.exec(tmp_cmd))
    {

    }
    else
    {

    }


}

bool ag_station_emu_table::is_emu_exist(QSqlDatabase &m_database, QString station, QString emu)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("SELECT %1 FROM %2 WHERE %3='%4' AND %5='%6'")
            .arg(c_field_emu,m_name,c_field_station,station,c_field_emu,emu);
    if(query.exec(tmp_cmd))
    {
        if(query.size() == 1)
        {
            return true;
        }
    }
    return false;
}
