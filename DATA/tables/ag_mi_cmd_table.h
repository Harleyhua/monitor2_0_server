#ifndef AG_MI_CMD_TABLE_H
#define AG_MI_CMD_TABLE_H

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
    "datas":{
        "mi_cid":"10010001",
        "cmd":1,
        "data":"04",
        "date":"2022-12-23 13:58:02"
    }
}

 * r_data
{
    "params":{
        "mi_cid":"10010001"
    }
}

{
    "datas":{
        "mi_cid":"10010001",
        "cmd":1,
        "data":"04",
        "date":"2022-12-23 13:58:02"
    }
}

 *d_data
 {
    "datas":{
        "mi_cid":"10010001",
        "cmd":1,
        "data":"04",
        "date":"2022-12-23 13:58:02"
    }
 }

*/




class ag_mi_cmd_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_mi_cmd_table(QObject *parent = nullptr);

    static const QString c_field_id;
    static const QString c_field_cmd;
    static const QString c_field_data;
    static const QString c_field_date;

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    void write_data(QSqlDatabase &m_database,QJsonObject &w_data);
    bool read_data(QSqlDatabase &m_database,QJsonObject &r_data,QJsonObject &data);
    void delete_data(QSqlDatabase &m_database,QJsonObject &r_data);
signals:

private:
    QString m_name = "mi_cmd_table";
};

#endif // AG_MI_CMD_TABLE_H
