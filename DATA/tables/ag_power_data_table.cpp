#include "ag_power_data_table.h"

#include "QsLog.h"
#include "mysql_table.h"

#include "ag_power_index_table.h"
#include "ag_mi_property_table.h"
const QString ag_power_data_table::c_field_id = "id";
const QString ag_power_data_table::c_field_emu_cid = "emu_cid";
const QString ag_power_data_table::c_field_emu_time = "emu_time";
const QString ag_power_data_table::c_field_mi_cid = "mi_cid";
const QString ag_power_data_table::c_field_mim_version = "mim_version";
const QString ag_power_data_table::c_field_mis_version = "mis_version";
const QString ag_power_data_table::c_field_hard_version = "hard_version";
const QString ag_power_data_table::c_field_pv = "pv";
const QString ag_power_data_table::c_field_power = "power";
const QString ag_power_data_table::c_field_energy = "energy";
const QString ag_power_data_table::c_field_temperature = "temperature";
const QString ag_power_data_table::c_field_gridv = "gridv";
const QString ag_power_data_table::c_field_gridcur = "gridcur";
const QString ag_power_data_table::c_field_gridf = "gridf";
const QString ag_power_data_table::c_field_mim_err = "mim_err";
const QString ag_power_data_table::c_field_mis_err = "mis_err";
const QString ag_power_data_table::c_field_pv_id = "pv_id";
const QString ag_power_data_table::c_field_nominal_power = "nominal_power";
const QString ag_power_data_table::c_field_reissue_data = "reissue_data";
const QString ag_power_data_table::c_field_sys_time = "sys_time";

ag_power_data_table::ag_power_data_table(QString name, QObject *parent)
    : QObject{parent}
{
    m_name = name;
}

ag_power_data_table::ag_power_data_table(QObject *parent)
    : QObject{parent}
{

}

bool ag_power_data_table::create_table(QSqlDatabase &m_database)
{
    QString tmp_field =
            QString("(%1 INT UNSIGNED NOT NULL AUTO_INCREMENT primary key,"
             "%2 VARCHAR(8) NOT NULL,"
             "%3 DATETIME NOT NULL,"
             "%4 VARCHAR(12) NOT NULL,"
             "%5 TINYINT UNSIGNED NOT NULL,"
             "%6 VARCHAR(7) NOT NULL,"
             "%7 VARCHAR(7) NOT NULL,"
             "%8 VARCHAR(7) ,"
             "%9 FLOAT NOT NULL,"
             "%10 FLOAT NOT NULL,"
             "%11 FLOAT NOT NULL,"
             "%12 FLOAT NOT NULL,"
             "%13 FLOAT NOT NULL,"
             "%14 FLOAT ,"
             "%15 FLOAT NOT NULL,"
             "%16 VARCHAR(4) NOT NULL,"
             "%17 VARCHAR(8) NOT NULL,"
             "%18 SMALLINT UNSIGNED,"
             "%19 VARCHAR(3) NOT NULL,"
             "%20 DATETIME NOT NULL,"
             "INDEX(%21),"
             "INDEX(%22),"
             "INDEX(%23),"
             "INDEX(%24))")
             .arg(c_field_id,c_field_emu_cid,c_field_emu_time,c_field_mi_cid,c_field_pv_id,c_field_mim_version,
                  c_field_mis_version,c_field_hard_version,c_field_pv,c_field_power,c_field_energy,c_field_temperature,
                  c_field_gridv,c_field_gridcur,c_field_gridf,c_field_mim_err,c_field_mis_err,c_field_nominal_power,
                  c_field_reissue_data,c_field_sys_time,c_field_mi_cid,c_field_sys_time,c_field_mim_err,c_field_mis_err);

    return mysql_table::create_table(m_database,m_name,tmp_field);
}

bool ag_power_data_table::delete_table(QSqlDatabase &m_database)
{
    return mysql_table::delete_table(m_database,m_name);
}

