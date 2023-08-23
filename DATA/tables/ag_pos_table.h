#ifndef AG_POS_TABLE_H
#define AG_POS_TABLE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/* w_data
{
    "datas":[
        {
            "station":"room-1",
            "emu_cid":"90000001",
            "mi_cid":"",
            "emu_desc":"",
            "mi_desc":""
        }
    ]
}
*/

/* r_data
{
    "params":
        {
            "station":"room-1"
        }
}

{
    "station":"room-1",
    "device":
    [
        {
            "emu_cid":"90000001",
            "emu_desc":"",
            "mis":
            [
                {
                    "mi_cid":"",
                    "desc":""
                }
            ]

        }
    ]
}
*/

class ag_pos_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_pos_table(QObject *parent = nullptr);

    static const QString c_field_station;
    static const QString c_field_emu_cid;
    static const QString c_field_mi_cid;
    static const QString c_field_mi_desc;
    static const QString c_field_emu_desc;

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    //void write_emu(QSqlDatabase &m_database,);

    void write_mapping(QSqlDatabase &m_database,QJsonObject &w_data);
    void read_mapping(QSqlDatabase &m_database,QJsonObject &r_data,QJsonObject &data);
    void read_mapping(QSqlDatabase &m_database,QString station,QJsonObject &data,QStringList &mi_list);
signals:

private:
    QString m_name = "pos_table";
};

#endif // AG_POS_TABLE_H
