#include "ag_pos_table.h"

#include "mysql_table.h"
#include "ag_emu_property_table.h"
#include "ag_mi_property_table.h"

#include "QsLog.h"
const QString ag_pos_table::c_field_station = "station";
const QString ag_pos_table::c_field_emu_cid = "emu_cid";
const QString ag_pos_table::c_field_mi_cid = "mi_cid";
const QString ag_pos_table::c_field_mi_desc = "mi_desc";
const QString ag_pos_table::c_field_emu_desc = "emu_desc";


ag_pos_table::ag_pos_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_pos_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =QString(
                "(%1 VARCHAR(30) NOT NULL,"
                "%2 VARCHAR(8) NOT NULL,"
                "%3 VARCHAR(8),"
                "%4 TEXT,"
                "%5 TEXT)")
            .arg(c_field_station,c_field_emu_cid,c_field_mi_cid,
                 c_field_emu_desc,c_field_mi_desc);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_pos_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_pos_table::write_mapping(QSqlDatabase &m_database, QJsonObject &w_data)
{
    QJsonArray tmp_data = w_data.value("datas").toArray();
    quint16 data_count = tmp_data.size();
    QString para = QString("(%1,%2,%3,%4,%5) values(?,?,?,?,?)")
            .arg(c_field_station,c_field_emu_cid,c_field_mi_cid,c_field_mi_desc);
    QSqlQuery query(m_database);

    for(int i=0;i<data_count;i++)
    {
        QString tmp_cmd = QString("INSERT INTO %1").arg(m_name) + para +
                QString("ON DUPLICATE KEY UPDATE %1='%2',%3='%4',%5='%6',%7='%8'")
                .arg(c_field_station,tmp_data[i].toObject().value(c_field_station).toString(),
                     c_field_emu_cid,tmp_data[i].toObject().value(c_field_emu_cid).toString(),
                     c_field_emu_desc,tmp_data[i].toObject().value(c_field_emu_desc).toString(),
                     c_field_mi_desc,tmp_data[i].toObject().value(c_field_mi_desc).toString());

        query.prepare(tmp_cmd);
        query.addBindValue(tmp_data[i].toObject().value(c_field_station).toString());
        query.addBindValue(tmp_data[i].toObject().value(c_field_emu_cid).toString());
        query.addBindValue(tmp_data[i].toObject().value(c_field_mi_cid).toString());
        query.addBindValue(tmp_data[i].toObject().value(c_field_emu_desc).toString());
        query.addBindValue(tmp_data[i].toObject().value(c_field_mi_desc).toString());

        QString tmp_log = tmp_data[i].toObject().value(c_field_station).toString() +
                tmp_data[i].toObject().value(c_field_emu_cid).toString() +
                tmp_data[i].toObject().value(c_field_mi_cid).toString() +
                tmp_data[i].toObject().value(c_field_emu_desc).toString() +
                tmp_data[i].toObject().value(c_field_mi_desc).toString() +
                m_name;
        if(query.exec())
        {
            QLOG_INFO() << QString("老化架微逆映射:%1-%2-%3 %4 写入数据表成功: ") + tmp_log;
        }
        else
        {
            QLOG_WARN() << QString("老化架微逆映射:%1-%2-%3 %4 写入数据表失败: ") + tmp_log;
        }
    }

}

