#include "ag_rack_data_table.h"


#include "QsLog.h"
#include "mysql_table.h"
#include "ag_rack_index_table.h"
#include "ag_rack_extra_data_table.h"
#include <QSqlError>

const QString ag_rack_data_table::c_field_id = "id";
const QString ag_rack_data_table::c_field_room_id = "room_id";
const QString ag_rack_data_table::c_field_rack_id = "rack_id";
const QString ag_rack_data_table::c_field_leakage_status = "leakage_status";
const QString ag_rack_data_table::c_field_mi_pos = "mi_pos";
const QString ag_rack_data_table::c_field_mi_cid = "mi_cid";
const QString ag_rack_data_table::c_field_dc_volt_in = "dc_volt_in";
const QString ag_rack_data_table::c_field_dc_current_in = "dc_current_in";
const QString ag_rack_data_table::c_field_dc_volt_eff_k = "dc_volt_eff_k";
const QString ag_rack_data_table::c_field_dc_volt_eff_b = "dc_volt_eff_b";
const QString ag_rack_data_table::c_field_dc_power_in = "dc_power_in";
const QString ag_rack_data_table::c_field_ac_volt_out = "ac_volt_out";
const QString ag_rack_data_table::c_field_ac_current_out = "ac_current_out";
const QString ag_rack_data_table::c_field_ac_volt_eff_k = "ac_volt_eff_k";
const QString ag_rack_data_table::c_field_ac_volt_eff_b = "ac_volt_eff_b";
const QString ag_rack_data_table::c_field_ac_power_out = "ac_power_out";
const QString ag_rack_data_table::c_field_efficiency = "efficiency";
const QString ag_rack_data_table::c_field_start_time = "start_time";
const QString ag_rack_data_table::c_field_cur_date = "cur_date";

const QString ag_rack_data_table::c_field_room = "room";







ag_rack_data_table::ag_rack_data_table(QString name,QObject *parent)
    : QObject{parent}
{
    m_name = name;
}

ag_rack_data_table::ag_rack_data_table(QObject *parent)
{

}

bool ag_rack_data_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field = QString("(%1 INT UNSIGNED NOT NULL AUTO_INCREMENT primary key,"
                                "%2 VARCHAR(8) NOT NULL,"
                                "%3 VARCHAR(8) NOT NULL,"
                                "%4 VARCHAR(8) NOT NULL,"
                                "%5 VARCHAR(8) NOT NULL,"
                                "%6 VARCHAR(8) NOT NULL,"
                                "%7 FLOAT NOT NULL,"
                                "%8 FLOAT NOT NULL,"
                                "%9 FLOAT NOT NULL,"
                                "%10 FLOAT NOT NULL,"
                                "%11 FLOAT NOT NULL,"
                                "%12 FLOAT NOT NULL,"
                                "%13 FLOAT NOT NULL,"
                                "%14 FLOAT NOT NULL,"
                                "%15 FLOAT NOT NULL,"
                                "%16 FLOAT NOT NULL,"
                                "%17 FLOAT NOT NULL,"
                                "%18 DATETIME NOT NULL,"
                                "%19 DATETIME NOT NULL,"
                                "INDEX(%20))")
            .arg(c_field_id,c_field_room_id,c_field_rack_id,c_field_leakage_status,c_field_mi_pos,c_field_mi_cid,
                 c_field_dc_volt_in,c_field_dc_current_in,c_field_dc_volt_eff_k,c_field_dc_volt_eff_b,
                 c_field_dc_power_in,c_field_ac_volt_out,c_field_ac_current_out,c_field_ac_volt_eff_k,
                 c_field_ac_volt_eff_b,c_field_ac_power_out,c_field_efficiency,c_field_start_time,
                 c_field_cur_date,c_field_mi_cid);
    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_rack_data_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_rack_data_table::read_size(QSqlDatabase &m_database, QJsonObject &r_data, quint64 &size)
{
    QString select_cmd_head = QString("SELECT COUNT(1) FROM ") + m_name + " ";
    QSqlQuery query(m_database);
    bool where_flag = false;
    //如果要进行筛选 微逆编号
    QString mid_cmd;
    if(r_data.value("mi_cid_flag").toBool())
    {
        if(!where_flag)
        {
            where_flag = true;
            select_cmd_head += " WHERE ";
        }
        QJsonArray mi_array = r_data.value("mi_cid").toArray();
        for(int i=0;i<mi_array.size();i++)
        {
            mid_cmd += c_field_mi_cid + QString("='%1' OR ").arg(mi_array[i].toString());
        }

        if(mi_array.size() >0)
        {
            mid_cmd.remove(-1,3);
        }
    }

    if(r_data.value("room_id").toString() != "")
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
        mid_cmd += c_field_room_id + QString("='%1' ").arg(r_data.value("room_id").toString());
        where_flag = true;
    }

    if(r_data.value("rack_id").toString() != "")
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
        mid_cmd += c_field_rack_id + QString("='%1' ").arg(r_data.value("rack_id").toString());
        where_flag = true;
    }

    if(r_data.value("mi_pos").toString() != "")
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
        mid_cmd += c_field_mi_pos + QString("='%1' ").arg(r_data.value("mi_pos").toString());
        where_flag = true;
    }

    if(!where_flag)
    {
        where_flag = true;
        select_cmd_head += " WHERE ";
    }
    else if(where_flag)
    {
        mid_cmd += " AND ";
    }

    mid_cmd += QString(" %1 BETWEEN '%2' AND '%3' ").arg(c_field_cur_date,r_data.value("start_date").toString(),
                                                      r_data.value("stop_date").toString());

    if(query.exec(select_cmd_head + mid_cmd))
    {
        if(query.next())
            size = query.value("COUNT(1)").toUInt();
    }

}

