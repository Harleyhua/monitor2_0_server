#ifndef AG_RACK_INDEX_TABLE_H
#define AG_RACK_INDEX_TABLE_H

#include <QObject>
#include <QHash>
#include <QSqlDatabase>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
class ag_rack_index_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_rack_index_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database);
    bool delete_table(QSqlDatabase &m_database);

    void write_rack_index(QSqlDatabase &m_database,QJsonObject &w_data);
    void read_rack_index(QSqlDatabase &m_database,QJsonObject &r_data,QJsonObject &data);

    QStringList read_tablelist_from_mi(QSqlDatabase &m_database,QStringList mi_cid);
    QStringList read_tablelist_from_time(QSqlDatabase &m_database,QString start_time,QString stop_time);
    QStringList read_tablelist_from_time(QSqlDatabase &m_database,QString start_time);
    static const QString c_field_cid;
    static const QString c_field_date;
    static const QString c_field_pwtable_name;
private:
    QString m_name = "rack_index_table";


signals:

};

#endif // AG_RACK_INDEX_TABLE_H
