#ifndef AG_MI_EXTRA_PROPERTY_TABLE_H
#define AG_MI_EXTRA_PROPERTY_TABLE_H

#include <QObject>
#include <QSqlDatabase>
class ag_mi_extra_property_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_mi_extra_property_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    void w_temporary_power(QSqlDatabase &m_database,QString mi_cid,QString &temporary_power);
    void r_temporary_power(QSqlDatabase &m_database,QString mi_cid,QString &temporary_power);

    void w_max_power(QSqlDatabase &m_database,QString mi_cid,QString &max_power);
    void r_max_power(QSqlDatabase &m_database,QString mi_cid,QString &max_power);

    void w_grid(QSqlDatabase &m_database,QString mi_cid,QString &grid);
    void r_grid(QSqlDatabase &m_database,QString mi_cid,QString &grid);

    void w_certification(QSqlDatabase &m_database,QString mi_cid,QString &certification);
    void r_certification(QSqlDatabase &m_database,QString mi_cid,QString &certification);
    static const QString c_field_mi_cid;
    static const QString c_field_temporary_power;
    static const QString c_field_max_power;
    static const QString c_field_grid;
    static const QString c_field_certification;
    static const QString c_field_sys_time;

private:
    QString m_name = "mi_extra_property_table";
signals:

};

#endif // AG_MI_EXTRA_PROPERTY_TABLE_H
