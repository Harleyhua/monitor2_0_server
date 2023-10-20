#include "mysql.h"

#include <QMutex>
#include <QSqlDriver>
#include <QSqlError>



#include "QsLog.h"
#include "common.h"
#include "abstract_bym.h"
#include "ag_gateway_data_table.h"
#include "ag_mi_property_table.h"
#include "ag_power_data_table.h"
#include "ag_power_index_table.h"
#include "ag_rack_data_table.h"
#include "ag_rack_index_table.h"
#include "ag_rack_mi_table.h"
#include "ag_temp_table.h"
#include "ag_user_table.h"
#include "ag_workorder_table.h"
#include "ag_mi_cmd_table.h"
#include "ag_pos_table.h"
#include "ag_user_station_table.h"
#include "ag_emu_property_table.h"
#include "ag_station_emu_table.h"
#include "ag_emu_mi_table.h"
#include "ag_rack_index_table.h"
#include "ag_mi_report_table.h"
#include "ag_device_control_table.h"
#include "ag_emu_status_table.h"
#include "ag_user_act_table.h"
#include "ag_rack_extra_data_table.h"
#include "emu_protocolb.h"
#include "ag_rack_extra_data_table.h"
#include "ag_mi_extra_property_table.h"
#include "ag_emu_extra_property_table.h"


QMutex sql_lock;   //数据库建立连接锁，避免多线程是同时连接的异常
mysql_login_stc login_param = {"127.0.0.1",3306,"jack_lin","zbeny001","bydas"};  //老化房 测试 本机
//mysql_login_stc login_param = {"127.0.0.1",3306,"jack_lin","zbeny001","bydas2"};   //本机
//mysql_login_stc login_param = {"1.117.152.46",3306,"root","zjbeny001","bydas"};

QHash<QString,QString> mysql::m_emucid_hand_lastTime{};

mysql::mysql(QString db_name, QObject *parent)
    : QObject{parent}
{
    this->db_name = QDateTime::currentDateTime().toString() + db_name;
    sql_lock.lock();
    m_db = QSqlDatabase::addDatabase("QMYSQL",this->db_name);
    m_db.setHostName(login_param.hostname);
    m_db.setPort(login_param.port);
    m_db.setUserName(login_param.username);
    m_db.setPassword(login_param.password);
    //m_db.setDatabaseName(login_param.database_name);
    m_db.open();

    sql_lock.unlock();
    //如果数据库未被建立  不能先setdatabasename
    m_db.setDatabaseName(login_param.database_name);
    QSqlQuery query(m_db);
    query.exec("USE " + login_param.database_name);
}
mysql::~mysql()
{
    sql_lock.lock();
    m_db.close();
    m_db = QSqlDatabase();  //重置db
    m_db.removeDatabase(this->db_name);
    sql_lock.unlock();
}



bool mysql::table_init()
{
    bool ret = true;
    ag_gateway_data_table tmp_gt_dt_tb;
    ag_mi_property_table tmp_mi_pro_tb;

    ag_power_index_table tmp_pw_idx_tb;
    //ag_rack_data_table tmp_rk_dt_tb;
    ag_rack_index_table tmp_rk_idx_tb;
    ag_rack_mi_table tmp_rk_mi_tb;
    ag_temp_table tmp_temp_tb;
    ag_user_table tmp_user_tb;
    ag_workorder_table tmp_wk_tb;

    ag_mi_cmd_table tmp_cmd_tb;

    ag_pos_table tmp_pos_tb;

    ag_user_station_table tmp_user_station_tb;
    ag_emu_property_table tmp_emu_pro_tb;

    ag_station_emu_table tmp_station_emu_tb;
    ag_emu_mi_table tmp_emu_mi_tb;
    ag_mi_report_table tmp_mi_report_tb;
    ag_device_control_table tmp_dev_ctl_tb;
    ag_emu_status_table tmp_emu_status_tb;
    ag_user_act_table tmp_user_act_tb;
    ag_rack_extra_data_table tmp_rack_extra_tb;
    ag_mi_extra_property_table tmp_mi_extra_pty_tb;
    ag_emu_extra_property_table tmp_emu_extra_pty_tb;


    ret &= create_database(m_db.databaseName()); //创建数据库 if not exist
    ret &= tmp_gt_dt_tb.create_table(m_db);
    ret &= tmp_mi_pro_tb.create_table(m_db);
    ret &= tmp_pw_idx_tb.create_table(m_db);
    ret &= tmp_rk_idx_tb.create_table(m_db);
    ret &= tmp_rk_mi_tb.create_table(m_db);
    ret &= tmp_temp_tb.create_table(m_db);
    ret &= tmp_user_tb.create_table(m_db);
    ret &= tmp_wk_tb.create_table(m_db);
    ret &= tmp_cmd_tb.create_table(m_db);
    ret &= tmp_pos_tb.create_table(m_db);
    ret &= tmp_user_station_tb.create_table(m_db);
    ret &= tmp_emu_pro_tb.create_table(m_db);
    ret &= tmp_station_emu_tb.create_table(m_db);
    ret &= tmp_emu_mi_tb.create_table(m_db);
    ret &= tmp_mi_report_tb.create_table(m_db);
    ret &= tmp_dev_ctl_tb.create_table(m_db);
    ret &= tmp_emu_status_tb.create_table(m_db);
    ret &= tmp_user_act_tb.create_table(m_db);
    ret &= tmp_rack_extra_tb.create_table(m_db);
    ret &= tmp_mi_extra_pty_tb.create_table(m_db);
    ret &= tmp_emu_extra_pty_tb.create_table(m_db);

    for(int i=1;i<=12;i++)
    {
        QString year = QDateTime::currentDateTime().toString("yy");
        QString name = "power_data_" + year + QString::number(i).rightJustified(2, '0')+ "_1";
        ag_power_data_table tmp_pw_dt_tb(name);
        ret &= tmp_pw_dt_tb.create_table(m_db);
    }

    for(int i=1;i<=12;i++)
    {
        QString year = QDateTime::currentDateTime().toString("yy");
        QString name = "rack_data_" + year + QString::number(i).rightJustified(2, '0')+ "_1";
        ag_rack_data_table tmp_rk_dt_tb(name);
        ret &= tmp_rk_dt_tb.create_table(m_db);
    }


    ag_user_table tmp_us_tb;
    QStringList total_stations;
    tmp_us_tb.read_all_total_station(m_db,total_stations);



    for(int i=0;i<total_stations.size();i++)
    {
        QJsonObject us_obj;
        QJsonArray st_array;
        ag_user_station_table tmp_us_tb;
        ag_gateway_data_table tmp_emu_data_tb;
        tmp_us_tb.read_station(m_db,total_stations[i],us_obj);
        st_array = us_obj.value("datas").toObject().value("station").toArray();
        for(int j=0;j<st_array.size();j++)
        {
            ag_station_emu_table tmp_st_emu_tb;
            QStringList emu_cid,emu_desc;
            tmp_st_emu_tb.r_emu(m_db,st_array[j].toString(),emu_cid,emu_desc);

            for(int k=0;k<emu_cid.size();k++)
            {
                QString tmp_last_time;
                tmp_emu_data_tb.read_last_hand_data_time(m_db,emu_cid[k],tmp_last_time);
                if(tmp_last_time != "")
                {
                    m_emucid_hand_lastTime.insert(emu_cid[k],tmp_last_time);
                }
            }
        }
    }


    return ret;
}

