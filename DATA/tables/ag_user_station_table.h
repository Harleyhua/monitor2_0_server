#ifndef AG_USER_STATION_TABLE_H
#define AG_USER_STATION_TABLE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
/*
 * w_data
{
    "params":{
        "account":"jack_lin",
        "station":"room-1"
    }
}

 * w_data  批量写
{

        "station":[
            "12353",
            "342534"
        ]

}


 *r_data
{
    "params":{
        "account":"jack_lin"
    }
}

{
    "datas":{
        "account":"jack_lin",
        "station":["room-1","room-2"]
    }
}


*/


class ag_user_station_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_user_station_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_datebase);
    bool delete_table(QSqlDatabase &m_database);

    bool is_station_exist(QSqlDatabase &m_database,QString user,QString station);

    //void write_station(QSqlDatabase &m_database,QJsonObject &w_data);
    void write_station(QSqlDatabase &m_database,QString total_station,QString station);
    bool write_station(QSqlDatabase &m_database,QString total_station,const QJsonObject &w_data);
    //void read_station(QSqlDatabase &m_database,QJsonObject &r_data,QJsonObject &data);
    void read_station(QSqlDatabase &m_database,QString total_station,QJsonObject &data);

    bool del_station_by_station(QSqlDatabase &m_database,QString total_station);
    static const QString c_field_account;
    static const QString c_field_station;

private:
    //添加电站到数据库
    QString m_name = "user_station_table";
signals:

};

#endif // AG_USER_STATION_TABLE_H
