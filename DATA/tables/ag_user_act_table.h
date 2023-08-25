#ifndef AG_USER_ACT_TABLE_H
#define AG_USER_ACT_TABLE_H

#include <QObject>
#include <QSqlDatabase>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*r_data
{
    "user":"",
    "total_station":"",
    "act":-1,
    "start_date":"2022-12-23 13:58:02",
    "stop_date":"2023-03-23 14:00:00",
    "nums":"2",
    "start_num":"0"
}

{
    "all_size":0,
    "datas":[
    {
        "user":"111",
        "total_station":"222",
        "act":1,
        "act_desc":"12343545",
        "act_time":"2023-03-23 14:00:00"
    },


    ]

}


*/

/*w_data
{
    "user":"111",
    "total_station":"222",
    "act":1,
    "act_desc":"12343545",
    "act_time":"2023-03-23 14:00:00"
}
*/

enum user_act
{

};


class ag_user_act_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_user_act_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    void read_datas(QSqlDatabase &m_database,QJsonObject &s_data,QJsonObject &r_data);
    void write_datas(QSqlDatabase &m_database,QJsonObject &s_data);

    static const QString c_field_user;
    static const QString c_field_total_station;  //绑定的总站
    static const QString c_field_act; //1-删除操作  2-添加操作  3-远程控制操作
    static const QString c_field_act_desc;
    static const QString c_field_act_time;

private:
    QString m_name = "user_act_table";
signals:

};

#endif // AG_USER_ACT_TABLE_H