void mysql::w_emu_action(QJsonObject &s_data)
{
    ag_gateway_data_table tmp_gt_dt_tb;
    QJsonObject d_obj = s_data.value("datas").toObject();

    tmp_gt_dt_tb.write_data(m_db,s_data);

    if(d_obj.value(ag_gateway_data_table::c_field_action).toString() == "handshake")
    {
        m_emucid_hand_lastTime.insert(d_obj.value(ag_gateway_data_table::c_field_emu_cid).toString(),
                                  d_obj.value(ag_gateway_data_table::c_field_sys_time).toString());
    }
}

void mysql::w_login(QJsonObject &s_data)
{
    ag_gateway_data_table tmp_gt_dt_tb;

    tmp_gt_dt_tb.write_data(m_db,s_data);
}

void mysql::w_emu_property(QJsonObject &s_data)
{
    ag_emu_property_table tmp_emu_property_tb;
    tmp_emu_property_tb.write_property(m_db,s_data);
}


void mysql::w_handshake(QJsonObject &s_data)
{
    ag_gateway_data_table tmp_gt_dt_tb;

    tmp_gt_dt_tb.write_data(m_db,s_data);
}





void mysql::add_station(QString user, QJsonObject &s_data)
{
    ag_user_station_table tmp_tb;

    QString total_station;
    ag_user_table tmp_user_tb;

    tmp_user_tb.read_total_stations(m_db,user,total_station);

    tmp_tb.write_station(m_db,total_station,s_data);

}

void mysql::add_station(QString user, QString station)
{
    ag_user_station_table tmp_tb;

    QString total_station;
    ag_user_table tmp_user_tb;

    tmp_user_tb.read_total_stations(m_db,user,total_station);

    tmp_tb.write_station(m_db,total_station,station);
}

void mysql::add_emu_cid(QString user, QJsonObject &s_data)
{
    QString station = s_data.value("station").toString();
    ag_user_station_table tmp_us_tb;
    ag_station_emu_table tmp_sta_tb;

    QString total_station;
    ag_user_table tmp_user_tb;

    tmp_user_tb.read_total_stations(m_db,user,total_station);
    //如果当前电站存在  则允许添加网关
    if(tmp_us_tb.is_station_exist(m_db,total_station,station))
    {
        tmp_sta_tb.w_emu(m_db,station,s_data);
    }

}

void mysql::add_plcmi_cid(QString user, QJsonObject &s_data)
{
    QString station = s_data.value("station").toString();
    QString emu = s_data.value("emu").toString();
    ag_user_station_table tmp_us_tb;
    ag_station_emu_table tmp_sta_emu_tb;
    ag_emu_mi_table tmp_emu_mi_tb;

    QString total_station;
    ag_user_table tmp_user_tb;

    tmp_user_tb.read_total_stations(m_db,user,total_station);
    if(tmp_us_tb.is_station_exist(m_db,total_station,station))
    {
        if(tmp_sta_emu_tb.is_emu_exist(m_db,station,emu))
        {
            tmp_emu_mi_tb.w_mi(m_db,emu,s_data);
        }
    }
    update_dev_control(total_station,station,emu,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),0x01,"");

}

void mysql::add_wifimi_cid(QString user, QJsonObject &s_data)
{
    QString station = s_data.value("station").toString();
    QJsonArray emus = s_data.value("emus").toArray();

    for(int i=0;i<emus.size();i++)
    {
        QString emu = emus[i].toObject().value("emu").toString();
        ag_user_station_table tmp_us_tb;
        ag_station_emu_table tmp_sta_emu_tb;
        ag_emu_mi_table tmp_emu_mi_tb;
        QString total_station;
        ag_user_table tmp_user_tb;

        tmp_user_tb.read_total_stations(m_db,user,total_station);
        if(tmp_us_tb.is_station_exist(m_db,total_station,station))
        {

            tmp_sta_emu_tb.w_one_emu(m_db,station,emu,"");
            //写入成功
            if(tmp_sta_emu_tb.is_emu_exist(m_db,station,emu))
            {
                tmp_emu_mi_tb.w_one_mi(m_db,emu,emus[i].toObject().value("mi").toObject().value("mi_cid").toString(),
                                       emus[i].toObject().value("mi").toObject().value("mi_desc").toString());
            }
        }
    }
}

