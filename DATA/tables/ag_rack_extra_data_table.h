#ifndef AG_RACK_EXTRA_DATA_TABLE_H
#define AG_RACK_EXTRA_DATA_TABLE_H

#include <QObject>
#include <QSqlDatabase>

#include <QJsonObject>
class ag_rack_extra_data_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_rack_extra_data_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    void write_data(QSqlDatabase &m_database,const QJsonObject &w_data);
    void read_data(QSqlDatabase &m_database,QString mi_cid,QString start_time,int &aging_time);

    QStringList read_table_list_from_time(QSqlDatabase &m_database,QString room_id,QString start_time,QString stop_time);

    //读取批次列表（新）
    void read_list_data(QSqlDatabase &m_database,QString room_id, QString start_time, QString stop_time,QStringList &batch_list);
    void read_mi_list(QSqlDatabase &m_database,QString room_id, QString start_time, QStringList &mi_list);

    static const QString c_field_mi_cid;
    static const QString c_field_start_time;
    static const QString c_field_aging_time;
    static const QString c_field_data_table_name;

    static const QString c_field_date;
    static const QString c_field_room_id;

private:
    QString m_name = "rack_extra_data_table";
signals:

};

#endif // AG_RACK_EXTRA_DATA_TABLE_H
