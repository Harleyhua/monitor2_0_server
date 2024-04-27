#ifndef AG_EMU_MI_TABLE_H
#define AG_EMU_MI_TABLE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlQuery>
#include <QSqlDatabase>
/* w_data

{
    "params":{
        "station":"43245"
        "emu":"1234",
        "mis":[
            {
                "mi_cid":"90000000",
                "mi_desc":"1-1"
            }
        ]
    }
}

*/
class ag_emu_mi_table : public QObject
{
    Q_OBJECT
public:
    explicit ag_emu_mi_table(QObject *parent = nullptr);

    bool create_table(QSqlDatabase &m_database); //
    bool delete_table(QSqlDatabase &m_database); //

    void r_mi(QSqlDatabase &m_database,QString emu,QStringList &mi,QStringList &mi_desc);
    bool w_mi(QSqlDatabase &m_database,QString emu,const QJsonObject &w_data);
    void w_one_mi(QSqlDatabase &m_database,QString emu,QString mi,QString desc);

    bool del_one_mi_by_micid(QSqlDatabase &m_database,QString emu,QString mi);
    void del_mis_by_emucid(QSqlDatabase &m_database,QString emu);
    static const QString c_field_emu;
    static const QString c_field_mi;
    static const QString c_field_mi_desc;
private:
    QString m_name = "emu_mi_table";
signals:

};

#endif // AG_EMU_MI_TABLE_H
