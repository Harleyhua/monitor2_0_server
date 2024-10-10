#include "devmgservice.h"

#include "emu_protocolb.h"

#include "devdataservice.h"

#include <QMutex>
#include "abstract_bym.h"
#include "abstract_emu.h"
#include "QsLog.h"
#include "mysql.h"

extern QMutex emu_act_cache_lock;

devmgservice::devmgservice(QSqlDatabase &DataBase, QObject *parent)
    : QObject{parent}
{
    mDataBase = DataBase;
}

void devmgservice::addDevCtl(QString totalStation, QString station, QString emu, QString ctlTime, quint8 serverCmd, QString sendData)
{
    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;
    ag_device_control_table devCtlTable;
    dev_ctl_strc tmpCtl;
    //判断 服务器命令是否合法
    if(!emu_protocolb::c_server_to_client.contains(serverCmd))
    {
        return ;
    }

    tmpCtl.emu_cid = emu;
    tmpCtl.is_cmd_send = 0;
    tmpCtl.cmd_send_time = "";
    tmpCtl.send_data = sendData;
    //tmp_ctl.is_send = s_data.value("is_send").toInt();
    tmpCtl.is_data_send = 0;
    //tmp_ctl.send_time = s_data.value("send_time").toString();
    tmpCtl.data_send_time = "";
    tmpCtl.emu_cmd = emu_protocolb::c_server_to_client.value(serverCmd);
    //tmp_ctl.cmd_time = s_data.value("cmd_time").toString();
    //tmp_ctl.cmd_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    tmpCtl.cmd_time = ctlTime;
    tmpCtl.server_cmd = serverCmd;

    if(!userStationTable.is_station_exist(mDataBase,totalStation,station))
    {
        return ;
    }
    if(!stationEmuTable.is_emu_exist(mDataBase,station,emu))
    {
        return ;
    }
    if(serverCmd == 0x01 && abstract_emu::is_wifiemu_cid_valid(emu))
    {
        return ;
    }
    //尝试更新最新的(指令顺序必须相邻)  不是最新的更新
    if(!devCtlTable.update_first_data(mDataBase,tmpCtl))
    {
        devCtlTable.w_data(mDataBase,tmpCtl);
    }
}

