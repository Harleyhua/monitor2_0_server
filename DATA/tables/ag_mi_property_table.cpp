#include "ag_mi_property_table.h"

#include "QsLog.h"
#include "mysql_table.h"

const QString ag_mi_property_table::c_field_cid = "mi_cid";
const QString ag_mi_property_table::c_field_nominal_power = "nominal_power";
const QString ag_mi_property_table::c_field_mim_version = "mim_version";
const QString ag_mi_property_table::c_field_mis_version = "mis_version";
const QString ag_mi_property_table::c_field_hard_version = "hard_version";
ag_mi_property_table::ag_mi_property_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_mi_property_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString(
                "(%1 VARCHAR(8) NOT NULL,"
                "%2 SMALLINT UNSIGNED,"
                "%3 VARCHAR(8) NOT NULL,"
                "%4 VARCHAR(8) NOT NULL,"
                "%5 VARCHAR(8) NOT NULL,"
                "primary key(%6),"
                "INDEX(%7),"
                "INDEX(%8),"
                "INDEX(%9))")
            .arg(c_field_cid,c_field_nominal_power,c_field_mim_version,c_field_mis_version,c_field_hard_version,
                 c_field_cid,c_field_nominal_power,c_field_mim_version,c_field_mis_version);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_mi_property_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_mi_property_table::write_property(QSqlDatabase &m_database, QJsonObject &w_data)
{
    QSqlQuery query(m_database);
    quint16 data_count = 0;

    data_count = w_data.value("datas").toArray().size();

    for(int i=0;i<data_count;i++)
    {
        QJsonObject tmp_property_data = w_data.value("datas").toArray()[i].toObject();
        QString tmp_cmd = QString("INSERT INTO %1 (%2,%3,%4,%5) values(?,?,?,?) ON DUPLICATE KEY UPDATE %6=%7,"
                                  "%8='%9',%10='%11'")
                .arg(m_name,c_field_cid,c_field_nominal_power,c_field_mim_version,c_field_mis_version,
                     c_field_nominal_power,QString::number(tmp_property_data.value(c_field_nominal_power).toInt(1000000)),
                     c_field_mim_version,tmp_property_data.value(c_field_mim_version).toString(""),
                     c_field_mis_version,tmp_property_data.value(c_field_mis_version).toString(""));

        query.prepare(tmp_cmd);
        query.addBindValue(tmp_property_data.value(c_field_cid).toString(""));
        query.addBindValue(tmp_property_data.value(c_field_nominal_power).toInt(1000000));
        query.addBindValue(tmp_property_data.value(c_field_mim_version).toString(""));
        query.addBindValue(tmp_property_data.value(c_field_mis_version).toString(""));

        if(query.exec())
        {
            //QLOG_INFO() << QString("插入微逆对象属性成功 %1").arg(tmp_property_data.value("room_id").toString(""));
        }
        else
        {
            QLOG_WARN() << QString("插入微逆对象属性失败 %1,%2,%3,%4")
                           .arg(tmp_property_data.value(c_field_cid).toString(""),
                                QString::number(tmp_property_data.value(c_field_nominal_power).toInt(1000000)),
                                tmp_property_data.value(c_field_mim_version).toString(""),
                                tmp_property_data.value(c_field_mis_version).toString(""));
        }
    }
}

void ag_mi_property_table::read_property(QSqlDatabase &m_database, QJsonObject &r_data, QJsonObject &data)
{
    QSqlQuery query(m_database);

    quint16 data_count = 0;
    QJsonArray datas;

    data_count = r_data.value("params").toObject().value("mi_cid").toArray().size();
    for(int i=0;i<data_count;i++)
    {
        QJsonObject one_mi;
        QString tmp_cmd = QString("SELECT %1,%2,%3,%4 FROM %5 WHERE %6='%7'")
                .arg(c_field_cid,c_field_nominal_power,c_field_mim_version,c_field_mis_version,m_name,
                     c_field_cid,r_data.value("params").toObject().value("mi_cid").toArray()[i].toString());

        query.prepare(tmp_cmd);
        if(query.exec())
        {
            if(query.next())
            {
                one_mi.insert(c_field_cid,query.value(c_field_cid).toString());
                one_mi.insert(c_field_nominal_power,query.value(c_field_nominal_power).toInt());
                one_mi.insert(c_field_mim_version,query.value(c_field_mim_version).toString());
                one_mi.insert(c_field_mis_version,query.value(c_field_mis_version).toString());

                datas.append(one_mi);

                //QLOG_INFO() << "读取微逆对象属性成功";
            }
        }
        else
        {
            QLOG_WARN() << "读取微逆对象属性失败";
        }
    }

    data.insert("datas",datas);
}

void ag_mi_property_table::read_property(QSqlDatabase &m_database, QString mi_cid, QJsonObject &data)
{
    QSqlQuery query(m_database);
    QJsonObject one_mi;
    QString tmp_cmd = QString("SELECT %1,%2,%3,%4 FROM %5 WHERE %6='%7'")
            .arg(c_field_cid,c_field_nominal_power,c_field_mim_version,
                 c_field_mis_version,m_name,c_field_cid,mi_cid);

    query.prepare(tmp_cmd);
    if(query.exec())
    {
        if(query.next())
        {
            one_mi.insert(c_field_cid,query.value(c_field_cid).toString());
            one_mi.insert(c_field_nominal_power,query.value(c_field_nominal_power).toInt());
            one_mi.insert(c_field_mim_version,query.value(c_field_mim_version).toString());
            one_mi.insert(c_field_mis_version,query.value(c_field_mis_version).toString());

            data.insert("datas",one_mi);

            //QLOG_INFO() << "读取微逆对象属性成功";
        }
    }
    else
    {
        QLOG_WARN() << "读取微逆对象属性失败";
    }
}

void ag_mi_property_table::read_nominal_power(QSqlDatabase &m_database, QStringList mi_cids, QHash<QString, QString> &data)
{
    QSqlQuery query(m_database);
    for(int i=0;i<mi_cids.size();i++)
    {
        QJsonObject one_mi;
        //添加版本号的读取Mis_version
        QString tmp_cmd = QString("SELECT %1,%2,%3,%4 FROM %5 WHERE %6='%7'")
                              .arg(c_field_cid,c_field_nominal_power,c_field_mim_version,c_field_mis_version,m_name,c_field_cid,mi_cids[i]);

        query.prepare(tmp_cmd);
        if(query.exec())
        {
            query.next();
            data.insert(query.value(c_field_cid).toString(),query.value(c_field_nominal_power).toString());
            data.insert(c_field_mim_version,query.value(c_field_mim_version).toString());
            data.insert(c_field_mis_version,query.value(c_field_mis_version).toString());
            //QLOG_INFO() << "读取微逆对象属性成功";
        }
        else
        {
            QLOG_WARN() << "读取微逆对象属性失败";
        }
    }
}

