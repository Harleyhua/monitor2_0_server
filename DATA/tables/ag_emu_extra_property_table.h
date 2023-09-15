#ifndef AG_EMU_EXTRA_PROPERTY_TABLE_H
#define AG_EMU_EXTRA_PROPERTY_TABLE_H

#include <QObject>
#include <QSqlDatabase>
class ag_emu_extra_property_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_emu_extra_property_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    void w_countercurrent(QSqlDatabase &m_database,QString emu_cid,QString countercurrent);
    bool r_countercurrent(QSqlDatabase &m_database,QString emu_cid,QString &countercurrent);

    static const QString c_field_emu_cid;
    static const QString c_field_func;
    static const QString c_field_sys_time;

private:
    QString m_name = "emu_extra_property_table";

signals:

};

#endif // AG_EMU_EXTRA_PROPERTY_TABLE_H
