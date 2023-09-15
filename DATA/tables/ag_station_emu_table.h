#ifndef AG_STATION_EMU_TABLE_H
#define AG_STATION_EMU_TABLE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlQuery>
#include <QSqlDatabase>

/* w_data

{

        "station":"1234",
        "emu":[
            {
                "emu_cid":"90000000",
                "emu_desc":"rack1"
            }
        ]

}

*/





class ag_station_emu_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_station_emu_table(QObject *parent = nullptr);
    ~ag_station_emu_table();

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    void w_emu(QSqlDatabase &m_database,QString station,QJsonObject &w_data);
    void w_one_emu(QSqlDatabase &m_database,QString station,QString emu_cid,QString desc);

    void r_emu(QSqlDatabase &m_database,QString station,QStringList &emu_cid,QStringList &emu_desc);

    void del_emu_by_emucid(QSqlDatabase &m_database,QString station ,QString emu);
    void del_emu_by_station(QSqlDatabase &m_database,QString station);
    bool is_emu_exist(QSqlDatabase &m_database,QString station,QString emu);
    static const QString c_field_station;
    static const QString c_field_emu;
    static const QString c_field_emu_desc;

private:
    QString m_name = "station_emu_table";
signals:

};

#endif // AG_STATION_EMU_TABLE_H