void ag_pos_table::read_mapping(QSqlDatabase &m_database, QJsonObject &r_data, QJsonObject &data)
{
    QJsonObject tmp_param = r_data.value("params").toObject();
    QString select_cmd_head = QString("SELECT %1,%2,%3,%4,%5 FROM %6 ")
            .arg(c_field_station,c_field_emu_cid,c_field_mi_cid,c_field_emu_desc,c_field_mi_desc,m_name);

    QSqlQuery query(m_database);
    bool where_flag = false;
    QString mid_cmd;

    if(tmp_param.value(c_field_station).toString() != "")
    {
        if(!where_flag)
        {
            where_flag = true;
            select_cmd_head += " WHERE ";
        }
        else if(where_flag)
        {
            mid_cmd += " AND ";
        }
        mid_cmd += c_field_station + QString("='%1' ").arg(tmp_param.value(c_field_station).toString());
        where_flag = true;
    }
    else
    {
        return;
    }
//    if(tmp_param.value(c_field_emu_cid).toString() != "")
//    {
//        if(!where_flag)
//        {
//            where_flag = true;
//            select_cmd_head += " WHERE ";
//        }
//        else if(where_flag)
//        {
//            mid_cmd += " AND ";
//        }
//        mid_cmd += c_field_emu_cid + QString("='%1' ").arg(tmp_param.value(c_field_emu_cid).toString());
//        where_flag = true;
//    }

    mid_cmd += QString("ORDER BY %1 DESC").arg(c_field_emu_cid);

    if(query.exec(select_cmd_head + mid_cmd))
    {
        QJsonArray device_array;
        QString old_emu_name = "";
        QString new_emu_name = "";
        while (query.next()) {
            QJsonObject emu_data_js;
            QJsonArray mi_data_array;

            if(new_emu_name == old_emu_name)
            {
                QJsonObject mi_data_js;
                mi_data_js.insert(c_field_mi_cid,query.value(c_field_mi_cid).toString());
                mi_data_js.insert(c_field_mi_desc,query.value(c_field_mi_desc).toString());
                mi_data_array.append(mi_data_js);
            }
            else
            {
                old_emu_name = new_emu_name;
                emu_data_js.insert(c_field_emu_cid,old_emu_name);
                emu_data_js.insert("mis",mi_data_array);
                device_array.append(emu_data_js);
            }
            new_emu_name = query.value(c_field_emu_cid).toString();
        }

        //QJsonObject datas;
        data.insert(c_field_station,query.value(c_field_station).toString());
        data.insert("device",device_array);


        //data.insert("datas",datas);
        QLOG_INFO() << "查询老化架 微逆映射关系成功";
    }
    else
    {
        QLOG_WARN() << "查询老化架 微逆映射关系失败";
    }

}