void ag_power_data_table::read_size(QSqlDatabase &m_database, QJsonObject &r_data, quint64 &size)
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

    if(r_data.value("emu_cid").toString() != "")
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
        mid_cmd += c_field_emu_cid + QString("='%1' ").arg(r_data.value("emu_cid").toString());
        where_flag = true;
    }

    if(r_data.value("mim_err").toString() != "")
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
        mid_cmd += c_field_mim_err + QString("='%1' ").arg(r_data.value("mim_err").toString());
        where_flag = true;
    }

    if(r_data.value("mis_err").toString() != "")
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
        mid_cmd += c_field_mis_err + QString("='%1' ").arg(r_data.value("mis_err").toString());
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

    mid_cmd += QString(" %1 BETWEEN '%2' AND '%3' ").arg(c_field_sys_time,r_data.value("start_date").toString(),
                                                      r_data.value("stop_date").toString());

    if(query.exec(select_cmd_head + mid_cmd))
    {
        if(query.next())
            size = query.value("COUNT(1)").toUInt();
    }
}

bool ag_power_data_table::read_data(QSqlDatabase &m_database,const QJsonObject &r_data, QJsonObject &data,bool is_one)
{
    QString select_cmd_head = QString("SELECT %1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17 FROM ")
            .arg(c_field_emu_cid,c_field_emu_time,c_field_mi_cid,c_field_mim_version,c_field_mis_version,c_field_pv,
                 c_field_power,c_field_energy,c_field_temperature,c_field_gridv,c_field_gridf,c_field_mim_err,
                 c_field_mis_err,c_field_pv_id,c_field_nominal_power,c_field_reissue_data,c_field_sys_time);

    QString select_size_head = QString("SELECT COUNT(1) FROM ");

    QString condition;

    QSqlQuery query(m_database);
    bool where_flag = false;
    //如果要进行筛选 微逆编号
    QString power_data_cmd;
    QString size_cmd;


    QStringList mi_list;
    QStringList table_list;
    ag_power_index_table pw_tb;
    QJsonArray mi_array = r_data.value(c_field_mi_cid).toArray();

    for(int i=0;i<mi_array.size();i++)
    {
        mi_list.append(mi_array[i].toString());
    }

    table_list = pw_tb.read_tablelist_from_mi(m_database,mi_list);

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

    if(r_data.value("emu_cid").toString() != "")
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
        condition += c_field_emu_cid + QString("='%1' ").arg(r_data.value("emu_cid").toString());
        //where_flag = true;
    }
    if(r_data.value("pv_id").toString() != "")
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
        condition += c_field_pv_id + QString("='%1' ").arg(r_data.value("pv_id").toString());
        //where_flag = true;
    }
    if(r_data.value("mim_err").toString() != "")
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
        condition += c_field_mim_err + QString("='%1' ").arg(r_data.value("mim_err").toString());
        //where_flag = true;
    }

    if(r_data.value("mis_err").toString() != "")
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
        condition += c_field_mis_err + QString("='%1' ").arg(r_data.value("mis_err").toString());
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

        condition += QString(" %1 BETWEEN '%2' AND '%3' ").arg(c_field_sys_time,r_data.value("start_date").toString(),
                                                          r_data.value("stop_date").toString());
    }


    for(int i=0;i<table_list.size();i++)
    {
        if(i !=0)
        {
            power_data_cmd.append(" UNION ALL ");
            size_cmd.append(" UNION ALL ");
        }
        power_data_cmd.append(select_cmd_head);
        power_data_cmd.append(table_list[i]);
        power_data_cmd.append(condition);

        size_cmd.append(select_size_head);
        size_cmd.append(table_list[i]);
        size_cmd.append(condition);
    }
    power_data_cmd += QString("ORDER BY %1 DESC,%2 ASC").arg(c_field_sys_time,c_field_pv_id);

    if(r_data.value("nums").toString() != "-1")
    {
        power_data_cmd += QString(" LIMIT %1,%2 ").arg(r_data.value("start_num").toString(),
                                                     r_data.value("nums").toString());
    }


    uint64_t data_size = 0;
    if(query.exec(size_cmd))
    {
        while (query.next()) {
            data_size += query.value("COUNT(1)").toUInt();
        }
    }

    data.insert("all_size",QString::number(data_size));

    if(query.exec(power_data_cmd))
    {
        QJsonArray pv_data_array;
        while (query.next())
        {
            QJsonObject pv_data_js;
            pv_data_js.insert(c_field_emu_cid,query.value(c_field_emu_cid).toString());
            pv_data_js.insert(c_field_emu_time,query.value(c_field_emu_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            pv_data_js.insert(c_field_mi_cid,query.value(c_field_mi_cid).toString());
            pv_data_js.insert(c_field_pv_id,query.value(c_field_pv_id).toInt());
            pv_data_js.insert(c_field_mim_version,query.value(c_field_mim_version).toString());
            pv_data_js.insert(c_field_mis_version,query.value(c_field_mis_version).toString());
            pv_data_js.insert(c_field_pv,QString::number(query.value(c_field_pv).toFloat(),'f',2));
            pv_data_js.insert(c_field_power,QString::number(query.value(c_field_power).toFloat(),'f',2));
            pv_data_js.insert(c_field_energy,QString::number(query.value(c_field_energy).toFloat(),'f',2));
            pv_data_js.insert(c_field_temperature,QString::number(query.value(c_field_temperature).toFloat(),'f',2));
            pv_data_js.insert(c_field_gridv,QString::number(query.value(c_field_gridv).toFloat(),'f',2));
            pv_data_js.insert(c_field_gridf,QString::number(query.value(c_field_gridf).toFloat(),'f',2));
            pv_data_js.insert(c_field_mim_err,query.value(c_field_mim_err).toString());
            pv_data_js.insert(c_field_mis_err,query.value(c_field_mis_err).toString());
            //pv_data_js.insert("nominal_power",(int64_t)tmp_nominal.value(query.value(m_powertable_field.mi_cid).toString()));
            pv_data_js.insert(c_field_reissue_data,query.value(c_field_reissue_data).toString());
            pv_data_js.insert(c_field_sys_time,query.value(c_field_sys_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

            pv_data_array.append(pv_data_js);
        }

        if(pv_data_array.size()>0)
        {
            data.insert("stop_date",pv_data_array[0].toObject().value(c_field_sys_time).toString());
        }
        else
        {
            data.insert("stop_date",r_data.value("start_date").toString());
        }

        data.insert("datas",pv_data_array);

        data.insert("start_date",r_data.value("start_date").toString());

        if(is_one && mi_list.size() == 1)
        {
            data.insert("mi_cid",mi_list[0]);
        }
        QLOG_INFO() << "查询 发电数据成功";
        return true;
    }
    QLOG_WARN() << "查询发电数据失败";

    return false;
}





void ag_power_data_table::read_current_data(QSqlDatabase &m_database, QStringList &mi_list, QJsonObject &data)
{
    //QJsonObject tmp_param = r_data.value("params").toObject();
    QString select_cmd_head = QString("SELECT %1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17 ")
            .arg(c_field_emu_cid,c_field_emu_time,c_field_mi_cid,c_field_mim_version,c_field_mis_version,c_field_pv,
                 c_field_power,c_field_energy,c_field_temperature,c_field_gridv,c_field_gridf,c_field_mim_err,
                 c_field_mis_err,c_field_pv_id,c_field_nominal_power,c_field_reissue_data,c_field_sys_time);
    QSqlQuery query(m_database);
    //QStringList mi_list;
    QStringList table_list;
    ag_power_index_table pw_tb;
    ag_mi_property_table mi_pty_tb;
    QHash<QString,QString> mi_nominal_pw;
    QJsonArray pv_data_array;

    mi_pty_tb.read_nominal_power(m_database,mi_list,mi_nominal_pw);

    for(int i=0;i< mi_list.size();i++)
    {
        QString tmp_cur_table = "";
        QString tmp_cmd;

        table_list = pw_tb.read_tablelist_from_mi(m_database,QStringList() << mi_list[i]);
        //找到日期最大的表
        for(int j=0;j<table_list.size();j++)
        {
            if(tmp_cur_table < table_list[j])
            {
                tmp_cur_table = table_list[j];
            }
        }
        if(tmp_cur_table == "")
            continue;

        tmp_cmd = select_cmd_head + " FROM ( " + select_cmd_head +
                QString(" , ROW_NUMBER() OVER(PARTITION BY %1,%2 ORDER BY sys_time DESC) AS row_num FROM %3 WHERE %4 = '%5') "
                        "AS ranked_data WHERE row_num = 1").arg(c_field_mi_cid,c_field_pv_id,tmp_cur_table,c_field_mi_cid,mi_list[i]);

        if(query.exec(tmp_cmd))
        {
            while(query.next())
            {
                QJsonObject pv_data_js;
                pv_data_js.insert(c_field_emu_cid,query.value(c_field_emu_cid).toString());
                pv_data_js.insert(c_field_emu_time,query.value(c_field_emu_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
                pv_data_js.insert(c_field_mi_cid,query.value(c_field_mi_cid).toString());
                pv_data_js.insert(c_field_pv_id,query.value(c_field_pv_id).toInt());
                pv_data_js.insert(c_field_mim_version,query.value(c_field_mim_version).toString());
                pv_data_js.insert(c_field_mis_version,query.value(c_field_mis_version).toString());
                pv_data_js.insert(c_field_pv,QString::number(query.value(c_field_pv).toFloat(),'f',2));
                pv_data_js.insert(c_field_power,QString::number(query.value(c_field_power).toFloat(),'f',2));
                pv_data_js.insert(c_field_energy,QString::number(query.value(c_field_energy).toFloat(),'f',2));
                pv_data_js.insert(c_field_temperature,QString::number(query.value(c_field_temperature).toFloat(),'f',2));
                pv_data_js.insert(c_field_gridv,QString::number(query.value(c_field_gridv).toFloat(),'f',2));
                pv_data_js.insert(c_field_gridf,QString::number(query.value(c_field_gridf).toFloat(),'f',2));
                pv_data_js.insert(c_field_mim_err,query.value(c_field_mim_err).toString());
                pv_data_js.insert(c_field_mis_err,query.value(c_field_mis_err).toString());
                pv_data_js.insert(c_field_nominal_power,mi_nominal_pw.value(query.value(c_field_mi_cid).toString()));
                pv_data_js.insert(c_field_reissue_data,query.value(c_field_reissue_data).toString());
                pv_data_js.insert(c_field_sys_time,query.value(c_field_sys_time).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

                pv_data_array.append(pv_data_js);
            }
        }
    }
    data.insert("pw_datas",pv_data_array);
}

void ag_power_data_table::write_data(QSqlDatabase &m_database, QJsonObject &w_data)
{
    QSqlQuery query(m_database);
    QJsonArray tmp_datas = w_data.value("datas").toArray();

    QString head = QString("INSERT INTO %1 ").arg(m_name);
    QString cmd = QString("(%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)")
            .arg(c_field_emu_cid,c_field_emu_time,c_field_mi_cid,c_field_pv_id,c_field_mim_version,c_field_mis_version,c_field_pv,
                 c_field_power,c_field_energy,c_field_temperature,c_field_gridv,c_field_gridf,c_field_mim_err,c_field_mis_err,
                 c_field_reissue_data,c_field_sys_time);
    QVariantList records[16];
    for(int i=0;i<tmp_datas.size();i++)
    {
        QJsonObject mi_pw_obj = tmp_datas[i].toObject();
        records[0] << mi_pw_obj.value(c_field_emu_cid).toString();
        records[1] << mi_pw_obj.value(c_field_emu_time).toString();
        records[2] << mi_pw_obj.value(c_field_mi_cid).toString();
        records[3] << mi_pw_obj.value(c_field_pv_id).toInt();
        records[4] << mi_pw_obj.value(c_field_mim_version).toString();
        records[5] << mi_pw_obj.value(c_field_mis_version).toString();
        records[6] << mi_pw_obj.value(c_field_pv).toDouble();
        records[7] << mi_pw_obj.value(c_field_power).toDouble();
        records[8] << mi_pw_obj.value(c_field_energy).toDouble();
        records[9] << mi_pw_obj.value(c_field_temperature).toDouble();
        records[10] << mi_pw_obj.value(c_field_gridv).toDouble();
        records[11] << mi_pw_obj.value(c_field_gridf).toDouble();
        records[12] << mi_pw_obj.value(c_field_mim_err).toString();
        records[13] << mi_pw_obj.value(c_field_mis_err).toString();
        records[14] << mi_pw_obj.value(c_field_reissue_data).toString();
        records[15] << mi_pw_obj.value(c_field_sys_time).toString();
    }

    query.prepare(head + cmd);
    query.addBindValue(records[0]);
    query.addBindValue(records[1]);
    query.addBindValue(records[2]);
    query.addBindValue(records[3]);
    query.addBindValue(records[4]);
    query.addBindValue(records[5]);
    query.addBindValue(records[6]);
    query.addBindValue(records[7]);
    query.addBindValue(records[8]);
    query.addBindValue(records[9]);
    query.addBindValue(records[10]);
    query.addBindValue(records[11]);
    query.addBindValue(records[12]);
    query.addBindValue(records[13]);
    query.addBindValue(records[14]);
    query.addBindValue(records[15]);

    if(query.execBatch())
    {
        //QLOG_INFO() << QString("微逆:%1 写入发电数据表成功");
    }
    else {
        QLOG_WARN() << QString("微逆:%1 写入发电数据表失败");
    }
}
