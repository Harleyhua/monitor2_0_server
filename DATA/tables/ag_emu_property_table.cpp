#include "ag_emu_property_table.h"

#include "mysql_table.h"

const QString ag_emu_property_table::c_field_emu_cid = "emu_cid";
const QString ag_emu_property_table::c_field_type = "type";
const QString ag_emu_property_table::c_field_hard_version = "hard_version";
const QString ag_emu_property_table::c_field_soft_version = "soft_version";


ag_emu_property_table::ag_emu_property_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_emu_property_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString("(%1 VARCHAR(8) NOT NULL,"
                                "%2 VARCHAR(10) NOT NULL,"
                                "%3 VARCHAR(8) NOT NULL,"
                                "%4 VARCHAR(8) NOT NULL,"
                                "primary key(%5),"
                                "INDEX(%6),"
                                "INDEX(%7),"
                                "INDEX(%8))")
                        .arg(c_field_emu_cid,c_field_type,c_field_hard_version,c_field_soft_version,
                             c_field_emu_cid,c_field_type,c_field_hard_version,c_field_soft_version);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_emu_property_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_emu_property_table::write_property(QSqlDatabase &m_database, QJsonObject &w_data)
{
    QSqlQuery query(m_database);
    QJsonObject property_data = w_data.value("datas").toObject();
    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4,%5) values(?,?,?,?) ON DUPLICATE KEY UPDATE %6='%7',%8='%9',%10='%11'")
            .arg(m_name,c_field_emu_cid,c_field_type,c_field_hard_version,c_field_soft_version,
                 c_field_type,property_data.value(c_field_type).toString(),
                 c_field_hard_version,property_data.value(c_field_hard_version).toString(),
                 c_field_soft_version,property_data.value(c_field_soft_version).toString());
    query.prepare(tmp_cmd);
    query.addBindValue(property_data.value(c_field_emu_cid).toString(""));
    query.addBindValue(property_data.value(c_field_type).toString(""));
    query.addBindValue(property_data.value(c_field_hard_version).toString(""));
    query.addBindValue(property_data.value(c_field_soft_version).toString(""));

    if(query.exec())
    {


    }
    else
    {

    }

}

void ag_emu_property_table::read_property(QSqlDatabase &m_database, QJsonObject &r_data, QJsonObject &data)
{
    QSqlQuery query(m_database);
    QJsonArray emu_cid = r_data.value("params").toObject().value("emu_cid").toArray();
    QJsonArray datas;
    for(int i=0;i<emu_cid.size();i++)
    {
        QJsonObject one_emu;
        QString tmp_cmd = QString("SELECT %1,%2,%3,%4 FROM %5 WHERE %6='%7'")
                .arg(c_field_emu_cid,c_field_type,c_field_hard_version,c_field_soft_version,
                     m_name,c_field_emu_cid,emu_cid[i].toString());
        query.prepare(tmp_cmd);
        if(query.exec())
        {
            query.next();
            one_emu.insert(c_field_emu_cid,query.value(c_field_emu_cid).toString());
            one_emu.insert(c_field_type,query.value(c_field_type).toString());
            one_emu.insert(c_field_hard_version,query.value(c_field_hard_version).toString());
            one_emu.insert(c_field_soft_version,query.value(c_field_soft_version).toString());

            datas.append(one_emu);

        }
        else
        {

        }
    }
    data.insert("datas",datas);
}

void ag_emu_property_table::read_property(QSqlDatabase &m_database, QString cid, QJsonObject &data)
{
    QSqlQuery query(m_database);
    QJsonObject one_emu;
    QString tmp_cmd = QString("SELECT %1,%2,%3,%4 FROM %5 WHERE %6='%7'")
            .arg(c_field_emu_cid,c_field_type,c_field_hard_version,c_field_soft_version,
                 m_name,c_field_emu_cid,cid);
    query.prepare(tmp_cmd);
    if(query.exec())
    {
        query.next();
        one_emu.insert(c_field_emu_cid,query.value(c_field_emu_cid).toString());
        one_emu.insert(c_field_type,query.value(c_field_type).toString());
        one_emu.insert(c_field_hard_version,query.value(c_field_hard_version).toString());
        one_emu.insert(c_field_soft_version,query.value(c_field_soft_version).toString());
        data.insert("datas",one_emu);
    }
    else
    {

    }






}



