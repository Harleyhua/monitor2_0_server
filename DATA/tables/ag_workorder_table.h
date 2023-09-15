#ifndef AG_WORKORDER_TABLE_H
#define AG_WORKORDER_TABLE_H

#include <QObject>

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
/*
 * w_data
{
    "datas":[
        {
            "workorder_id":"001",
            "mi_series_id":"10010008~10010010",
            "extra_id":"10010020",
            "vacant_id":"10010009",
            "update_record":"2022-12-23 13:58:02"
        }
    ]
}

 * r_data
{
    "workorder_id":"001"
}
{
    "datas":{
            "workorder_id":"001",
            "mi_series_id":"10010008~10010010",
            "extra_id":"10010020",
            "vacant_id":"10010009",
            "update_record":"2022-12-23 13:58:02"
        }
}



*/
class ag_workorder_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_workorder_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    void write_workorder(QSqlDatabase &m_database,QJsonObject &w_data);
    void read_workorder(QSqlDatabase &m_database,QJsonObject &r_data,QJsonObject &data);

    static const QString c_field_workorder_id;
    static const QString c_field_mi_series_id;
    static const QString c_field_extra_id;
    static const QString c_field_vacant_id;
    static const QString c_field_update_record;
private:
    QString m_name = "workorder_table";



signals:

};

#endif // AG_WORKORDER_TABLE_H
