#ifndef AG_RACK_DATA_TABLE_H
#define AG_RACK_DATA_TABLE_H

#include <QObject>

#include <QSqlQuery>
#include <QSqlDatabase>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * r_data
{
    "room_id":"room-1",
    "rack_id":"rack-1",
    "mi_pos":"1-1",
    "mi_cid":["10010085","10010086"],
    "start_date":"2022-12-23 13:58:02",
    "stop_date":"2023-03-23 14:00:00",
    "nums":"2",
    "start_num":"0"
}

{
    "datas":[
        {
            "room_id":"room-1",
            "rack_id":"rack-1",
            "leakage_status":"ON",
            "mi_pos":"1-1",
            "mi_cid":"10010085",
            "dc_volt_in":"600.10",
            "dc_current_in":"2.2",
            "dc_volt_eff_k":"1",
            "dc_volt_eff_b":"0",
            "dc_power_in":"600",
            "ac_volt_out":"567",
            "ac_current_out":"230.01",
            "ac_volt_eff_k":"0.99",
            "ac_volt_eff_b":"0",
            "ac_power_out":"600.00",
            "efficiency":"98.00",
            "start_time":"2022-12-23 13:58:02",
            "cur_date":"2022-12-23 13:58:02"
        }
    ]
}


 * w_data
{
    "datas":[
        {
            "room_id":"room-1",
            "rack_id":"rack-1",
            "leakage_status":"ON",
            "mi_pos":"1-1",
            "mi_cid":"10010085",
            "dc_volt_in":"600.10",
            "dc_current_in":"2.2",
            "dc_volt_eff_k":"1",
            "dc_volt_eff_b":"0",
            "ac_current_out":"230.01",
            "ac_volt_eff_k":"0.99",
            "ac_volt_eff_b":"0",
            "ac_power_out":"600.00",
            "efficiency":"98.00",
            "start_time":"2022-12-23 13:58:02",
            "cur_date":"2022-12-23 13:58:02"
        }
    ]
}


*/




class ag_rack_data_table : public QObject
{
    Q_OBJECT
public:
    ag_rack_data_table(QString name,QObject *parent = nullptr);
    ag_rack_data_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database);
    bool delete_table(QSqlDatabase &m_database);

    void read_size(QSqlDatabase &m_database,QJsonObject &r_data,quint64 &size);

    void write_data(QSqlDatabase &m_database,QJsonObject &w_data);
    bool read_data(QSqlDatabase &m_database,const QJsonObject &r_data,QJsonObject &data);

    void read_batch_list(QSqlDatabase &m_database,QString start_time,QString stop_time,QStringList &batch_list);
    void read_mi_list(QSqlDatabase &m_database,QString start_time,QStringList &mi_list);
    //某个微逆 读取开始时间-结束时间
    //void read_last_date_by_mi(QStringList mi_list,QMap<QString,QString> mi_st,QMap<QString,QString> mi_sp);
    void read_mi_last_aging_time_by_mi(QSqlDatabase &m_database,QString mi,QString &room,QString &start_time,QString &stop_time,QString &pos_desc);
    void read_mi_stop_time_after_start_time(QSqlDatabase &m_database,QString mi, QString &room,QString start_time, QString &stop_time,QString &pos_desc);



private:
    QString m_name = "";

    static const QString c_field_id;
    static const QString c_field_room_id;
    static const QString c_field_rack_id;
    static const QString c_field_leakage_status;
    static const QString c_field_mi_pos;
    static const QString c_field_mi_cid;
    static const QString c_field_dc_volt_in;
    static const QString c_field_dc_current_in;
    static const QString c_field_dc_volt_eff_k;
    static const QString c_field_dc_volt_eff_b;
    static const QString c_field_dc_power_in;
    static const QString c_field_ac_volt_out;
    static const QString c_field_ac_current_out;
    static const QString c_field_ac_volt_eff_k;
    static const QString c_field_ac_volt_eff_b;
    static const QString c_field_ac_power_out;
    static const QString c_field_efficiency;
    static const QString c_field_start_time;
    static const QString c_field_cur_date;

    void read_mi_stop_time_after_start_time(QString mi,QString start_time,QString &stop_time);
signals:

};

#endif // AG_RACK_DATA_TABLE_H