void mysql::add_wifimi_cid(QString user,QString station, QString emu, QString mi, QString mi_desc)
{
    //QString emu = emus[i].toObject().value("emu").toString();
    ag_user_station_table tmp_us_tb;
    ag_station_emu_table tmp_sta_emu_tb;
    ag_emu_mi_table tmp_emu_mi_tb;
    QString total_station;
    ag_user_table tmp_user_tb;

    tmp_user_tb.read_total_stations(m_db,user,total_station);
    if(tmp_us_tb.is_station_exist(m_db,total_station,station))
    {
        tmp_sta_emu_tb.w_one_emu(m_db,station,emu,"");
        //写入成功
        if(tmp_sta_emu_tb.is_emu_exist(m_db,station,emu))
        {
            tmp_emu_mi_tb.w_one_mi(m_db,emu,mi,mi_desc);
        }
    }

}

void mysql::del_station(QString user, QJsonObject &s_data)
{
    ag_user_station_table tmp_us_tb;
    ag_station_emu_table tmp_st_emu_tb;
    QString station = s_data.value("station").toString();
    QStringList emus;
    QStringList emu_desc;
    tmp_st_emu_tb.r_emu(m_db,station,emus,emu_desc);

    for(int i=0;i<emus.size();i++)
    {
        del_emu(user,station,emus[i]);
    }
    tmp_us_tb.del_station_by_station(m_db,station);
}

void mysql::del_station(QString user, QString station)
{
    ag_user_station_table tmp_us_tb;
    ag_station_emu_table tmp_st_emu_tb;
    //QString station = s_data.value("station").toString();
    QStringList emus;
    QStringList emu_desc;
    tmp_st_emu_tb.r_emu(m_db,station,emus,emu_desc);

    for(int i=0;i<emus.size();i++)
    {
        del_emu(user,station,emus[i]);
    }
    tmp_us_tb.del_station_by_station(m_db,station);
}



void mysql::del_emu(QString user, QString station, QString emu)
{
    ag_user_station_table tmp_us_tb;
    QString total_station;
    ag_user_table tmp_user_tb;

    tmp_user_tb.read_total_stations(m_db,user,total_station);

    if(tmp_us_tb.is_station_exist(m_db,total_station,station))
    {
        ag_station_emu_table tmp_st_emu_tb;
        if(tmp_st_emu_tb.is_emu_exist(m_db,station,emu))
        {
            ag_emu_mi_table tmp_emu_mi_tb;
            tmp_emu_mi_tb.del_mis_by_emucid(m_db,emu);

            update_dev_control(total_station,station,emu,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),0x01,"");
        }

        tmp_st_emu_tb.del_emu_by_emucid(m_db,station,emu);
    }
}

void mysql::del_emu(QString user, QJsonObject &s_data)
{
    ag_user_station_table tmp_us_tb;
    QString total_station;
    ag_user_table tmp_user_tb;

    tmp_user_tb.read_total_stations(m_db,user,total_station);
    if(tmp_us_tb.is_station_exist(m_db,total_station,s_data.value("station").toString()))
    {
        ag_station_emu_table tmp_st_emu_tb;
        if(tmp_st_emu_tb.is_emu_exist(m_db,s_data.value("station").toString(),s_data.value("emu_cid").toString()))
        {
            ag_emu_mi_table tmp_emu_mi_tb;
            tmp_emu_mi_tb.del_mis_by_emucid(m_db,s_data.value("emu_cid").toString());

            update_dev_control(total_station,s_data.value("station").toString(),s_data.value("emu_cid").toString(),
                               QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),0x01,"");
        }

        tmp_st_emu_tb.del_emu_by_emucid(m_db,s_data.value("station").toString(),s_data.value("emu_cid").toString());
    }

}

void mysql::del_mi(QString user, QJsonObject &s_data)
{
    ag_user_station_table tmp_us_tb;
    QString total_station;
    ag_user_table tmp_user_tb;

    tmp_user_tb.read_total_stations(m_db,user,total_station);
    if(tmp_us_tb.is_station_exist(m_db,total_station,s_data.value("station").toString()))
    {
        ag_station_emu_table tmp_st_emu_tb;
        if(tmp_st_emu_tb.is_emu_exist(m_db,s_data.value("station").toString(),s_data.value("emu_cid").toString()))
        {
            ag_emu_mi_table tmp_emu_mi_tb;
            tmp_emu_mi_tb.del_one_mi_by_micid(m_db,s_data.value("emu_cid").toString(),s_data.value("mi_cid").toString());

            update_dev_control(total_station,s_data.value("station").toString(),s_data.value("emu_cid").toString(),
                               QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),0x01,"");
        }
    }
}


//void mysql::r_aging_data_by_mis(QString mi, QString &room,QString &pos_desc,QJsonObject &rt_data)
//{
//    QString start_time;
//    QString stop_time;


//    r_aging_data_by_mi_date(mi,start_time,stop_time,rt_data);
//}





void mysql::w_mi_property(QJsonObject &s_data)
{
    ag_mi_property_table tmp_mi_pty_tb;

    tmp_mi_pty_tb.write_property(m_db,s_data);
}

void mysql::r_mi_property(QJsonObject &s_data, QJsonObject &rt_data)
{
    ag_mi_property_table tmp_mi_pty_tb;

    tmp_mi_pty_tb.read_property(m_db,s_data,rt_data);
}

void mysql::w_power(QJsonObject &s_data, uint16_t date)
{
    QString table_name;
    //uint16_t date = common::get_system_yearmon_time();
    QJsonArray power_datas = s_data.value("datas").toArray();

    for(int i=0;i<power_datas.size();i++)
    {
        ag_power_index_table tmp_pw_idx_tb;
        QJsonArray table_list;
        QJsonObject idx_r_data;
        QJsonObject idx_datas;
        QJsonObject rt_data;
        idx_datas.insert("mi_cid",power_datas[i].toObject().value("mi_cid").toString());
        idx_datas.insert("date",date);
        idx_r_data.insert("params",idx_datas);

        tmp_pw_idx_tb.read_power_index(m_db,idx_r_data,rt_data);

        table_list = rt_data.value("datas").toObject().value("datatable_name").toArray();

        if(table_list.size() >0)
        {
            table_name = table_list[0].toString();
        }
        else
        {
            table_name = "power_data_" + QString::number(date) + "_1";
            //读写的json  比较接近 直接拿来用了
            idx_r_data.insert("datatable_name",table_name);
            QJsonObject w_data;
            QJsonObject w_datas;
            w_datas.insert("mi_cid",power_datas[i].toObject().value("mi_cid").toString());
            w_datas.insert("date",date);
            w_datas.insert("datatable_name",table_name);
            w_data.insert("datas",w_datas);
            //第一次写入时
            tmp_pw_idx_tb.write_power_index(m_db,w_data);
        }
    }
    ag_power_data_table tmp_pw_data_tb(table_name);

    //写数据要开启事务
    //保证多路微逆的数据原子性
    m_db.transaction();
    //写发电数据
    tmp_pw_data_tb.write_data(m_db,s_data);

    m_db.commit();
}

