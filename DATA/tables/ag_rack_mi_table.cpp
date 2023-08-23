#include "ag_rack_mi_table.h"

#include "QsLog.h"
#include "mysql_table.h"

const QString ag_rack_mi_table::c_field_room = "room_id";
const QString ag_rack_mi_table::c_field_rack = "rack_id";
const QString ag_rack_mi_table::c_field_pos = "pos";
const QString ag_rack_mi_table::c_field_emu_cid = "emu_cid";
const QString ag_rack_mi_table::c_field_mi_cid = "mi_cid";
const QString ag_rack_mi_table::c_field_pv_id = "mi_pvid";
const QString ag_rack_mi_table::c_field_dc_eff_k = "dc_eff_k";
const QString ag_rack_mi_table::c_field_dc_eff_b = "dc_eff_b";
const QString ag_rack_mi_table::c_field_ac_eff_k = "ac_eff_k";
const QString ag_rack_mi_table::c_field_ac_eff_b = "ac_eff_b";

ag_rack_mi_table::ag_rack_mi_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_rack_mi_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field = QString("(%1 VARCHAR(8) NOT NULL,"
                                "%2 VARCHAR(8) NOT NULL,"
                                "%3 VARCHAR(8) NOT NULL,"
                                "%4 VARCHAR(8),"
                                "%5 VARCHAR(8),"
                                "%6 TINYINT UNSIGNED ,"
                                "%7 FLOAT ,"
                                "%8 FLOAT ,"
                                "%9 FLOAT ,"
                                "%10 FLOAT ,"
                                "primary key(%11,%12,%13),"
                                "INDEX(%14),"
                                "INDEX(%15),"
                                "INDEX(%16),"
                                "INDEX(%17),"
                                "INDEX(%18),"
                                "INDEX(%19),"
                                "INDEX(%20))")
            .arg(c_field_room,c_field_rack,c_field_pos,c_field_emu_cid,c_field_mi_cid,c_field_pv_id,c_field_dc_eff_k,
                 c_field_dc_eff_b,c_field_ac_eff_k,c_field_ac_eff_b,c_field_room,c_field_rack,c_field_pos,c_field_dc_eff_k,
                 c_field_dc_eff_b,c_field_ac_eff_k,c_field_ac_eff_b,c_field_emu_cid,c_field_mi_cid,c_field_pv_id);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_rack_mi_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

bool ag_rack_mi_table::read_eff_factor(QSqlDatabase &m_database, QJsonObject &r_data, QJsonObject &data)
{
    QJsonObject param_obj = r_data.value("params").toObject();

    QSqlQuery query = QSqlQuery(m_database);
    QString cmd = QString("SELECT DISTINCT %1,%2,%3,%4,%5 FROM %6 WHERE %7='%8' AND %9='%10' AND %11='%12'")
            .arg(c_field_pos,c_field_dc_eff_k,c_field_dc_eff_b,c_field_ac_eff_k,c_field_ac_eff_b,m_name,
                 c_field_room,param_obj.value(c_field_room).toString(),
                 c_field_rack,param_obj.value(c_field_rack).toString(),
                 c_field_pos,param_obj.value(c_field_pos).toString());
    if(query.exec(cmd))
    {
        QJsonObject data_obj;
        if(query.next())
        {
            data_obj.insert(c_field_room,query.value(c_field_room).toString());
            data_obj.insert(c_field_rack,query.value(c_field_rack).toString());
            data_obj.insert(c_field_pos,query.value(c_field_pos).toString());
            data_obj.insert(c_field_mi_cid,query.value(c_field_mi_cid).toString());
            data_obj.insert(c_field_dc_eff_k,query.value(c_field_dc_eff_k).toString());
            data_obj.insert(c_field_dc_eff_b,query.value(c_field_dc_eff_b).toString());
            data_obj.insert(c_field_ac_eff_k,query.value(c_field_ac_eff_k).toString());
            data_obj.insert(c_field_ac_eff_b,query.value(c_field_ac_eff_b).toString());

            data.insert("datas",data_obj);
            return true;
        }
    }
    return false;
}

void ag_rack_mi_table::write_mapping(QSqlDatabase &m_database, QJsonObject &w_data)
{
    QJsonArray tmp_data = w_data.value("datas").toArray();
    quint16 data_count = tmp_data.size();
    QString para = QString("(%1,%2,%3,%4,%5) values(?,?,?,?,?)")
            .arg(c_field_room,c_field_rack,c_field_pos,c_field_mi_cid,c_field_pv_id);
    QSqlQuery query(m_database);

    for(int i=0;i<data_count;i++)
    {
        QString tmp_cmd = QString("INSERT INTO %1").arg(m_name) + para +
                QString("ON DUPLICATE KEY UPDATE %1='%2',%3=%4")
                .arg(c_field_mi_cid,tmp_data[i].toObject().value(c_field_mi_cid).toString(),
                     c_field_pv_id).arg(tmp_data[i].toObject().value(c_field_pv_id).toInt());

        query.prepare(tmp_cmd);
        query.addBindValue(tmp_data[i].toObject().value(c_field_room).toString());
        query.addBindValue(tmp_data[i].toObject().value(c_field_rack).toString());
        query.addBindValue(tmp_data[i].toObject().value(c_field_pos).toString());
        query.addBindValue(tmp_data[i].toObject().value(c_field_mi_cid).toString());
        query.addBindValue(tmp_data[i].toObject().value(c_field_pv_id).toInt());

        QString tmp_log = tmp_data[i].toObject().value(c_field_room).toString() +
                tmp_data[i].toObject().value(c_field_rack).toString() +
                tmp_data[i].toObject().value(c_field_pos).toString() +
                tmp_data[i].toObject().value(c_field_mi_cid).toString() +
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

void ag_rack_mi_table::read_mapping(QSqlDatabase &m_database, QJsonObject &r_data, QJsonObject &data)
{
    QJsonObject tmp_param = r_data.value("params").toObject();
    QString select_cmd_head = QString("SELECT %1,%2,%3,%4 FROM %5 ")
            .arg(c_field_room,c_field_rack,c_field_pos,c_field_mi_cid,m_name);

    QSqlQuery query(m_database);
    bool where_flag = false;
    QString mid_cmd;

//    if(tmp_param.value("room_id").toString() != "")
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
//        mid_cmd += c_field_room + QString("='%1' ").arg(tmp_param.value("room_id").toString());
//        where_flag = true;
//    }

    if(tmp_param.value("emu_cid").toString() != "")
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
        mid_cmd += c_field_emu_cid + QString("='%1' ").arg(tmp_param.value("emu_cid").toString());
        where_flag = true;
    }

    if(query.exec(select_cmd_head + mid_cmd))
    {
        int i=0;
        QJsonArray pv_data_array;
        while (query.next()) {
            QJsonObject pv_data_js;
            pv_data_js.insert(c_field_room,query.value(c_field_room).toString());
            pv_data_js.insert(c_field_rack,query.value(c_field_rack).toString());
            pv_data_js.insert(c_field_pos,query.value(c_field_pos).toString());
            pv_data_js.insert(c_field_mi_cid,query.value(c_field_mi_cid).toString());
            pv_data_array.insert(i,pv_data_js);
            ++i;
        }
        data.insert("datas",pv_data_array);
        QLOG_INFO() << "查询老化架 微逆映射关系成功";
    }
    else
    {
        QLOG_WARN() << "查询老化架 微逆映射关系失败";
    }
}


