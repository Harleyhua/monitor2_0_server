#ifndef AG_POWER_INDEX_TABLE_H
#define AG_POWER_INDEX_TABLE_H

#include <QObject>
#include <QHash>
#include <QSqlDatabase>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * w_data
{
    "datas":{
            "mi_cid":"10010085",
            "date":2301,
            "datatable_name":"power_data_2301_1"
        }
}

 * r_data
{
    "params":{
        "mi_cid":10010085,
        "date":2301
    }
}


{
    "datas":
    {
        "mi_cid":"10010085",
        "date":2301,
        "datatable_name":[
            "power_data_2301_1",
            "power_data_2301_2"
        ]
    }
}
*
*/





class ag_power_index_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_power_index_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database);
    bool delete_table(QSqlDatabase &m_database);

    void write_power_index(QSqlDatabase &m_database,QJsonObject &w_data);
    void read_power_index(QSqlDatabase &m_database,QJsonObject &r_data,QJsonObject &data);

    QStringList read_tablelist_from_mi(QSqlDatabase &m_database,QStringList mi_cid);

    static const QString c_field_cid;
    static const QString c_field_date;
    static const QString c_field_pwtable_name;
private:
    QString m_name = "power_index_table";



signals:

};

#endif // AG_POWER_INDEX_TABLE_H