void mysql::r_emu_mapping(QJsonObject &s_data, QJsonObject &rt_data)
{
    ag_emu_mi_table tb;
    QStringList mis;
    QStringList desc_list;
    tb.r_mi(m_db,s_data.value("params").toObject().value("emu_cid").toString(),
            mis,desc_list);
    if(mis.size() != desc_list.size())
        return;

    QJsonArray datas;

    for(int i=0;i<mis.size();i++)
    {
        QJsonObject mi;
        mi.insert("mi_cid",mis[i]);

        datas.append(mi);
    }

    rt_data.insert("datas",datas);
}

void mysql::w_emu_status(QJsonObject &s_data)
{
    ag_emu_status_table tb;

    tb.w_data(m_db,s_data);
}

void mysql::w_room_temp(QJsonObject &s_data)
{
    ag_temp_table tp_tb;
    tp_tb.write_temp(m_db,s_data);

}

void mysql::w_rack_data(QJsonObject &s_data)
{
    //ag_rack_data_table rk_dt_tb;
    //ag_station_emu_table st_emu_tb;
    ag_emu_mi_table emu_mi_tb;


    //清除room-1的mapping内容
    QString room = s_data.value("room_id").toString();
    QString current = s_data.value("current_time").toString();
    QJsonArray racks = s_data.value("racks").toArray();

    QJsonObject rk_data_obj;
    QJsonArray w_datas_array;

    QStringList default_aging_emu;
    ag_station_emu_table station_emu_tb;


    del_station("aging_room1","room-1");

    add_station("aging_room1","room-1");

    for(int i=1;i<=6;i++)
    {
        for(int j=1;j<=8;j++)
        {
            QString emu_cid = "909999" + QString::number(i)+QString::number(j);
            QString emu_desc = "rack-" + QString::number(i) + " L" + QString::number(j);
            default_aging_emu.append(emu_cid);
            emu_mi_tb.del_mis_by_emucid(m_db,emu_cid);

            station_emu_tb.w_one_emu(m_db,"room-1",emu_cid,emu_desc);
        }
    }


    for(int i=0;i<racks.size();i++)
    {
        QString rack_name = racks[i].toObject().value("rack_id").toString();


        QJsonArray leakage_arr = racks[i].toObject().value("leakage").toArray();
        QJsonArray nodes = racks[i].toObject().value("nodes").toArray();
        for(int j=0;j<leakage_arr.size();j++)
        {


        }
        QMap<QString,QStringList> emu_mi;
        QMap<QString,QString> emu_wifi_mis;
        QMap<QString,QString> mi_desc;
        //一个节点一个微逆
        for(int j=0;j<nodes.size();j++)
        {
            QJsonObject node_obj = nodes[j].toObject();
            QString pos = node_obj.value("pos").toString();
            if(pos.size() <=0)
            {
                continue;
            }
            QString mi_cid = node_obj.value("mi_cid").toString();
            QString emu_cid = "909999" + rack_name.rightRef(1) + pos.mid(0,1);//默认规则  需要人为支持
            QStringList plc_mis = emu_mi.value(emu_cid);
            bool is_wifi = false;
            QString old_emu_wifi_mi_cid = mi_cid;
            if(mi_cid.startsWith("A"))
            {
                mi_cid[0] = '1';
                is_wifi = true;
            }
            else if(mi_cid.startsWith("B"))
            {
                mi_cid[0] = '2';
                is_wifi = true;
            }
            else if(mi_cid.startsWith("C"))
            {
                mi_cid[0] = '4';
                is_wifi = true;
            }

            //编号必须有效
            if(abstract_bym::is_cid_valid(mi_cid))
            {
                if(is_wifi)
                {
                    emu_wifi_mis.insert(old_emu_wifi_mi_cid,mi_cid);
                }
                else
                {
                    plc_mis.append(mi_cid);
                }

                mi_desc.insert(mi_cid, rack_name + " " + pos);
            }
            emu_mi.insert(emu_cid,plc_mis);

            QJsonObject mi_obj;
            mi_obj.insert("room_id",room);
            mi_obj.insert("rack_id",rack_name);
            mi_obj.insert("leakage_status","OFF");
            mi_obj.insert("mi_pos",pos);
            mi_obj.insert("mi_cid",mi_cid);
            mi_obj.insert("dc_volt_in",node_obj.value("dc_V").toDouble());
            mi_obj.insert("dc_current_in",node_obj.value("dc_A").toDouble());
            mi_obj.insert("dc_power_in",node_obj.value("dc_W").toDouble());
            mi_obj.insert("dc_volt_eff_k",1);
            mi_obj.insert("dc_volt_eff_b",0);
            mi_obj.insert("ac_volt_out",node_obj.value("ac_V").toDouble());
            mi_obj.insert("ac_current_out",node_obj.value("ac_A").toDouble());
            mi_obj.insert("ac_power_out",node_obj.value("ac_W").toDouble());
            mi_obj.insert("ac_volt_eff_k",1);
            mi_obj.insert("ac_volt_eff_b",0);
            //mi_obj.insert("efficiency");
            mi_obj.insert("start_time",node_obj.value("aging_start_time").toString());
            mi_obj.insert("age_time",node_obj.value("age_time").toInt());
            mi_obj.insert("cur_date",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

            w_datas_array.append(mi_obj);
        }

        //处理映射
        QStringList keys = emu_mi.keys();

        for(int j=0;j<keys.size();j++)
        {
            QStringList e_mis = emu_mi[keys[j]];
            for(int k = 0;k<e_mis.size();k++)
            {
                emu_mi_tb.w_one_mi(m_db,keys[j],e_mis[k],mi_desc[e_mis[k]]);
            }
        }

        keys = emu_wifi_mis.keys();
        for(int j=0;j<keys.size();j++)
        {
             add_wifimi_cid("aging_room1","room-1",keys[j],emu_wifi_mis[keys[j]],mi_desc[emu_wifi_mis[keys[j]]]);
        }
    }

    for(int i=0;i<default_aging_emu.size();i++)
    {
        update_dev_control("total_station1","room-1",default_aging_emu[i],
                           QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),0x01,"");
    }

    rk_data_obj.insert("datas",w_datas_array);

    m_db.transaction();
    //写rackdata
    w_rk_data(rk_data_obj);

    m_db.commit();

    //rk_dt_tb.write_data(m_db,rk_data_obj);
}



