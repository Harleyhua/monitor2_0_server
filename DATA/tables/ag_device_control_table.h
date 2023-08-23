#ifndef AG_DEVICE_CONTROL_TABLE_H
#define AG_DEVICE_CONTROL_TABLE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
typedef struct
{
    QString emu_cid;
    uint8_t server_cmd;
    QString cmd_time;
    uint8_t emu_cmd;
    QString send_data;
    uint8_t is_send;
    QString send_time;
}dev_ctl_strc;


class ag_device_control_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_device_control_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    bool w_data(QSqlDatabase &m_database,dev_ctl_strc data);
    bool r_data(QSqlDatabase &m_database,QString emu_cid,dev_ctl_strc &rt_data);
    bool r_first_data_nosend(QSqlDatabase &m_database,QString emu_cid,dev_ctl_strc &rt_data);
    bool update_first_data(QSqlDatabase &m_database,dev_ctl_strc data);


    void update_send_flag(QSqlDatabase &m_database,dev_ctl_strc s_data);

    static const QString c_field_emu_cid;
    static const QString c_field_server_cmd;
    static const QString c_field_cmd_time;
    static const QString c_field_emu_cmd;
    static const QString c_field_send_data;
    static const QString c_field_is_send;
    static const QString c_field_send_time;
private:
    QString m_name = "device_control_table";
signals:

};

#endif // AG_DEVICE_CONTROL_TABLE_H
