#ifndef AG_USER_TABLE_H
#define AG_USER_TABLE_H

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
    ""

}


 * r_data
{
    "account":"jack_lin"

}

{
    "datas":{
        "account":"us1",
        "password":"1234",
        "permission":"",
        "algorithm":"",
        "login_date":""
    }
}

*/

class ag_user_table: public QObject
{
    Q_OBJECT
public:
    explicit ag_user_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    void write_account(QSqlDatabase &m_database,QJsonObject &w_data);
    bool read_account(QSqlDatabase &m_database,const QJsonObject &r_data,QJsonObject &data);

    //void write_algorithm(QSqlDatabase &m_database,QString account, QJsonObject w_data);
    bool update_algorithm(QSqlDatabase &m_database,QString account,QJsonObject w_data);
    bool read_algorithm(QSqlDatabase &m_database,QString account,QJsonObject &r_data);
    bool read_total_stations(QSqlDatabase &m_database,QString account,QString &total_station);
    bool read_all_total_station(QSqlDatabase &m_database,QStringList &stations);


    static const QString c_field_account;
    static const QString c_field_password;
    static const QString c_field_total_stations;  //电站总控字段
    static const QString c_field_permission;
    static const QString c_field_algorithm;
    static const QString c_field_login_date;

private:
    QString m_name = "user_table";

};

#endif // AG_USER_TABLE_H