void mysql::w_rk_data(QJsonObject &s_data)
{
    QString table_name;
    uint16_t date = common::get_system_yearmon_time();
    QJsonArray rack_datas = s_data.value("datas").toArray();

    for(int i=0;i<rack_datas.size();i++)
    {
        ag_rack_index_table tmp_rk_idx_tb;
        QJsonArray table_list;
        QJsonObject idx_r_data;
        QJsonObject idx_datas;
        QJsonObject rt_data;
        idx_datas.insert("mi_cid",rack_datas[i].toObject().value("mi_cid").toString());
        idx_datas.insert("date",date);
        idx_r_data.insert("params",idx_datas);

        tmp_rk_idx_tb.read_rack_index(m_db,idx_r_data,rt_data);

        table_list = rt_data.value("datas").toObject().value("datatable_name").toArray();

        if(table_list.size() >0)
        {
            table_name = table_list[0].toString();
        }
        else
        {
            table_name = "rack_data_" + QString::number(date) + "_1";
            //读写的json  比较接近 直接拿来用了
            idx_r_data.insert("datatable_name",table_name);
            QJsonObject w_data;
            QJsonObject w_datas;
            w_datas.insert("mi_cid",rack_datas[i].toObject().value("mi_cid").toString());
            w_datas.insert("date",date);
            w_datas.insert("datatable_name",table_name);
            w_data.insert("datas",w_datas);
            //第一次写入时
            tmp_rk_idx_tb.write_rack_index(m_db,w_data);
        }
    }
    ag_rack_data_table tmp_rk_data_tb(table_name);
    ag_rack_extra_data_table tmp_rk_extra_tb;
    //写发电数据
    tmp_rk_data_tb.write_data(m_db,s_data);
    tmp_rk_extra_tb.write_data(m_db,s_data);
}
void mysql::r_user_property(QString name, QJsonObject &rt_data)
{
    ag_user_table user_tb;
    QJsonObject user_js;
    user_js.insert(ag_user_table::c_field_account,name);

    user_tb.read_account(m_db,user_js,rt_data);
}

void mysql::r_user_property(QJsonObject &s_data, QJsonObject &rt_data)
{
    ag_user_table user_tb;
    user_tb.read_account(m_db,s_data,rt_data);
}
void mysql::r_emu_action(QJsonObject &s_data, QJsonObject &rt_data)
{
    ag_gateway_data_table tmp_gt_dt_tb;

    tmp_gt_dt_tb.read_data(m_db,s_data,rt_data);
}

void mysql::r_emu_status(QJsonObject &s_data, QJsonObject &rt_data)
{
    ag_emu_status_table tmp_emu_status_tb;
    tmp_emu_status_tb.r_data(m_db,s_data,rt_data);
}
void mysql::r_mi_rack_data(QJsonObject &s_data, QJsonObject &rt_data)
{
    ag_rack_data_table tmp_rack_data_tb;

    tmp_rack_data_tb.read_data(m_db,s_data,rt_data);
}