void ag_pos_table::read_mapping(QSqlDatabase &m_database, QString station, QJsonObject &data, QStringList &mi_list)
{
    QString select_cmd_head = QString("SELECT %1,%2,%3,%4,%5 FROM %6 ")
            .arg(c_field_station,c_field_emu_cid,c_field_mi_cid,c_field_emu_desc,c_field_mi_desc,m_name);

    QSqlQuery query(m_database);
    bool where_flag = false;
    QString mid_cmd;
    //QStringList emu_list;
    //QStringList mi_list;

    if(station != "")
    {
        if(!where_flag)
        {
            where_flag = true;
            select_cmd_head += " WHERE ";
        }
        else if(where_flag)
        {
            mid_cmd += " AND ";
        }
        mid_cmd += c_field_station + QString("='%1' ").arg(station);
        where_flag = true;
    }
    else
    {
        return ;
    }
//    if(tmp_param.value(c_field_emu_cid).toString() != "")
//    {
//        if(!where_flag)
//        {
//            where_flag = true;
//            select_cmd_head += " WHERE ";
//        }
//        else if(where_flag)
//        {
//            mid_cmd += " AND ";
//        }
//        mid_cmd += c_field_emu_cid + QString("='%1' ").arg(tmp_param.value(c_field_emu_cid).toString());
//        where_flag = true;
//    }

    mid_cmd += QString("ORDER BY %1,%2 ASC").arg(c_field_emu_desc,c_field_mi_desc);

    if(query.exec(select_cmd_head + mid_cmd))
    {
        QJsonArray device_array;
        QString old_emu_desc = "";
        QString old_emu_name = "";
        QString new_emu_name = "";
        QJsonArray mi_data_array;

        for(int i=0;i<query.size();i++)
        {
            QJsonObject emu_data_js;
            QJsonObject mi_data_js;
            ag_emu_property_table tmp_emu_pty_tb;
            ag_mi_property_table tmp_mi_pty_tb;

            query.next();

            if(i != 0)
               new_emu_name = query.value(c_field_emu_cid).toString();

            if(new_emu_name == old_emu_name)
            {
                {
                    QJsonObject mi_pty_obj;
                    tmp_mi_pty_tb.read_property(m_database,query.value(c_field_mi_cid).toString(),mi_pty_obj);
                    mi_data_js.insert(ag_mi_property_table::c_field_nominal_power,
                                      mi_pty_obj.value("datas").toObject().value(ag_mi_property_table::c_field_nominal_power).toString());
                    mi_data_js.insert(ag_mi_property_table::c_field_mim_version,
                                      mi_pty_obj.value("datas").toObject().value(ag_mi_property_table::c_field_mim_version).toString());
                    mi_data_js.insert(ag_mi_property_table::c_field_mis_version,
                                      mi_pty_obj.value("datas").toObject().value(ag_mi_property_table::c_field_mis_version).toString());

                }
                mi_data_js.insert(c_field_mi_cid,query.value(c_field_mi_cid).toString());
                mi_data_js.insert(c_field_mi_desc,query.value(c_field_mi_desc).toString());

                mi_list.append(query.value(c_field_mi_cid).toString());

                mi_data_array.append(mi_data_js);
            }
            else
            {
                {
                    QJsonObject emu_pty_obj;
                    tmp_emu_pty_tb.read_property(m_database,old_emu_name,emu_pty_obj);
                    emu_data_js.insert(ag_emu_property_table::c_field_type,
                                      emu_pty_obj.value("datas").toObject().value(ag_emu_property_table::c_field_type).toString());
                    emu_data_js.insert(ag_emu_property_table::c_field_hard_version,
                                      emu_pty_obj.value("datas").toObject().value(ag_emu_property_table::c_field_hard_version).toString());
                    emu_data_js.insert(ag_emu_property_table::c_field_soft_version,
                                      emu_pty_obj.value("datas").toObject().value(ag_emu_property_table::c_field_soft_version).toString());
                }

                emu_data_js.insert(c_field_emu_cid,old_emu_name);
                emu_data_js.insert(c_field_emu_desc,old_emu_desc);
                emu_data_js.insert("mis",mi_data_array);
                device_array.append(emu_data_js);

                mi_data_array = QJsonArray();

                {
                    QJsonObject mi_pty_obj;
                    tmp_mi_pty_tb.read_property(m_database,query.value(c_field_mi_cid).toString(),mi_pty_obj);
                    mi_data_js.insert(ag_mi_property_table::c_field_nominal_power,
                                      mi_pty_obj.value("datas").toObject().value(ag_mi_property_table::c_field_nominal_power).toString());
                    mi_data_js.insert(ag_mi_property_table::c_field_mim_version,
                                      mi_pty_obj.value("datas").toObject().value(ag_mi_property_table::c_field_mim_version).toString());
                    mi_data_js.insert(ag_mi_property_table::c_field_mis_version,
                                      mi_pty_obj.value("datas").toObject().value(ag_mi_property_table::c_field_mis_version).toString());
                }
                mi_data_js.insert(c_field_mi_cid,query.value(c_field_mi_cid).toString());
                mi_data_js.insert(c_field_mi_desc,query.value(c_field_mi_desc).toString());

                mi_list.append(query.value(c_field_mi_cid).toString());

                mi_data_array.append(mi_data_js);

            }
//            new_emu_name = query.value(c_field_emu_cid).toString();
//            old_emu_name = new_emu_name;
            old_emu_name = query.value(c_field_emu_cid).toString();
            old_emu_desc = query.value(c_field_emu_desc).toString();

            if(i == query.size()-1)
            {
                {
                    QJsonObject emu_pty_obj;
                    tmp_emu_pty_tb.read_property(m_database,old_emu_name,emu_pty_obj);
                    emu_data_js.insert(ag_emu_property_table::c_field_type,
                                      emu_pty_obj.value("datas").toObject().value(ag_emu_property_table::c_field_type).toString());
                    emu_data_js.insert(ag_emu_property_table::c_field_hard_version,
                                      emu_pty_obj.value("datas").toObject().value(ag_emu_property_table::c_field_hard_version).toString());
                    emu_data_js.insert(ag_emu_property_table::c_field_soft_version,
                                      emu_pty_obj.value("datas").toObject().value(ag_emu_property_table::c_field_soft_version).toString());
                }

                emu_data_js.insert(c_field_emu_cid,old_emu_name);
                emu_data_js.insert(c_field_emu_desc,old_emu_desc);
                emu_data_js.insert("mis",mi_data_array);
                device_array.append(emu_data_js);
            }
        }

        data.insert(c_field_station,station);
        data.insert("device",device_array);


        //data.insert("datas",datas);
        QLOG_INFO() << "查询老化架 微逆映射关系成功";
    }
    else
    {
        QLOG_WARN() << "查询老化架 微逆映射关系失败";
    }
}



