#ifndef AG_EMU_STATUS_TABLE_H
#define AG_EMU_STATUS_TABLE_H

#include <QObject>
#include <QSqlDatabase>
class ag_emu_status_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_emu_status_table(QObject *parent = nullptr);

    static const QString c_field_emu_cid;
    static const QString c_field_status;
    static const QString c_field_status_reserve;
    static const QString c_field_run_mode;
    static const QString c_field_sys_time;

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    void w_data(QSqlDatabase &m_database,QString emu_cid,QString status,
                QString reserve,QString run_mode,QString sys_time);
    bool r_last_data(QSqlDatabase &m_database,QString emu_cid,QString &status,
                     QString &reserve,QString &run_mode,QString &sys_time);
signals:


private:
    QString m_name = "emu_status_table";

};

#endif // AG_EMU_STATUS_TABLE_H
