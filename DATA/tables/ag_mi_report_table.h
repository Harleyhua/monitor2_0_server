#ifndef AG_MI_REPORT_TABLE_H
#define AG_MI_REPORT_TABLE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

typedef struct
{
    QString mi_cid;
    QString start_time;
    QString stop_time;
    QString alg;
    QString report;
    QString update_time;
}aging_report_strc;


class ag_mi_report_table: public QObject
{
    Q_OBJECT
public:
    explicit ag_mi_report_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    bool read_mi_report(QSqlDatabase &m_database,QString mi,QString start_time,QString stop_time,QVector<aging_report_strc> &rt_data);
    void write_mi_report(QSqlDatabase &m_database,aging_report_strc &s_data);


    static const QString c_field_mi_cid;
    static const QString c_field_start_time;
    static const QString c_field_stop_time;
    static const QString c_field_alg;  //老化算法字段
    static const QString c_field_aging_report;
    static const QString c_field_update_time;



private:
    QString m_name = "mi_report_table";
};

#endif // AG_MI_REPORT_TABLE_H