void mysql::r_mapping(QString account, QJsonObject &rt_data,QStringList &mis_list)
{
    QJsonObject us_obj;
    QJsonArray st_array;
    QJsonArray datas;
    ag_user_station_table tmp_us_tb;
    ag_pos_table tmp_pos_table;

    QJsonObject st_obj;

    QString total_station;
    ag_user_table tmp_user_tb;

    tmp_user_tb.read_total_stations(m_db,account,total_station);

    tmp_us_tb.read_station(m_db,total_station,us_obj);

    st_array = us_obj.value("datas").toObject().value("station").toArray();
    for(int i=0;i<st_array.size();i++)
    {
        ag_station_emu_table tmp_st_emu_tb;
        ag_emu_property_table tmp_emu_pty_tb;
        ag_emu_status_table tmp_emu_sta_tb;
        ag_gateway_data_table tmp_emu_data_tb;
        QStringList emu_cid,emu_desc;
        QJsonArray emu_array;
        tmp_st_emu_tb.r_emu(m_db,st_array[i].toString(),emu_cid,emu_desc);
        for(int j=0;j<emu_cid.size();j++)
        {
            QJsonArray mis;
            ag_emu_mi_table tmp_emu_mi_tb;
            QStringList mi_cid,mi_desc;
            QJsonObject tmp_emu_pty_obj;
            QJsonObject tmp_emu_obj;

            tmp_emu_mi_tb.r_mi(m_db,emu_cid[j],mi_cid,mi_desc);
            for(int k=0;k<mi_cid.size();k++)
            {
                ag_mi_property_table tmp_mi_pty_tb;
                QJsonObject tmp_mi_pty_obj;
                QJsonObject tmp_mi_obj;
                tmp_mi_pty_tb.read_property(m_db,mi_cid[k],tmp_mi_pty_obj);
                tmp_mi_obj.insert(ag_mi_property_table::c_field_nominal_power,
                                  tmp_mi_pty_obj.value("datas").toObject().value(ag_mi_property_table::c_field_nominal_power).toInt());
                tmp_mi_obj.insert(ag_mi_property_table::c_field_mim_version,
                                  tmp_mi_pty_obj.value("datas").toObject().value(ag_mi_property_table::c_field_mim_version).toString());
                tmp_mi_obj.insert(ag_mi_property_table::c_field_mis_version,
                                  tmp_mi_pty_obj.value("datas").toObject().value(ag_mi_property_table::c_field_mis_version).toString());
                tmp_mi_obj.insert(ag_emu_mi_table::c_field_mi,mi_cid[k]);
                tmp_mi_obj.insert(ag_emu_mi_table::c_field_mi_desc,mi_desc[k]);

                mis.append(tmp_mi_obj);

                mis_list.append(mi_cid[k]);
            }

            QString status;
            QString status_reserve;
            QString run_mode;
            QString emu_sys_time;
            QString last_time = "";
            //读取最后异常action的通讯时间
            last_time = m_emucid_hand_lastTime.value(emu_cid[j],"");
            //内存不存在时  读库
            if(last_time == "")
            {
                tmp_emu_data_tb.read_last_hand_data_time(m_db,emu_cid[j],last_time);
            }

            tmp_emu_obj.insert("last_act_time",last_time);
            //读取网关最新工作状态
            tmp_emu_sta_tb.r_last_data(m_db,emu_cid[j],status,status_reserve,run_mode,emu_sys_time);
            tmp_emu_obj.insert(ag_emu_status_table::c_field_status,status);
            tmp_emu_obj.insert(ag_emu_status_table::c_field_run_mode,run_mode);
            tmp_emu_obj.insert("status_time",emu_sys_time);

            //读取网关属性
            tmp_emu_pty_tb.read_property(m_db,emu_cid[j],tmp_emu_pty_obj);
            tmp_emu_obj.insert(ag_emu_property_table::c_field_type,
                              tmp_emu_pty_obj.value("datas").toObject().value(ag_emu_property_table::c_field_type).toString());
            tmp_emu_obj.insert(ag_emu_property_table::c_field_hard_version,
                              tmp_emu_pty_obj.value("datas").toObject().value(ag_emu_property_table::c_field_hard_version).toString());
            tmp_emu_obj.insert(ag_emu_property_table::c_field_soft_version,
                              tmp_emu_pty_obj.value("datas").toObject().value(ag_emu_property_table::c_field_soft_version).toString());
            tmp_emu_obj.insert(ag_station_emu_table::c_field_emu,emu_cid[j]);
            tmp_emu_obj.insert(ag_station_emu_table::c_field_emu_desc,emu_desc[j]);
            tmp_emu_obj.insert("mis",mis);
            emu_array.append(tmp_emu_obj);
        }
        st_obj.insert("device",emu_array);
        st_obj.insert(ag_user_station_table::c_field_station,st_array[i].toString());

        datas.append(st_obj);
    }
    rt_data.insert("mapping_datas",datas);
}

void mysql::r_mi_current_power(QStringList mi_list, QJsonObject &rt_data)
{
    ag_power_data_table tmp_pw_tb;
    //读出功率数据
    tmp_pw_tb.read_current_data(m_db,mi_list,rt_data);
}
void mysql::r_mi_power(QJsonObject &s_data, QJsonObject &rt_data ,bool is_one)
{
    ag_power_data_table tmp_pw_tb;
    //读出功率数据
    tmp_pw_tb.read_data(m_db,s_data,rt_data,is_one);
}
void mysql::r_temp(QJsonObject &s_data, QJsonObject &rt_data)
{
    ag_temp_table tmp_tp_tb;

    tmp_tp_tb.read_temp(m_db,s_data,rt_data);
}
void mysql::r_temp(QString room_name, QString start_time, QString stop_time, QHash<QString, uint16_t> &temp)
{
    ag_temp_table tm_tb;
    tm_tb.read_temp(m_db,room_name,start_time,stop_time,temp);
}

