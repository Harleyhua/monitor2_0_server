#ifndef AG_EMU_HANDSHAKE_TABLE_H
#define AG_EMU_HANDSHAKE_TABLE_H

#include <QObject>

#include <QSqlQuery>
#include <QSqlDatabase>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


class ag_emu_handshake_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_emu_handshake_table(QObject *parent = nullptr);

    static const QString c_field_id;
    static const QString c_field_emu_cid;
    static const QString c_field_sys_time;

    bool create_table(QSqlDatabase &m_database);
    bool delete_table(QSqlDatabase &m_database);

    void write_data(QSqlDatabase &m_database,QJsonObject &w_data);
    bool read_data(QSqlDatabase &m_database,const QJsonObject &r_data,QJsonObject &data);

    //void read_size(QSqlDatabase &m_database,QJsonObject &r_data,quint64 &size);
    void read_last_hand_data_time(QSqlDatabase &m_database,QString emu_cid,QString &time);
private:
    QString m_name = "emu_handshake_table";

};

#endif // AG_EMU_HANDSHAKE_TABLE_H