void ag_rack_data_table::write_data(QSqlDatabase &m_database,const QJsonObject &w_data)
{
    QJsonArray tmp_datas = w_data.value("datas").toArray();
    QString cmd = QString("(%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)")
            .arg(c_field_room_id,c_field_rack_id,c_field_leakage_status,c_field_mi_cid,c_field_mi_pos,
                 c_field_dc_volt_in,c_field_dc_current_in,c_field_dc_volt_eff_k,c_field_dc_volt_eff_b,
                 c_field_dc_power_in,c_field_ac_volt_out,c_field_ac_current_out,c_field_ac_volt_eff_k,
                 c_field_ac_volt_eff_b,c_field_ac_power_out,c_field_efficiency,c_field_start_time,
                 c_field_cur_date);
    QString head = QString("INSERT INTO %1 ").arg(m_name);
    QSqlQuery query(m_database);

    QVariantList record[18];

    for(int i=0;i<tmp_datas.size();i++)
    {
        QJsonObject rack_obj = tmp_datas[i].toObject();

//        QJsonObject rack_eff_obj;
//        QJsonObject params_obj;
//        QJsonObject eff_data;
//        ag_rack_mi_table tmp_tb;

//        params_obj.insert(c_field_room_id,rack_obj.value(c_field_room_id).toString());
//        params_obj.insert(c_field_rack_id,rack_obj.value(c_field_rack_id).toString());
//        params_obj.insert(c_field_mi_pos,rack_obj.value(c_field_mi_pos).toString());
//        rack_eff_obj.insert("params",params_obj);
//        if(tmp_tb.read_eff_factor(m_database,rack_eff_obj,eff_data))
        {
//            record[7] << eff_data.value("datas").toObject().value(c_field_dc_volt_eff_k).toDouble();
//            record[8] << eff_data.value("datas").toObject().value(c_field_dc_volt_eff_b).toDouble();
//            record[12] << eff_data.value("datas").toObject().value(c_field_ac_volt_eff_k).toDouble();
//            record[13] << eff_data.value("datas").toObject().value(c_field_ac_volt_eff_b).toDouble();
        }
//        else
//        {
//            record[7] << 1;
//            record[8] << 0;
//            record[12] << 1;
//            record[13] << 0;
//        }

        record[0] << rack_obj.value(c_field_room_id).toString();
        record[1] << rack_obj.value(c_field_rack_id).toString();
        record[2] << rack_obj.value(c_field_leakage_status).toString();
        record[3] << rack_obj.value(c_field_mi_cid).toString();
        record[4] << rack_obj.value(c_field_mi_pos).toString();
        record[5] << rack_obj.value(c_field_dc_volt_in).toDouble();
        record[6] << rack_obj.value(c_field_dc_current_in).toDouble();

        record[7] << rack_obj.value(c_field_dc_volt_eff_k).toDouble();
        record[8] << rack_obj.value(c_field_dc_volt_eff_b).toDouble();

        record[9] << rack_obj.value(c_field_dc_power_in).toDouble();
        record[10] << rack_obj.value(c_field_ac_volt_out).toDouble();
        record[11] << rack_obj.value(c_field_ac_current_out).toDouble();

        record[12] << rack_obj.value(c_field_ac_volt_eff_k).toDouble();
        record[13] << rack_obj.value(c_field_ac_volt_eff_b).toDouble();

        record[14] << rack_obj.value(c_field_ac_power_out).toDouble();
        record[15] << rack_obj.value(c_field_efficiency).toDouble();
        record[16] << rack_obj.value(c_field_start_time).toString();
        record[17] << rack_obj.value(c_field_cur_date).toString();
    }

    query.prepare(head + cmd);
    query.addBindValue(record[0]);
    query.addBindValue(record[1]);
    query.addBindValue(record[2]);
    query.addBindValue(record[3]);
    query.addBindValue(record[4]);
    query.addBindValue(record[5]);
    query.addBindValue(record[6]);
    query.addBindValue(record[7]);
    query.addBindValue(record[8]);
    query.addBindValue(record[9]);
    query.addBindValue(record[10]);
    query.addBindValue(record[11]);
    query.addBindValue(record[12]);
    query.addBindValue(record[13]);
    query.addBindValue(record[14]);
    query.addBindValue(record[15]);
    query.addBindValue(record[16]);
    query.addBindValue(record[17]);

    if(query.execBatch())
    {
        QLOG_INFO() << QString("老化架:写入数据表成功");
    }
    else {
        QLOG_WARN() << query.lastQuery() << query.lastError().text();

        QLOG_WARN() << QString("老化架:写入数据表失败: ");
    }
}