void devmgservice::getOneControlResuletBack(QString field, const QJsonObject &s, QJsonObject &retData)
{
    QString rctltime,vdata;
    QString rttime,data;
    void (ag_mi_extra_property_table::*f)(QSqlDatabase&,QString, QString &);

    QJsonObject dataJsObj;
    ag_device_control_table devCtlTable;
    ag_mi_extra_property_table miExtraPropertyTable;
    QString dataParam;
    QJsonArray misArray;
    QJsonObject miJsObj;
    QJsonArray misRetArray;
    QJsonObject r_mxopret_obj;

    // 根据传入的field参数，设置相应的查询参数和函数指针
    if(field == "r_maxop")
    {
        rctltime = "r_maxop_ctl_time";
        vdata = "v_max_power";
        rttime = "r_maxop_time";
        data = "mis_maxop";
        f = &ag_mi_extra_property_table::r_max_power;
    }
    else if(field == "r_cop")
    {
        rctltime = "r_cop_ctl_time";
        vdata = "v_cop";
        rttime = "r_cop_time";
        data = "mis_cop";
        f = &ag_mi_extra_property_table::r_temporary_power;
    }
    else if(field == "r_grid")
    {
        rctltime = "r_grid_ctl_time";
        vdata = "v_grid";
        rttime = "r_grid_time";
        data = "mis_grid";
        f = &ag_mi_extra_property_table::r_grid;
    }
    else if(field == "r_cer")
    {
        rctltime = "r_cer_ctl_time";
        vdata = "v_cer";
        rttime = "r_cer_time";
        data = "mis_cer";
        f = &ag_mi_extra_property_table::r_certification;
    }
    else if(field == "w_maxop")
    {
        rctltime = "maxop_ctl_time";
        vdata = "v_max_power";
        rttime = "w_maxop_time";
        data = "mis_maxop";
        f = &ag_mi_extra_property_table::r_max_power;
    }
    else if(field == "w_cop")
    {
        rctltime = "cop_ctl_time";
        vdata = "v_cop";
        rttime = "w_cop_time";
        data = "mis_cop";
        f = &ag_mi_extra_property_table::r_temporary_power;
    }
    else if(field == "w_grid")
    {
        rctltime = "grid_ctl_time";
        vdata = "v_grid";
        rttime = "w_grid_time";
        data = "mis_grid";
        f = &ag_mi_extra_property_table::r_grid;
    }
    else if(field == "w_cer")
    {
        rctltime = "cer_ctl_time";
        vdata = "mis_cer";
        rttime = "w_cer_time";
        data = "mis_cer";
        f = &ag_mi_extra_property_table::r_certification;
    }
    else
    {
        return;
    }

    // 从传入的JSON对象中获取mis数组
    dataJsObj = s.value(field).toObject();
    misArray = dataJsObj.value("mis").toArray();

    // 如果控制时间字段不为空，并且数据尚未发送
    if(dataJsObj.value(rctltime).toString() != "" &&
       devCtlTable.is_data_send(mDataBase,s.value("emu_cid").toString(),dataJsObj.value(rctltime).toString()))
    {
        // 遍历mis数组
        for(int i=0;i<misArray.size();i++)
        {
            dataParam = "";

            (miExtraPropertyTable.*f)(mDataBase,misArray[i].toString(),dataParam);


            miJsObj.insert("mi_cid",misArray[i].toString());
            miJsObj.insert(vdata,dataParam);

            misRetArray.append(miJsObj);
        }

        r_mxopret_obj.insert(rttime,dataJsObj.value(rctltime).toString());
        r_mxopret_obj.insert(data,misRetArray);

        retData.insert(field,r_mxopret_obj);
    }
}

void devmgservice::DevMapping(QString totalStation, QStringList &retMis, QJsonObject &retData)
{
    QJsonObject stationsJsObj;
    QJsonArray stationsArray;

    devdataservice devDataSv(mDataBase);
    ag_user_station_table userStationTable;
    ag_emu_mi_table emuMiTable;
    ag_gateway_data_table emuDataTable;
    ag_emu_handshake_table emu_hs_Table;
    ag_station_emu_table stationEmuTable;

    QJsonArray mapDataArray;

    //获取电站列表
    userStationTable.read_station(mDataBase,totalStation,stationsJsObj);
    stationsArray = stationsJsObj.value("datas").toObject().value("station").toArray();

    for(int i=0;i<stationsArray.size();i++)
    {
        QJsonObject retStationJsObj;
        QStringList emus,emusDesc;
        QJsonObject emuJsObj;
        QJsonArray emuArray;
        stationEmuTable.r_emu(mDataBase,stationsArray[i].toString(),emus,emusDesc);
        for(int j=0;j<emus.size();j++)
        {
            QStringList mis,misDesc;
            QJsonArray misArray;
            QString emuLastTime;

            emuMiTable.r_mi(mDataBase,emus[j],mis,misDesc);
            for(int k=0;k<mis.size();k++)
            {
                QJsonObject MiPropertyJsObj;
                //读取微逆属性
                devDataSv.readMiProperty(mis[k],MiPropertyJsObj);
                MiPropertyJsObj.insert(ag_emu_mi_table::c_field_mi_desc,misDesc[k]);

                misArray.append(MiPropertyJsObj);
                retMis.append(mis[k]);
            }

            //读取最后异常action的通讯时间
            emuLastTime = mysql::m_emucid_hand_lastTime.value(emus[j],"");
            //内存不存在时  读库
            if(emuLastTime == "")
            {
                //emuDataTable.read_last_hand_data_time(mDataBase,emus[j],emuLastTime);
                emu_hs_Table.read_last_hand_data_time(mDataBase,emus[j],emuLastTime);
                if(emu_act_cache_lock.tryLock(1000))
                {
                    mysql::m_emucid_hand_lastTime.insert(emus[j],emuLastTime);
                    emu_act_cache_lock.unlock();
                }
                    //QLOG_INFO() << "no cache read " + emus[j] + "time:" + emuLastTime;
            }

            //插入网关的最后通讯时间
            emuJsObj.insert("last_act_time",emuLastTime);
            //读取网关最新工作状态
            devDataSv.readEmuLastStatus(emus[j],emuJsObj);
            //读完网关属性
            devDataSv.readEmuProperty(emus[j],emuJsObj);
            emuJsObj.insert(ag_station_emu_table::c_field_emu_desc,emusDesc[j]);

            emuJsObj.insert("mis",misArray);
            emuArray.append(emuJsObj);

        }
        retStationJsObj.insert("device",emuArray);
        retStationJsObj.insert(ag_user_station_table::c_field_station,stationsArray[i].toString());

        mapDataArray.append(retStationJsObj);

    }

    retData.insert("mapping_datas",mapDataArray);
}

