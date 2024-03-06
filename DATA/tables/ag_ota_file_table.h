#ifndef AG_OTA_FILE_TABLE_H
#define AG_OTA_FILE_TABLE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
class ag_ota_file_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_ota_file_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    bool write_file_desc(QSqlDatabase &m_database,QJsonObject &w_data);



    static const QString c_field_file_status;
    static const QString c_field_file_name;
    static const QString c_field_file_protocol;
    static const QString c_field_source_sv;
    static const QString c_field_source_size;
    static const QString c_field_source_format;
    static const QString c_field_source_md5;
    static const QString c_field_dest_level;
    static const QString c_field_dest_model;
    static const QString c_field_dest_hv;
    static const QString c_field_match_mim;
    static const QString c_field_match_mis;
    static const QString c_field_upload_time;
private:
    QString mName = "ota_file_table";
signals:

};

#endif // AG_OTA_FILE_TABLE_H
