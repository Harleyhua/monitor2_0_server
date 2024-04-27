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

    static const QString c_field_mi_cid;
    static const QString c_field_start_time;
    static const QString c_field_aging_time;
private:
    QString m_name = "rack_extra_data_table";
signals:

};

#endif // AG_RACK_EXTRA_DATA_TABLE_H
