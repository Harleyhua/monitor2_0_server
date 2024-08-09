#include "ag_emu_mi_table.h"
#include "mysql_table.h"

#include "QsLog.h"
const QString ag_emu_mi_table::c_field_emu = "emu";
const QString ag_emu_mi_table::c_field_mi = "mi_cid";
const QString ag_emu_mi_table::c_field_mi_desc = "mi_desc";

ag_emu_mi_table::ag_emu_mi_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_emu_mi_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString(
                "(%1 VARCHAR(30) NOT NULL,"
                "%2 VARCHAR(8) NOT NULL,"
                "%3 TEXT,"
                "primary key(%4,%5))")
            .arg(c_field_emu,c_field_mi,c_field_mi_desc,
                 c_field_emu,c_field_mi);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_emu_mi_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_emu_mi_table::r_mi(QSqlDatabase &m_database, QString emu, QStringList &mi, QStringList &mi_desc)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("SELECT %1,%2 FROM %3 WHERE %4='%5'")
            .arg(c_field_mi,c_field_mi_desc,m_name,c_field_emu,emu);
    if(query.exec(tmp_cmd))
    {
        while(query.next())
        {
            mi.append(query.value(c_field_mi).toString());
            mi_desc.append(query.value(c_field_mi_desc).toString());
        }
    }
    else
    {
        QLOG_WARN() << "读 网关_微逆关系表 失败";
    }
}

bool ag_emu_mi_table::w_mi(QSqlDatabase &m_database, QString emu,const QJsonObject &w_data)
{
    QSqlQuery query(m_database);
    QJsonArray tmp_datas = w_data.value("mis").toArray();
    bool b_ret = true;

    for(int i=0;i<tmp_datas.size();i++)
    {
        QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4) VALUES (?,?,?) ON DUPLICATE KEY UPDATE %5='%6'")
                .arg(m_name,c_field_emu,c_field_mi,c_field_mi_desc,
                     c_field_mi_desc,tmp_datas[i].toObject().value("mi_desc").toString());
        query.prepare(tmp_cmd);

        query.addBindValue(emu);
        query.addBindValue(tmp_datas[i].toObject().value("mi_cid").toString());
        query.addBindValue(tmp_datas[i].toObject().value("mi_desc").toString());

        if(query.exec())
        {

        }
        else
        {
            b_ret = false;
            QLOG_WARN() << "写 网关_微逆关系表 失败";
        }
    }

    return b_ret;
}

void ag_emu_mi_table::w_one_mi(QSqlDatabase &m_database, QString emu, QString mi, QString desc)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4) VALUES (?,?,?) ON DUPLICATE KEY UPDATE %5='%6'")
            .arg(m_name,c_field_emu,c_field_mi,c_field_mi_desc,
                 c_field_mi_desc,desc);
    query.prepare(tmp_cmd);

    query.addBindValue(emu);
    query.addBindValue(mi);
    query.addBindValue(desc);

    if(query.exec())
    {

    }
    else
    {
        QLOG_WARN() << "写 网关_微逆关系表 失败";
    }
}

bool ag_emu_mi_table::del_one_mi_by_micid(QSqlDatabase &m_database, QString emu, QString mi)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("DELETE FROM %1 WHERE %2='%3' AND %4='%5'")
            .arg(m_name,c_field_emu,emu,c_field_mi,mi);
    if(query.exec(tmp_cmd))
    {
        return true;
    }
    else
    {
        QLOG_WARN() << "删除 网关_微逆关系表 通过mi_cid 失败";
        return false;
    }
}

void ag_emu_mi_table::del_mis_by_emucid(QSqlDatabase &m_database, QString emu)
{
    QSqlQuery query(m_database);
    QString tmp_cmd = QString("DELETE FROM %1 WHERE %2='%3'")
            .arg(m_name,c_field_emu,emu);
    if(query.exec(tmp_cmd))
    {

    }
    else
    {
        QLOG_WARN() << "删除 网关_微逆关系表 通过emu_cid 失败";
    }
}