void mysql::r_workorder(QJsonObject &s_data, QJsonObject &rt_data)
{
    ag_workorder_table tmp_work_tb;
    tmp_work_tb.read_workorder(m_db,s_data,rt_data);
}
void mysql::w_workorder(QJsonObject &s_data)
{
    ag_workorder_table tmp_work_tb;
    tmp_work_tb.write_workorder(m_db,s_data);
}
QString mysql::r_total_station(QString account)
{
    QString total_station;
    ag_user_table tmp_user_tb;

    tmp_user_tb.read_total_stations(m_db,account,total_station);

    return total_station;
}
void mysql::w_user_act(QString user, QString total_sta, int act, QString des)
{
    ag_user_act_table user_act_tb;
    QJsonObject param_obj;
    param_obj.insert("user",user);
    param_obj.insert("total_station",total_sta);
    param_obj.insert("act",act);
    param_obj.insert("act_desc",des);
    param_obj.insert("act_time",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    user_act_tb.write_datas(m_db,param_obj);

}

void mysql::r_mi_last_aging_param(QString mi, QString &room, QString &start_time, QString &stop_time, QString &pos_desc)
{
    ag_rack_data_table rk_dt_tb;
    rk_dt_tb.read_mi_last_aging_time_by_mi(m_db,mi,room,start_time,stop_time,pos_desc);
}

void mysql::r_aging_batch_by_room_start_time(QString mi, QString &room, QString start_time, QString &stop_time, QString &pos_desc)
{
   ag_rack_data_table rk_dt_tb;

   rk_dt_tb.read_mi_stop_time_after_start_time(m_db,mi,room,start_time,stop_time,pos_desc);
}
bool mysql::r_user_judge_params(QString user, QJsonObject &rt_data)
{
    ag_user_table us_tb;
    return us_tb.read_algorithm(m_db,user,rt_data);
}

void mysql::r_aging_data_by_mi_date(QString mi, QString start_time, QString stop_time,QJsonObject &rt_data)
{
    ag_power_data_table pw_dt_tb;
    ag_mi_property_table mi_pty_tb;
    //
    QJsonObject s_data;

    QJsonArray mi_cid;
    mi_cid.append(mi);
    s_data.insert("mi_cid",mi_cid);
    s_data.insert("start_date",start_time);
    s_data.insert("stop_date",stop_time);
    s_data.insert("nums","-1");
    bym_type mi_type = abstract_bym::analysis_type(mi);
    QJsonObject read_pv_data;
    QJsonArray datas;
    QHash<QString,int> mi_nominal_pw;
    QString st_time;
    mi_pty_tb.read_nominal_power(m_db,QStringList() << mi,mi_nominal_pw);
    for(int i=0;i<mi_type;i++)
    {
        QJsonObject pv_data;
        s_data.insert("pv_id",QString::number(i+1));

        pw_dt_tb.read_data(m_db,s_data,read_pv_data);

        pv_data.insert("pv_datas",read_pv_data.value("datas").toArray());
        pv_data.insert("pv_id",i+1);

        st_time = read_pv_data.value("stop_date").toString();

        datas.append(pv_data);
    }

    rt_data.insert("mi_cid",mi);
    rt_data.insert("all_pv",QString::number(mi_type));
    rt_data.insert("pv_nominal_power",mi_nominal_pw.value(mi,1000000));
    rt_data.insert("total_nominal_power",mi_nominal_pw.value(mi,1000000) * abstract_bym::analysis_type(mi));
    rt_data.insert("start_time",start_time);
    rt_data.insert("stop_time",st_time);
    rt_data.insert("datas",datas);
}
bool mysql::r_mi_report(QString mi, QString start_time,QString stop_time,QString alg, QString &report)
{
    ag_mi_report_table mi_report_tb;
    QVector<aging_report_strc> r_data;
    mi_report_tb.read_mi_report(m_db,mi,start_time,stop_time,r_data);

    for(int i=0;i<r_data.size();i++)
    {
        if(alg == r_data[i].alg)
        {
            report = r_data[i].report;
            return true;
        }
    }
    return false;
}

void mysql::r_aging_time(QString mi_cid, QString start_time, int &aging_time)
{
    ag_rack_extra_data_table rk_extra_data_table;

    rk_extra_data_table.read_data(m_db,mi_cid,start_time,aging_time);
}

void mysql::w_mi_report(QString mi, QString start_time, QString stop_time, QString alg, QString report)
{
    ag_mi_report_table mi_report_tb;
    aging_report_strc s_data;
    s_data.mi_cid = mi;
    s_data.start_time = start_time;
    s_data.stop_time = stop_time;
    s_data.alg = alg;
    s_data.report = report;
    s_data.update_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    mi_report_tb.write_mi_report(m_db,s_data);

}
bool mysql::update_alg(QString account, QJsonObject alg_data)
{
    ag_user_table us_tb;

    return us_tb.update_algorithm(m_db,account,alg_data);
}

void mysql::r_batch_list(QJsonObject &s_data, QJsonObject &rt_data)
{
    ag_rack_data_table rk_dt_tb;
    QStringList batch_list;  //开始时间列表
    rk_dt_tb.read_batch_list(m_db,s_data.value("start_time").toString(),s_data.value("stop_time").toString(),batch_list);

    QJsonArray batch_array;
    for(int i=0;i<batch_list.size();i++)
    {
        QStringList mi_list;
        QJsonObject one_batch_obj;
        QJsonArray mis_array;
        rk_dt_tb.read_mi_list(m_db,batch_list[i],mi_list);



        for(int j=0;j<mi_list.size();j++)
        {
            if(abstract_bym::is_cid_valid(mi_list[j]))
            {
                mis_array.append(mi_list[j]);
            }
        }

        one_batch_obj.insert("start_time",batch_list[i]);
        one_batch_obj.insert("mis",mis_array);
        batch_array.append(one_batch_obj);
    }

    rt_data.insert("batchs",batch_array);
}

void mysql::r_user_act(QJsonObject &s_data, QJsonObject &rt_data)
{
    ag_user_act_table us_act_tb;
    return us_act_tb.read_datas(m_db,s_data,rt_data);
}

void mysql::r_emu_property(QString emu_cid, QJsonObject &emu_pro_obj)
{
    ag_emu_property_table emu_pty_tb;
    emu_pty_tb.read_property(m_db,emu_cid,emu_pro_obj);
}

bool mysql::update_dev_control(QString total_station, QString station, QString emu,QString ctl_time, quint8 server_cmd, QString send_data)
{
    dev_ctl_strc tmp_ctl;

    if(!emu_protocolb::c_server_to_client.contains(server_cmd))
    {
        return false;
    }

    tmp_ctl.emu_cid = emu;
    tmp_ctl.is_cmd_send = 0;
    tmp_ctl.cmd_send_time = "";

    tmp_ctl.send_data = send_data;
    //tmp_ctl.is_send = s_data.value("is_send").toInt();
    tmp_ctl.is_data_send = 0;
    //tmp_ctl.send_time = s_data.value("send_time").toString();
    tmp_ctl.data_send_time = "";
    tmp_ctl.emu_cmd = emu_protocolb::c_server_to_client.value(server_cmd);
    //tmp_ctl.cmd_time = s_data.value("cmd_time").toString();
    //tmp_ctl.cmd_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    tmp_ctl.cmd_time = ctl_time;
    tmp_ctl.server_cmd = server_cmd;

    ag_user_station_table tmp_us_tb;
    ag_station_emu_table tmp_sta_emu_tb;
    if(tmp_us_tb.is_station_exist(m_db,total_station,station))
    {
        if(tmp_sta_emu_tb.is_emu_exist(m_db,station,emu))
        {
            ag_device_control_table dev_ctl_tb;
            //尝试更新最新的(指令顺序必须相邻)
            if(dev_ctl_tb.update_first_data(m_db,tmp_ctl))
            {
                return true;
            }
            else
            {
                //直接插入数据
                return dev_ctl_tb.w_data(m_db,tmp_ctl);
            }
        }
    }
    return false;
}

bool mysql::r_data_send_flag(QString emu_cid, QString data_send_flag)
{
    ag_device_control_table tmp_dev_ctl_tb;

    return tmp_dev_ctl_tb.is_data_send(m_db,emu_cid,data_send_flag);
}

void mysql::w_mi_temporary_power(QString name,QString data)
{
    ag_mi_extra_property_table tmp_mi_extra_pty_tb;
    tmp_mi_extra_pty_tb.w_temporary_power(m_db,name,data);

}

bool mysql::r_mi_temporary_power(QString name, QString &data)
{
    ag_mi_extra_property_table tmp_mi_extra_pty_tb;
    return tmp_mi_extra_pty_tb.r_temporary_power(m_db,name,data);
}

void mysql::w_mi_max_power(QString name, QString data)
{
    ag_mi_extra_property_table tmp_mi_extra_pty_tb;
    tmp_mi_extra_pty_tb.w_max_power(m_db,name,data);
}

bool mysql::r_mi_max_power(QString name, QString &data)
{
    ag_mi_extra_property_table tmp_mi_extra_pty_tb;
    return tmp_mi_extra_pty_tb.r_max_power(m_db,name,data);
}

void mysql::w_mi_grid(QString name, QString data)
{
    ag_mi_extra_property_table tmp_mi_extra_pty_tb;
    tmp_mi_extra_pty_tb.w_grid(m_db,name,data);
}

bool mysql::r_mi_grid(QString name, QString &data)
{
    ag_mi_extra_property_table tmp_mi_extra_pty_tb;
    return tmp_mi_extra_pty_tb.r_grid(m_db,name,data);
}

void mysql::w_mi_certification(QString name, QString data)
{
    ag_mi_extra_property_table tmp_mi_extra_pty_tb;
    tmp_mi_extra_pty_tb.w_certification(m_db,name,data);
}

bool mysql::r_mi_certification(QString name, QString &data)
{
    ag_mi_extra_property_table tmp_mi_extra_pty_tb;
    return tmp_mi_extra_pty_tb.r_certification(m_db,name,data);
}

void mysql::w_emu_func_code(QString name, QString data)
{
    ag_emu_extra_property_table tmp_emu_extra_pty_tb;
    tmp_emu_extra_pty_tb.w_countercurrent(m_db,name,data);
}

bool mysql::r_emu_func_code(QString name, QString &data)
{
    ag_emu_extra_property_table tmp_emu_extra_prt_tb;
    return tmp_emu_extra_prt_tb.r_countercurrent(m_db,name,data);
}


//bool mysql::w_device_ctl(QString user,QJsonObject &s_data)
//{
//    QString station = s_data.value("station").toString();
//    QString emu = s_data.value("emu_cid").toString();
//    ag_user_station_table tmp_us_tb;
//    ag_station_emu_table tmp_sta_emu_tb;

//    QString total_station;
//    ag_user_table tmp_user_tb;

//    tmp_user_tb.read_total_stations(m_db,user,total_station);

//    if(tmp_us_tb.is_station_exist(m_db,total_station,station))
//    {
//        if(tmp_sta_emu_tb.is_emu_exist(m_db,station,emu))
//        {
//            ag_device_control_table dev_ctl_tb;
//            dev_ctl_strc tmp_ctl;
//            tmp_ctl.emu_cid = s_data.value("emu_cid").toString();
//            tmp_ctl.send_data = s_data.value("send_data").toString();
//            //tmp_ctl.is_send = s_data.value("is_send").toInt();
//            tmp_ctl.is_send = 0;
//            //tmp_ctl.send_time = s_data.value("send_time").toString();
//            tmp_ctl.send_time = "";
//            tmp_ctl.emu_cmd = s_data.value("emu_cmd").toInt();
//            //tmp_ctl.cmd_time = s_data.value("cmd_time").toString();
//            tmp_ctl.cmd_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//            tmp_ctl.server_cmd = s_data.value("server_cmd").toInt();

//            return dev_ctl_tb.w_data(m_db,tmp_ctl);
//        }
//    }

//    return false;
//}

void mysql::r_device_ctl_first_cmd_nosend(QString emu_cid, dev_ctl_strc &ctl_data)
{
    ag_device_control_table dev_ctl_tb ;
    dev_ctl_tb.r_first_cmd_nosend(m_db,emu_cid,ctl_data);
}

void mysql::r_device_ctl_last_data_nosend(QString emu_cid,uint8_t cmd, dev_ctl_strc &ctl_data)
{
    ag_device_control_table dev_ctl_tb;
    dev_ctl_tb.r_last_data_nosend(m_db,emu_cid,cmd,ctl_data);
}

void mysql::update_device_cmd_send_flag(dev_ctl_strc ctl_data)
{
    ag_device_control_table dev_ctl_tb;
    dev_ctl_tb.update_cmd_send_flag(m_db,ctl_data);
}

void mysql::update_device_data_send_flag(dev_ctl_strc ctl_data)
{
    ag_device_control_table dev_ctl_tb;

    dev_ctl_tb.update_data_send_flag(m_db,ctl_data);
}


bool mysql::set_current_database(QString database_name)
{
    QSqlQuery query(m_db);
    if(query.exec("USE " + database_name))
    {
        //QLOG_DEBUG() <<  QString("正在使用 %1 数据库").arg(m_database);
        return true;
    }
    else {
        QLOG_WARN() << QString("无法使用 %1 数据库").arg(database_name);
        return false;
    }
    return false;

}
bool mysql::create_database(QString database_name)
{
    QSqlQuery query = QSqlQuery(m_db);
    if(query.exec("CREATE DATABASE IF NOT EXISTS "+ database_name +" CHARACTER SET utf8mb4"))
    {
        QLOG_INFO() << QString("创建数据库:%1 成功").arg(database_name);
    }
    else {
        QLOG_WARN() << QString("创建数据库: %1 失败").arg(database_name);
        return false;
    }
    return true;
}

bool mysql::delete_database(QString database_name)
{
    QSqlQuery query = QSqlQuery(m_db);
    if(query.exec("DROP DATABASE IF EXISTS " + database_name))
    {
        QLOG_INFO() << QString("删除数据库:%1 成功").arg(database_name);
    }
    else {
        QLOG_WARN() << QString("删除数据库: %1 失败").arg(database_name);
        return false;
    }
    return true;
}