bool ag_rack_data_table::read_data(QSqlDatabase &m_database,const QJsonObject &r_data, QJsonObject &data)
{
    QString select_cmd_head = QString("SELECT %1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18 FROM ")
            .arg(c_field_room_id,c_field_rack_id,c_field_leakage_status,c_field_mi_pos,c_field_mi_cid,
                 c_field_dc_volt_in,c_field_dc_current_in,c_field_dc_volt_eff_k,c_field_dc_volt_eff_b,
                 c_field_dc_power_in,c_field_ac_volt_out,c_field_ac_current_out,c_field_ac_volt_eff_k,
                 c_field_ac_volt_eff_b,c_field_ac_power_out,c_field_efficiency,c_field_start_time,
                 c_field_cur_date) + m_name + " ";
    QString select_size_head = QString("SELECT COUNT(1) FROM ");

    QString condition;

    QSqlQuery query(m_database);
    bool where_flag = false;
    //如果要进行筛选 微逆编号
    QString rack_cmd;
    QString rack_size_cmd;

    QStringList mi_list;
    QStringList table_list;
    ag_rack_index_table rk_tb;
    QJsonArray mi_array = r_data.value(c_field_mi_cid).toArray();

    for(int i=0;i<mi_array.size();i++)
    {
        mi_list.append(mi_array[i].toString());
    }

    table_list = rk_tb.read_tablelist_from_mi(m_database,mi_list);

    if(mi_array.size() != 0)
    {
        if(!where_flag)
        {
            where_flag = true;
            condition += " WHERE ";
        }
        else
        {
            condition += " OR ";

        }
        for(int i=0;i<mi_array.size();i++)
        {
            condition += c_field_mi_cid + QString(" ='%1' ").arg(mi_array[i].toString());

            if(i != (mi_array.size()-1))
                condition += " OR ";
            //where_flag = true;
        }
    }


    if(r_data.value("room_id").toString() != "")
    {
        if(!where_flag)
        {
            where_flag = true;
            condition += " WHERE ";
        }
        else
        {
            condition += " AND ";
        }
        condition += c_field_room_id + QString("='%1' ").arg(r_data.value("room_id").toString());
        //where_flag = true;
    }


    if(r_data.value("rack_id").toString() != "")
    {
        if(!where_flag)
        {
            where_flag = true;
            condition += " WHERE ";
        }
        else
        {
            condition += " AND ";
        }
        condition += c_field_rack_id + QString("='%1' ").arg(r_data.value("rack_id").toString());
        //where_flag = true;
    }

    if(r_data.value("mi_pos").toString() != "")
    {
        if(!where_flag)
        {
            where_flag = true;
            condition += " WHERE ";
        }
        else
        {
            condition += " AND ";
        }
        condition += c_field_mi_pos + QString("='%1' ").arg(r_data.value("mi_pos").toString());
        //where_flag = true;
    }

    if(r_data.value("start_date").toString() != "" && r_data.value("stop_date").toString() != "")
    {
        if(!where_flag)
        {
            where_flag = true;
            condition += " WHERE ";
        }
        else if(where_flag)
        {
            condition += " AND ";
        }

        condition += QString(" %1 BETWEEN '%2' AND '%3' ").arg(c_field_cur_date,r_data.value("start_date").toString(),
                                                          r_data.value("stop_date").toString());
    }

    for(int i=0;i<table_list.size();i++)
    {
        if(i !=0)
        {
            rack_cmd.append(" UNION ALL ");
            rack_size_cmd.append(" UNION ALL ");
        }
        rack_cmd.append(select_cmd_head);
        rack_cmd.append(table_list[i]);
        rack_cmd.append(condition);

        rack_size_cmd.append(select_size_head);
        rack_size_cmd.append(table_list[i]);
        rack_size_cmd.append(condition);
    }

    rack_cmd += QString("ORDER BY %1 DESC").arg(c_field_cur_date);
    if(r_data.value("nums").toString() != "-1")
    {
        rack_cmd += QString(" LIMIT %1,%2 ").arg(r_data.value("start_num").toString(),
                                                 r_data.value("nums").toString());
    }

    uint64_t data_size = 0;
    if(query.exec(rack_size_cmd))
    {
        while (query.next()) {
            data_size += query.value("COUNT(1)").toUInt();
        }
    }

    data.insert("all_size",QString::number(data_size));

    if(query.exec(rack_cmd))
    {
        QJsonArray pv_data_array;
        while (query.next())
        {
            QJsonObject pv_data_js;
            pv_data_js.insert(c_field_room_id,query.value(c_field_room_id).toString());
            pv_data_js.insert(c_field_rack_id,query.value(c_field_rack_id).toString());
            pv_data_js.insert(c_field_leakage_status,query.value(c_field_leakage_status).toString());
            pv_data_js.insert(c_field_mi_pos,query.value(c_field_mi_pos).toString());
            pv_data_js.insert(c_field_mi_cid,query.value(c_field_mi_cid).toString());
            pv_data_js.insert(c_field_dc_volt_in,QString::number(query.value(c_field_dc_volt_in).toFloat(),'f',2));
            pv_data_js.insert(c_field_dc_current_in,QString::number(query.value(c_field_dc_current_in).toFloat(),'f',2));
            pv_data_js.insert(c_field_dc_volt_eff_k,QString::number(query.value(c_field_dc_volt_eff_k).toFloat(),'f',3));
            pv_data_js.insert(c_field_dc_volt_eff_b,QString::number(query.value(c_field_dc_volt_eff_b).toFloat(),'f',3));
            pv_data_js.insert(c_field_dc_power_in,QString::number(query.value(c_field_dc_power_in).toFloat(),'f',2));
            pv_data_js.insert(c_field_ac_volt_out,QString::number(query.value(c_field_ac_volt_out).toFloat(),'f',2));
            pv_data_js.insert(c_field_ac_current_out,QString::number(query.value(c_field_ac_current_out).toFloat(),'f',2));
            pv_data_js.insert(c_field_ac_volt_eff_k,QString::number(query.value(c_field_ac_volt_eff_k).toFloat(),'f',3));
            pv_data_js.insert(c_field_ac_volt_eff_b,QString::number(query.value(c_field_ac_volt_eff_b).toFloat(),'f',3));
            pv_data_js.insert(c_field_ac_power_out,QString::number(query.value(c_field_ac_power_out).toFloat(),'f',2));
            pv_data_js.insert(c_field_efficiency,QString::number(query.value(c_field_efficiency).toFloat(),'f',3));
            pv_data_js.insert(c_field_start_time,query.value(c_field_start_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            pv_data_js.insert(c_field_cur_date,query.value(c_field_cur_date).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

            pv_data_array.append(pv_data_js);
        }

        data.insert("datas",pv_data_array);
        return true;
        //QLOG_INFO() << "查询 老化架数据成功";
    }
    QLOG_WARN() << "查询 老化架数据失败";
    return false;
}

//数据库读取批次数据
void ag_rack_data_table::read_batch_list(QSqlDatabase &m_database, QString start_time, QString stop_time,QStringList &batch_list)
{
    QSqlQuery query(m_database);
    QString head_cmd = QString("SELECT DISTINCT %1 FROM ").arg(c_field_start_time);
    QString tail_cmd = QString(" WHERE %1>='%2' AND %3<='%4'").arg(c_field_start_time,start_time,c_field_start_time,stop_time);
    ag_rack_index_table rk_idx_tb;

    //调用索引表的成员函数，根据时间范围读取表列表
    QStringList table_list = rk_idx_tb.read_tablelist_from_time(m_database,start_time,stop_time);
    QString cmd;
    for(int i=0;i<table_list.size();i++)
    {
        if(i!=0)
        {
           cmd.append(" UNION ALL ");
        }
        cmd.append(head_cmd); //添加查询头部命令
        cmd.append(table_list[i]); //添加表名
        cmd.append(tail_cmd); //添加查询尾部命令，包括时间过滤
    }
    //添加ORDER BY子句，按开始时间降序排序结果
    cmd.append(QString(" ORDER BY %1 DESC").arg(c_field_start_time));

    if(query.exec(cmd))
    {
        while(query.next())
        {
            batch_list.append(query.value(c_field_start_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        }
    }

}

void ag_rack_data_table::read_mi_list(QSqlDatabase &m_database, QString start_time, QStringList &mi_list)
{
    ag_rack_index_table rk_idx_tb;
    QSqlQuery query(m_database);
    QStringList table_list = rk_idx_tb.read_tablelist_from_time(m_database,start_time);

    QString head_cmd = QString("SELECT DISTINCT %1 FROM ").arg(c_field_mi_cid);
    QString tail_cmd = QString(" WHERE %1='%2' ").arg(c_field_start_time,start_time);
    QString cmd;
    for(int i=0;i<table_list.size();i++)
    {
        if(i != 0)
        {
            cmd.append(" UNION ALL ");
        }
        cmd.append(head_cmd);
        cmd.append(table_list[i]);
        cmd.append(tail_cmd);
    }

    cmd.append(QString(" ORDER BY %1 ASC ").arg(c_field_mi_cid));

    if(query.exec(cmd))
    {
        while(query.next())
        {
            mi_list.append(query.value(c_field_mi_cid).toString());
        }

    }

}



void ag_rack_data_table::read_mi_last_aging_time_by_mi(QSqlDatabase &m_database,QString mi,QString &room, QString &start_time, QString &stop_time,QString &pos_desc)
{

    QString cmd_ret;
    QStringList table_list;
    ag_rack_index_table ag_rack_inx_tb;
    table_list =  ag_rack_inx_tb.read_tablelist_from_mi(m_database,QStringList() << mi);

    QString select_head = QString("SELECT %1,%2,%3,%4 FROM ").arg(c_field_room_id,c_field_rack_id,c_field_mi_pos,c_field_start_time);
    QString select_tail = QString(" WHERE %1='%2' ").arg(c_field_mi_cid,mi);

    if(table_list.size() == 0)
        return;

    cmd_ret.append(select_head + " ( ");
    for(int i=0;i<table_list.size();i++)
    {
        if(i !=0)
        {
            cmd_ret.append(" UNION ALL ");
        }
        cmd_ret.append(select_head);
        cmd_ret.append(table_list[i]);
        cmd_ret.append(select_tail);
    }
    cmd_ret.append(QString(" ) e ORDER BY %1 DESC LIMIT 1").arg(c_field_start_time));

    QSqlQuery query(m_database);
    if(query.exec(cmd_ret))
    {
        if(query.size() == 1)
        {
            query.next();
            room = query.value(c_field_room_id).toString();
            start_time = query.value(c_field_start_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            stop_time = "2030-01-30 17:05:33";

            pos_desc = query.value(c_field_rack_id).toString() + " " + query.value(c_field_mi_pos).toString();
        }
    }
}

void ag_rack_data_table::read_mi_stop_time_after_start_time(QSqlDatabase &m_database, QString mi, QString &room, QString start_time, QString &stop_time,QString &pos_desc)
{
    QString cmd_head = QString(" SELECT %1,%2 FROM ").arg(c_field_room_id,c_field_start_time);
    QString pos_desc_head = QString(" SELECT %1,%2 FROM ").arg(c_field_rack_id,c_field_mi_pos);
    QString cmd_tail = QString(" WHERE %1='%2' AND %3>'%4' ").arg(c_field_mi_cid,mi,c_field_start_time,start_time);
    QString pos_desc_tail = QString(" WHERE %1='%2' AND %3='%4' ").arg(c_field_mi_cid,mi,c_field_start_time,start_time);
    QString tmp_cmd;
    QString tmp_pos_desc_cmd;
    QSqlQuery query(m_database);
    QStringList table_list;
    ag_rack_index_table ag_rack_inx_tb;

    table_list =  ag_rack_inx_tb.read_tablelist_from_mi(m_database,QStringList() << mi);
    for(int i=0;i<table_list.size();i++)
    {
        if(i !=0)
        {
            tmp_cmd.append(" UNION ALL");
            tmp_pos_desc_cmd.append(" UNION ALL");
        }

        tmp_cmd.append(cmd_head);
        tmp_cmd.append(table_list[i]);
        tmp_cmd.append(cmd_tail);

        tmp_pos_desc_cmd.append(pos_desc_head);
        tmp_pos_desc_cmd.append(table_list[i]);
        tmp_pos_desc_cmd.append(pos_desc_tail);
    }

    tmp_cmd.append(QString(" ORDER BY %1 ASC LIMIT 1").arg(c_field_start_time));
    tmp_pos_desc_cmd.append(QString(" LIMIT 1").arg(c_field_start_time));


    if(query.exec(tmp_cmd))
    {
        if(query.size() ==1)
        {
            query.next();
            room = query.value(c_field_room_id).toString();
            stop_time = query.value(c_field_start_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

        }
        else
        {
            room = "room-1";
            stop_time = "2030-01-30 17:05:33";

        }
    }

    if(query.exec(tmp_pos_desc_cmd))
    {
        if(query.size() == 1)
        {
            query.next();
            pos_desc = query.value(c_field_rack_id).toString() + " " + query.value(c_field_mi_pos).toString();
        }
    }
}

void ag_rack_data_table::read_mi_all(QSqlDatabase &m_database, const QString &mi, QStringList &rooms, QStringList &start_times, QStringList &stop_times, QStringList &pos_descs)
{

    QString cmd_ret;
    QStringList table_list;
    ag_rack_index_table ag_rack_inx_tb;
    table_list =  ag_rack_inx_tb.read_tablelist_from_mi(m_database,QStringList() << mi);

    QString select_head = QString("SELECT %1,%2,%3,%4 FROM ").arg(c_field_room_id,c_field_rack_id,c_field_mi_pos,c_field_start_time);
    QString select_tail = QString(" WHERE %1='%2' ").arg(c_field_mi_cid,mi);

    if(table_list.size() == 0)
        return;

    //cmd_ret.append(select_head + " ( ");
    for(int i=0;i<table_list.size();i++)
    {
        if(i !=0)
        {
            cmd_ret.append(" UNION ALL ");
        }
        cmd_ret.append(select_head);
        cmd_ret.append(table_list[i]);
        cmd_ret.append(select_tail);
    }
    //cmd_ret.append(" ) "); // 查询命令的结尾

    QSqlQuery query(m_database);
    if (query.exec(cmd_ret)) // 执行查询
    {
        while (query.next()) // 循环处理查询结果
        {
            // 从查询结果中获取房间号、开始时间、停止时间、位置描述，并添加到相应的列表中
            rooms.append(query.value(c_field_room_id).toString());
            start_times.append(query.value(c_field_start_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            stop_times.append("2030-01-30 17:05:33"); // 硬编码的停止时间，可以根据需要修改
            pos_descs.append(query.value(c_field_rack_id).toString() + " " + query.value(c_field_mi_pos).toString());
        }
    }
}



