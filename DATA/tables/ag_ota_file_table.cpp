#include "ag_ota_file_table.h"

#include "mysql_table.h"
const QString ag_ota_file_table::c_field_file_status = "file_status";
const QString ag_ota_file_table::c_field_file_name = "file_name";
const QString ag_ota_file_table::c_field_file_protocol = "file_protocol";
const QString ag_ota_file_table::c_field_source_sv = "source_sv";
const QString ag_ota_file_table::c_field_source_size = "source_size";
const QString ag_ota_file_table::c_field_source_format = "source_format";
const QString ag_ota_file_table::c_field_source_md5 = "source_md5";
const QString ag_ota_file_table::c_field_dest_level = "dest_level";
const QString ag_ota_file_table::c_field_dest_model = "dest_model";
const QString ag_ota_file_table::c_field_dest_hv = "dest_hv";
const QString ag_ota_file_table::c_field_match_mim = "match_mim";
const QString ag_ota_file_table::c_field_match_mis = "match_mis";
const QString ag_ota_file_table::c_field_upload_time = "upload_time";





ag_ota_file_table::ag_ota_file_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_ota_file_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field = QString(
                "(%1 TINYINT UNSIGNED NOT NULL,"
                "%2 VARCHAR(60) NOT NULL,"
                "%3 TINYINT UNSIGNED NOT NULL,"
                "%4 SMALLINT UNSIGNED NOT NULL,"
                "%5 INT UNSIGNED NOT NULL,"
                "%6 TINYINT UNSIGNED NOT NULL,"
                "%7 VARCHAR(32) NOT NULL,"
                "%8 TINYINT UNSIGNED NOT NULL,"
                "%9 TINYINT UNSIGNED NOT NULL,"
                "%10 SMALLINT UNSIGNED NOT NULL,"
                "%11 SMALLINT UNSIGNED NOT NULL,"
                "%12 SMALLINT UNSIGNED NOT NULL,"
                "%13 DATETIME,"
                "primary key(%14,%15,%16,%17),"
                "INDEX(%18),"
                "INDEX(%19))"
                )
            .arg(c_field_file_status,c_field_file_name,c_field_file_protocol,c_field_source_sv,
                 c_field_source_size,c_field_source_format,c_field_source_md5,c_field_dest_level,
                 c_field_dest_model,c_field_dest_hv,c_field_match_mim,c_field_match_mis,
                 c_field_upload_time,
                 c_field_file_status,c_field_dest_level,c_field_dest_model,c_field_dest_hv,
                 c_field_upload_time,c_field_source_sv);

    return mysql_table::create_table(m_database,mName,tmp_field);
}

bool ag_ota_file_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,mName);
}

bool ag_ota_file_table::write_file_desc(QSqlDatabase &m_database, QJsonObject &w_data)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14)"
                              " values(?,?,?,?,?,?,?,?,?,?,?,?,?)")
            .arg(mName,c_field_file_status,c_field_file_name,c_field_file_protocol,c_field_source_sv,
                 c_field_source_size,c_field_source_format,c_field_source_md5,c_field_dest_level,
                 c_field_dest_model,c_field_dest_hv,c_field_match_mim,c_field_match_mis,
                 c_field_upload_time);
    query.prepare(tmp_cmd);
    query.addBindValue(w_data.value(c_field_file_status).toInt());
    query.addBindValue(w_data.value(c_field_file_name).toString());
    query.addBindValue(w_data.value(c_field_file_protocol).toInt());
    query.addBindValue(w_data.value(c_field_source_sv).toInt());
    query.addBindValue(w_data.value(c_field_source_size).toInt());
    query.addBindValue(w_data.value(c_field_source_format).toInt());
    query.addBindValue(w_data.value(c_field_source_md5).toString());
    query.addBindValue(w_data.value(c_field_dest_level).toInt());
    query.addBindValue(w_data.value(c_field_dest_model).toInt());
    query.addBindValue(w_data.value(c_field_dest_hv).toInt());
    query.addBindValue(w_data.value(c_field_match_mim).toInt());
    query.addBindValue(w_data.value(c_field_match_mis).toInt());
    query.addBindValue(w_data.value(c_field_upload_time).toString());

    if(query.exec())
    {
        return true;
    }

    return false;
}
