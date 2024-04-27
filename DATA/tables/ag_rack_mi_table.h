#ifndef AG_RACK_MI_TABLE_H
#define AG_RACK_MI_TABLE_H

#include <QObject>
#include <QSqlQuery>
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
            "rack_id":"rack-1",
            "pos":"1-1",
            "emu_cid":"90000001",
            "mi_cid":"10010085",
            "mi_pvid": 1,
            "dc_eff_k":1,
            "dc_eff_b":0,
            "ac_eff_k":1,
            "ac_eff_b":0
        }
    ]
}
* r_data
{
    "params":
        {
            //"room_id":"room-1",
            "emu_cid":"90000001"
        }
}

{
    "datas":
    [
        {
            "room_id":"room-1",
            "rack_id":"rack-1",
            "pos":"1-1",
            "mi_cid":"10010085"
        }
    ]
}

* r_eff
{
    "params":
        {
            "room_id":"room-1",
            "rack_id":"rack-1",
            "pos":"1-1"
        }
}

{
    "datas":{
            "room_id":"room-1",
            "rack_id":"rack-1",
            "pos":"1-1",
            "mi_cid":"10010085",
            "dc_eff_k":"1",
            "dc_eff_b":"0",
            "ac_eff_k":"1",
            "ac_eff_b":"0"
        }
}
*/

class ag_rack_mi_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_rack_mi_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    bool read_eff_factor(QSqlDatabase &m_database,QJsonObject &r_data,QJsonObject &data);

    void write_mapping(QSqlDatabase &m_database,QJsonObject &w_data);
    void read_mapping(QSqlDatabase &m_database,QJsonObject &r_data,QJsonObject &data);

    static const QString c_field_room;
    static const QString c_field_rack;
    static const QString c_field_pos;
    static const QString c_field_emu_cid;
    static const QString c_field_mi_cid;
    static const QString c_field_pv_id;
    static const QString c_field_dc_eff_k;
    static const QString c_field_dc_eff_b;
    static const QString c_field_ac_eff_k;
    static const QString c_field_ac_eff_b;
private:
    QString m_name = "rack_emu_mi_table";


signals:
};

#endif // AG_RACK_MI_TABLE_H
