#ifndef AG_MI_PROPERTY_TABLE_H
#define AG_MI_PROPERTY_TABLE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>

#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
/*
 * w_data
{
    "datas":[
        {
            "mi_cid":"10010085",
            "nominal_power":1,
            "mim_version":500,
            "mis_version":500
        },
        {
            "mi_cid":"10010086",
            "nominal_power":1,
            "mim_version":500,
            "mis_version":500
        }
    ]
}
 * r_data
{
    "params":
    {
        "mi_cid":[
            "10010085",
            "10010086"
        ]
    }
}
{
    "datas":[
        {
            "mi_cid":"10010085",
            "nominal_power":1,
            "mim_version":500,
            "mis_version":500
        },
        {
            "mi_cid":"10010086",
            "nominal_power":1,
            "mim_version":500,
            "mis_version":500
        }
    ]
}

*
*/

class ag_mi_property_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_mi_property_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database);
    bool delete_table(QSqlDatabase &m_database);

    void write_property(QSqlDatabase &m_database,QJsonObject &w_data);
    void read_property(QSqlDatabase &m_database,QJsonObject &r_data,QJsonObject &data);
    void read_property(QSqlDatabase &m_database,QString mi_cid,QJsonObject &data);

        void read_nominal_power(QSqlDatabase &m_database,QStringList mi_cid,QHash<QString,QString> &data);

    static const QString c_field_cid;
    static const QString c_field_nominal_power;
    static const QString c_field_mim_version;
    static const QString c_field_mis_version;
    static const QString c_field_hard_version;
private:
    QString m_name = "mi_table";


signals:

};

#endif // AG_MI_PROPERTY_TABLE_H
