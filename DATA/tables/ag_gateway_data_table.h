#ifndef AG_GATEWAY_DATA_TABLE_H
#define AG_GATEWAY_DATA_TABLE_H

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
            "version":"1.0",
            "action":"login",
            "sys_time":"2022-12-23 13:58:02"
        }
}

 * r_data
{

    "emu_cid":"90000000",
    "action":"login",
    "start_date":"2022-12-23 13:58:02",
    "stop_date":"2023-03-23 14:00:00",
    "nums":"2",
    "start_num":"0"

}

{
    "all_size":"100000",
    "datas":[
        {
            "emu_cid":"90000000",
            "version":"1.0",
            "action":"login",
            "sys_time":"2022-12-23 13:58:02"
        }
    ]
}

*/



class ag_gateway_data_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_gateway_data_table(QObject *parent = nullptr);

    static const QString c_field_id;
    static const QString c_field_emu_cid;
    static const QString c_field_version;
    static const QString c_field_action;
    static const QString c_field_sys_time;

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    void read_size(QSqlDatabase &m_database,QJsonObject &r_data,quint64 &size);

    void write_data(QSqlDatabase &m_database,QJsonObject &w_data);
    void read_data(QSqlDatabase &m_database,QJsonObject &r_data,QJsonObject &data);

    void read_last_hand_data_time(QSqlDatabase &m_database,QString emu_cid,QString &time);
private:
    QString m_name = "gateway_data_table";

signals:

};



#endif // AG_GATEWAY_DATA_TABLE_H