//添加电站函数
void devmgservice::addStation(QString totalStation, const QJsonObject &s)
{
    ag_user_station_table userStationTable;

    userStationTable.write_station(mDataBase,totalStation,s);
}

void devmgservice::delStation(QString totalStation, const QJsonObject &s)
{
    QString station = s.value("station").toString();

    delStation(totalStation,station);
}

void devmgservice::addStation(QString totalStation, QString station)
{
   ag_user_station_table userStationTable;

   userStationTable.write_station(mDataBase,totalStation,station);
}

void devmgservice::delStation(QString totalStation, QString station)
{
    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;
    QStringList emus;
    QStringList emuDesc;

    stationEmuTable.r_emu(mDataBase,station,emus,emuDesc);
    for(int i=0;i<emus.size();i++)
    {
        delEmu(totalStation,station,emus[i]);
    }

    userStationTable.del_station_by_station(mDataBase,station);
}

void devmgservice::addOneEmu(QString station, QString emuCid, QString desc)
{
    ag_station_emu_table stationEmuTable;
    stationEmuTable.w_one_emu(mDataBase,station,emuCid,desc);
}

void devmgservice::addEmu(QString totalStation, const QJsonObject &s)
{
    QString station = s.value("station").toString();
    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;
    //电站存在 则允许添加网关
    if(userStationTable.is_station_exist(mDataBase,totalStation,station))
    {
        stationEmuTable.w_emu(mDataBase,station,s);
    }

}

void devmgservice::delEmu(QString totalStation, const QJsonObject &s)
{
    QString station,emu;
    station = s.value("station").toString();
    emu = s.value("emu_cid").toString();

    delEmu(totalStation,station,emu);
}

void devmgservice::delEmu(QString totalStation, QString station, QString emu)
{
    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;
    ag_emu_mi_table emuMiTable;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//    QString station,emu;
//    station = s.value("station").toString();
//    emu = s.value("emu_cid").toString();
    if(!userStationTable.is_station_exist(mDataBase,totalStation,station))
    {
        return;
    }

    if(!stationEmuTable.is_emu_exist(mDataBase,station,emu))
    {
        return;
    }

    mDataBase.transaction();
    emuMiTable.del_mis_by_emucid(mDataBase,emu);
    stationEmuTable.del_emu_by_emucid(mDataBase,station,emu);
    addDevCtl(totalStation,station,emu,time,0x01,"");
    mDataBase.commit();
}

void devmgservice::addOnePlcMi(QString totalStation, QString station, QString emu, QString mi, QString miDesc)
{
    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;
    ag_emu_mi_table emuMiTable;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //电站是否存在
    if(userStationTable.is_station_exist(mDataBase,totalStation,station))
    {
        //判断网关是否存在
        if(stationEmuTable.is_emu_exist(mDataBase,station,emu))
        {
            emuMiTable.w_one_mi(mDataBase,emu,mi,miDesc);
            //远程控制  通知列表变化
            //addDevCtl(totalStation,station,emu,time,0x01,"");
        }
    }
}

