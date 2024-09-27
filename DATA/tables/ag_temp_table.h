#ifndef AG_TEMP_TABLE_H
#define AG_TEMP_TABLE_H

#include <QObject>
#include <QHash>
#include <QSqlDatabase>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * w_data
{
    "datas":[
        {
            "room_id":"room-1",
            "run_status":1,
            "cur_temp":500,
            "set_temp":500,
            "cur_time":"2023-01-30 19:10:00"
        },
        {
            "room_id":"room-1",
            "run_status":1,
            "cur_temp":500,
            "set_temp":500,
            "cur_time":"2023-03-01 14:15:00"
        }
    ]
}

 * r_data
{
    "room_id":"room-1",
    "start_time":"2023-01-30 19:10:00",
    "stop_time":"2023-03-01 14:15:00",
    "nums":"2",
    "start_num":"0"

}

{
    "datas":[
        {
            "room_id":"room-1",
            "run_status":1,
            "cur_temp":500,
            "set_temp":500,
            "cur_time":"2023-01-30 19:10:00"
        },
        {
            "room_id":"room-1",
            "run_status":1,
            "cur_temp":500,
            "set_temp":500,
            "cur_time":"2023-01-30 19:12:00"
        }
    ]

}

*/


class ag_temp_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_temp_table(QObject *parent = nullptr);
    ~ag_temp_table();
    bool create_table(QSqlDatabase &m_database);
    bool delete_table(QSqlDatabase &m_database);
    void write_temp(QSqlDatabase &m_database,QString room,bool runstatus,
                    int curtemp,int settemp,QString curtime);
    void write_temp(QSqlDatabase &m_database,const QJsonObject &w_data);
    void write_temp_new(QSqlDatabase &m_database,const QJsonObject &w_data);
    void write_temp(QSqlDatabase &m_database,const QJsonObject &w_data,bool writeOnes);
    bool read_temp(QSqlDatabase &m_database,const QJsonObject &r_data,QJsonObject &data);
    void read_temp(QSqlDatabase &m_database,QString room_name,QString start_time,QString stop_time,QHash<QString, uint16_t> &temp);
    static const QString c_field_room_id;
    static const QString c_field_run_status;
    static const QString c_field_cur_temp;
    static const QString c_field_set_temp;
    static const QString c_field_cur_time;
private:
    QString m_name = "room_temp_table";


signals:

};

#endif // AG_TEMP_TABLE_H
