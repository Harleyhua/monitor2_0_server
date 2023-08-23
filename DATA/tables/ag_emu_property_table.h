#ifndef AG_EMU_PROPERTY_TABLE_H
#define AG_EMU_PROPERTY_TABLE_H

#include <QObject>

#include <QSqlQuery>
#include <QSqlDatabase>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * w_data
{
    "datas":{
        "emu_cid":"90000000",
        "type":"bry1000",
        "hard_version":"1.2",
        "soft_version":"1.3"
    }
}

 * r_data
{
    "params":{
        "emu_cid":["90000000","90000001"];
    }
}

{
    "datas":[
        {
            "emu_cid":"90000000",
            "type":"bry1000",
            "hard_version":"1.2",
            "soft_version":"1.3"
        }
    ]
}

 *r_data
   "90000001"

{
    "datas":{
            "emu_cid":"90000000",
            "type":"bry1000",
            "hard_version":"1.2",
            "soft_version":"1.3"
        }
}
*/

class ag_emu_property_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_emu_property_table(QObject *parent = nullptr);

    static const QString c_field_emu_cid;
    static const QString c_field_type;
    static const QString c_field_hard_version;
    static const QString c_field_soft_version;

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    void write_property(QSqlDatabase &m_database,QJsonObject &w_data);
    void read_property(QSqlDatabase &m_database,QJsonObject &r_data,QJsonObject &data);
    void read_property(QSqlDatabase &m_database,QString cid,QJsonObject &data);
private:
    QString m_name = "emu_property_table";

signals:

};

#endif // AG_EMU_PROPERTY_TABLE_H