void devmgservice::addPlcMi(QString totalStation, const QJsonObject &s)
{
    QString station = s.value("station").toString();
    QString emu = s.value("emu").toString();
    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;
    ag_emu_mi_table emuMiTable;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //电站是否存在
    if(userStationTable.is_station_exist(mDataBase,totalStation,station))
    {
        //判断网关是否存在
        if(stationEmuTable.is_emu_exist(mDataBase,station,emu))
        {
            emuMiTable.w_mi(mDataBase,emu,s);
            //远程控制  通知列表变化
            addDevCtl(totalStation,station,emu,time,0x01,"");
        }
    }

}

void devmgservice::addOneWiFiMi(QString totalStation, QString station, QString emu, QString mi, QString miDesc)
{
    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;
    ag_emu_mi_table emuMiTable;
    if(userStationTable.is_station_exist(mDataBase,totalStation,station))
    {
        if(!abstract_emu::is_wifiemu_cid_valid(emu) || !abstract_bym::is_cid_valid(mi))
            return;
        //开启事务
        mDataBase.transaction();

        stationEmuTable.w_one_emu(mDataBase,station,emu,"");
        //
        //if(stationEmuTable.is_emu_exist(mDataBase,station,tmpEmu))
        {
            emuMiTable.w_one_mi(mDataBase,emu,mi,miDesc);
        }
        mDataBase.commit();
    }
}

void devmgservice::addWiFiMi(QString totalStation, const QJsonObject &s)
{
    QString station = s.value("station").toString();
    QJsonArray emuArray = s.value("emus").toArray();

    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;
    ag_emu_mi_table emuMiTable;


    for(int i=0;i<emuArray.size();i++)
    {
        QString tmpEmu = emuArray[i].toObject().value("emu").toString();
        QString mi = emuArray[i].toObject().value("mi").toObject().value("mi_cid").toString();
        QString miDesc = emuArray[i].toObject().value("mi").toObject().value("mi_desc").toString();
        if(userStationTable.is_station_exist(mDataBase,totalStation,station))
        {

            if(!abstract_emu::is_wifiemu_cid_valid(tmpEmu) || !abstract_bym::is_cid_valid(mi))
                continue;
            //开启事务
            mDataBase.transaction();

            stationEmuTable.w_one_emu(mDataBase,station,tmpEmu,"");
            //
            //if(stationEmuTable.is_emu_exist(mDataBase,station,tmpEmu))
            {
                emuMiTable.w_one_mi(mDataBase,tmpEmu,mi,miDesc);
            }
            mDataBase.commit();
        }
    }
}

void devmgservice::delMi(QString totalStation, const QJsonObject &s)
{
    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;
    ag_emu_mi_table EmuMiTable;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString station,emu,mi;
    station = s.value("station").toString();
    emu = s.value("emu_cid").toString();
    mi = s.value("mi_cid").toString();
    //
    if(!userStationTable.is_station_exist(mDataBase,totalStation,station))
    {
        return;
    }

    if(stationEmuTable.is_emu_exist(mDataBase,station,emu))
    {
        //开启事务
        mDataBase.transaction();
        if(abstract_emu::is_wifiemu_cid_valid(emu))
        {
            stationEmuTable.del_emu_by_emucid(mDataBase,station,emu);
        }

        EmuMiTable.del_one_mi_by_micid(mDataBase,emu,mi);

        addDevCtl(totalStation,station,emu,time,0x01,"");
        mDataBase.commit();
    }

}

void devmgservice::delMiFromEmu(QString station, QString totalstation, QString emu)
{
    ag_emu_mi_table emuMiTable;
    QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    emuMiTable.del_mis_by_emucid(mDataBase,emu);
    addDevCtl(totalstation,station,emu,curTime,0x01,"");
}
