#ifndef AG_POWER_DATA_TABLE_H
#define AG_POWER_DATA_TABLE_H

#include <QObject>

#include <QSqlQuery>
#include <QSqlDatabase>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * read
{

    "mi_cid":[
        "10010085",
        "10010084"
    ],
    "emu_cid":"90000001",
    "mim_err":"0000",
    "mis_err":"0000",
    "start_date":"2022-12-23 13:58:02",
    "stop_date":"2023-03-23 14:00:00",
    "pv_id":"1",
    "nums":"2",
    "start_num":"0"

}

{
    "all_size":"100000",
    "datas":[
        {
            "emu_cid":"90000000",
            "emu_time":"2022-12-23 13:58:02",
            "mi_cid":"10010001",
            "pv_id":1,
            "mim_version":"1.1",
            "mis_version":"1.1",
            "pv":"1.12",
            "power":"1.13",
            "energy":"2.33",
            "temperature":"1.52",
            "grid_v":"1.65",
            "grid_f":"1.63",
            "mim_err":"0000",
            "mis_err":"0000",
            //"nominal_power":600,
            "reissue_data":"NO",
            "sys_time":"2022-12-23 13:58:02"
        }
    ]
}
* read_current_power
{
    "params":
    {
        "mi_cid":[
            "10000001",
            "10000002",
            "40000001"
        ]
    }
}

{
    "datas":[
        {

            "emu_cid":"90000000",
            "emu_time":"2022-12-23 13:58:02",
            "mi_cid":"10010001",
            "pv_id":1,
            "mim_version":"1.1",
            "mis_version":"1.1",
            "pv":"1.12",
            "power":"1.13",
            "energy":"2.33",
            "temperature":"1.52",
            "grid_v":"1.65",
            "grid_f":"1.63",
            "mim_err":"0000",
            "mis_err":"0000",
            //"nominal_power":600,
            "reissue_data":"NO",
            "sys_time":"2022-12-23 13:58:02"
        }
    ]
}




* write powerdata
{
    "datas":[
        {
            "mi_cid": 10010001,
            [
                {
                    "emu_cid":"90000000",
                    "emu_time":"2022-12-23 13:58:02",
                    "mi_cid":"10010001",
                    "pv_id":1,
                    "mim_version":"1.1",
                    "mis_version":"1.1",
                    "pv":"1.12",
                    "power":"1.13",
                    "energy":"2.33",
                    "temperature":"1.52",
                    "grid_v":"1.65",
                    "grid_f":"1.63",
                    "mim_err":"0000",
                    "mis_err":"0000",
                    //"nominal_power":600,
                    "reissue_data":"NO",
                    "sys_time":"2022-12-23 13:58:02"
                }
            ]
        }
    ]
}



*/
class ag_power_data_table : public QObject
{
    Q_OBJECT
public:
    ag_power_data_table(QString name,QObject *parent = nullptr);
    ag_power_data_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database);
    bool delete_table(QSqlDatabase &m_database);

    void read_size(QSqlDatabase &m_database,QJsonObject &r_data,quint64 &size);

    void write_data(QSqlDatabase &m_database,QJsonObject &w_data);
    void write_reissue_data(QSqlDatabase &m_database,QJsonObject &w_data);
    bool read_data(QSqlDatabase &m_database,const QJsonObject &r_data,QJsonObject &data,bool is_one =false);

    void read_current_data(QSqlDatabase &m_database,QStringList &mi_list,QJsonObject &data);

    static const QString c_field_id;
    static const QString c_field_emu_cid;
    static const QString c_field_emu_time;
    static const QString c_field_mi_cid;
    static const QString c_field_mim_version;
    static const QString c_field_mis_version;
    static const QString c_field_hard_version;
    static const QString c_field_pv;
    static const QString c_field_power;
    static const QString c_field_energy;
    static const QString c_field_temperature;
    static const QString c_field_gridv;
    static const QString c_field_gridcur;
    static const QString c_field_gridf;
    static const QString c_field_mim_err;
    static const QString c_field_mis_err;
    static const QString c_field_pv_id;
    static const QString c_field_nominal_power;
    static const QString c_field_reissue_data;
    static const QString c_field_sys_time;
private:
    QString m_name = "";



signals:

};

#endif // AG_POWER_DATA_TABLE_H
