#include "user_service.h"


#include "ag_user_table.h"
#include "ag_user_act_table.h"

#include "ag_emu_extra_property_table.h"
#include "ag_mi_extra_property_table.h"
#include "ag_device_control_table.h"
#include "ag_gateway_data_table.h"
#include "ag_emu_status_table.h"

#include "ag_rack_data_table.h"
#include "ag_power_data_table.h"
#include "ag_temp_table.h"
#include "ag_workorder_table.h"
#include "ag_user_table.h"
#include "ag_user_act_table.h"
#include "ag_user_station_table.h"
#include "ag_station_emu_table.h"
#include "ag_emu_mi_table.h"

//数据上报服务
//数据读取服务    业务逻辑的抽象： 数据读取服务可能提供了一些抽象层，用于处理通用的数据查询逻辑。这可以使各个服务共享相同的数据查询逻辑，而不需要每个服务都自己实现相似的查询操作。
//网关管理服务
//微逆管理服务
//用户管理服务
//客户端服务

user_service::user_service(QString name,QObject *parent)
    : QObject{parent},mSql(name)
{



}

QString user_service::getTotalStation(QString userName)
{
    QString totalStation;
    ag_user_table userTable;

    userTable.read_total_stations(mSql.db(),userName,totalStation);

    return totalStation;
}

bool user_service::getUserProperty(const QJsonObject &param, QJsonObject &ret)
{
    ag_user_table userTable;

    return userTable.read_account(mSql.db(),param,ret);
}

bool user_service::getUserOperationRecord(QString userName, const QJsonObject &param, QJsonObject &ret)
{
    ag_user_table userTable;
    ag_user_act_table userActTable;
    QJsonObject tmpParam = param;
    QString totalStation = getTotalStation(userName);
    tmpParam.insert("total_station",totalStation);

    return userActTable.read_datas(mSql.db(),tmpParam,ret);
}

bool user_service::setUserAlgorithm(QString name, const QJsonObject &param, QJsonObject &ret)
{
    ag_user_table userTable;

    if(userTable.update_algorithm(mSql.db(),name,param))
    {
        ret.insert("ret",true);
        return true;
    }
    else
    {
        ret.insert("ret",false);
        return false;
    }
}

bool user_service::setEmuProperty(QString userName, QJsonObject &param)
{
    ag_emu_extra_property_table emuExtraPropertyTable;

    QString totalStation = getTotalStation(userName);
    //写入网关功能码
    emuExtraPropertyTable.w_countercurrent(mSql.db(),param.value("emu_cid").toString(),
                                                     param.value("func").toString());

    mSql.update_dev_control(totalStation,param.value("station").toString(),
                           param.value("emu_cid").toString(),
                           QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),
                           param.value("server_cmd").toInt(),
                           param.value("data").toString());

    return true;
}

bool user_service::setMiTemporaryPower(QString userName, const QJsonObject &param, QJsonObject &ret)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    QString totalStation = getTotalStation(userName);
    QString data = param.value("temporary_power").toString();

    miExtraPropertyTable.w_temporary_power(mSql.db(),param.value("mi_cid").toString(),data);

    mSql.update_dev_control(totalStation,param.value("station").toString(),
                            param.value("emu_cid").toString(),
                            param.value("ctl_time").toString(),
                            param.value("server_cmd").toInt(),
                            param.value("mi_cid").toString());

    ret.insert("station",param.value("station").toString());
    ret.insert("emu_cid",param.value("emu_cid").toString());
    ret.insert("mi_cid",param.value("mi_cid").toString());

    return true;

}

bool user_service::getMiTemporaryPower(QString userName, const QJsonObject &param, QJsonObject &ret)
{

    ag_mi_extra_property_table miExtraPropertyTable;
    QString ret_data;
    QString totalStation = getTotalStation(userName);

    miExtraPropertyTable.r_temporary_power(mSql.db(),param.value("mi_cid").toString(),ret_data);

    mSql.update_dev_control(totalStation,param.value("station").toString(),
                           param.value("emu_cid").toString(),
                           param.value("ctl_time").toString(),
                           param.value("server_cmd").toInt(),
                           param.value("data").toString());

    ret.insert("station",ret.value("station").toString());
    ret.insert("emu_cid",ret.value("emu_cid").toString());
    ret.insert("mi_cid",ret.value("mi_cid").toString());
    ret.insert("temporary_power",ret_data);

    return true;
}

bool user_service::setMiMaxPower(QString userName, const QJsonObject &param, QJsonObject &ret)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    QString totalStation = getTotalStation(userName);
    QString data = param.value("max_power").toString();
    miExtraPropertyTable.w_max_power(mSql.db(),param.value("mi_cid").toString(),data);

    mSql.update_dev_control(totalStation,param.value("station").toString(),
                            param.value("emu_cid").toString(),
                            param.value("ctl_time").toString(),
                            param.value("server_cmd").toInt(),
                            param.value("mi_cid").toString());


    ret.insert("station",param.value("station").toString());
    ret.insert("emu_cid",param.value("emu_cid").toString());
    ret.insert("mi_cid",param.value("mi_cid").toString());

    return true;
}

bool user_service::getMiMaxPower(QString userName, const QJsonObject &param, QJsonObject &ret)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    QString ret_data;
    QString totalStation = getTotalStation(userName);

    miExtraPropertyTable.r_max_power(mSql.db(),param.value("mi_cid").toString(),ret_data);

    mSql.update_dev_control(totalStation,param.value("station").toString(),
                           param.value("emu_cid").toString(),
                           param.value("ctl_time").toString(),
                           param.value("server_cmd").toInt(),
                           param.value("data").toString());


    ret.insert("station",param.value("station").toString());
    ret.insert("emu_cid",param.value("emu_cid").toString());
    ret.insert("mi_cid",param.value("mi_cid").toString());
    ret.insert("max_power",ret_data);

    return true;
}

bool user_service::setMiGridParam(QString userName, const QJsonObject &param, QJsonObject &ret)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    QString totalStation = getTotalStation(userName);
    QString data = param.value("grid").toString();
    miExtraPropertyTable.w_grid(mSql.db(),param.value("mi_cid").toString(),data);

    mSql.update_dev_control(totalStation,param.value("station").toString(),
                            param.value("emu_cid").toString(),
                            param.value("ctl_time").toString(),
                            param.value("server_cmd").toInt(),
                            param.value("mi_cid").toString());

    ret.insert("station",param.value("station").toString());
    ret.insert("emu_cid",param.value("emu_cid").toString());
    ret.insert("mi_cid",param.value("mi_cid").toString());

    return true;
}

bool user_service::getMiGridParam(QString userName, const QJsonObject &param, QJsonObject &ret)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    QString ret_data;
    QString totalStation = getTotalStation(userName);

    miExtraPropertyTable.r_grid(mSql.db(),param.value("mi_cid").toString(),ret_data);

    mSql.update_dev_control(totalStation,param.value("station").toString(),
                           param.value("emu_cid").toString(),
                           param.value("ctl_time").toString(),
                           param.value("server_cmd").toInt(),
                           param.value("data").toString());

    ret.insert("station",param.value("station").toString());
    ret.insert("emu_cid",param.value("emu_cid").toString());
    ret.insert("mi_cid",param.value("mi_cid").toString());
    ret.insert("grid",ret_data);

    return true;

}

bool user_service::setMiCertification(QString userName, const QJsonObject &param, QJsonObject &ret)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    QString totalStation = getTotalStation(userName);
    QString data = param.value("certification").toString();
    miExtraPropertyTable.w_certification(mSql.db(),param.value("mi_cid").toString(),data);

    mSql.update_dev_control(totalStation,param.value("station").toString(),
                           param.value("emu_cid").toString(),
                           param.value("ctl_time").toString(),
                           param.value("server_cmd").toInt(),
                           param.value("mi_cid").toString());

    ret.insert("station",param.value("station").toString());
    ret.insert("emu_cid",param.value("emu_cid").toString());
    ret.insert("mi_cid",param.value("mi_cid").toString());

    return true;
}

bool user_service::getMiCertification(QString userName, const QJsonObject &param, QJsonObject &ret)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    QString ret_data;
    QString totalStation = getTotalStation(userName);

    miExtraPropertyTable.r_certification(mSql.db(),param.value("mi_cid").toString(),ret_data);

    mSql.update_dev_control(totalStation,param.value("station").toString(),
                           param.value("emu_cid").toString(),
                           param.value("ctl_time").toString(),
                           param.value("server_cmd").toInt(),
                           param.value("data").toString());

    ret.insert("station",param.value("station").toString());
    ret.insert("emu_cid",param.value("emu_cid").toString());
    ret.insert("mi_cid",param.value("mi_cid").toString());
    ret.insert("certification",ret_data);

    return true;
}

bool user_service::getControlResultBack(const QJsonObject &param, QJsonObject &ret)
{
    QString maxop_data,cop_data,grid_data,cer_data,
            r_maxop_data,r_cop_data,r_grid_data,r_cer_data;

    ag_device_control_table deviceCtlTable;
    ag_mi_extra_property_table miExtraPropertyTable;

    if(param.value("r_maxop_ctl_time").toString() != "" &&
        deviceCtlTable.is_data_send(mSql.db(),param.value("emu_cid").toString(),param.value("r_maxop_ctl_time").toString()))
    {
        miExtraPropertyTable.r_max_power(mSql.db(),param.value("mi_cid").toString(),r_maxop_data);
        ret.insert("r_max_power",r_maxop_data);
    }
    if(param.value("r_cop_ctl_time").toString() != "" &&
            deviceCtlTable.is_data_send(mSql.db(),param.value("emu_cid").toString(),param.value("r_cop_ctl_time").toString()))
    {
        miExtraPropertyTable.r_temporary_power(mSql.db(),param.value("mi_cid").toString(),r_cop_data);
        ret.insert("r_temporary_power",r_cop_data);
    }
    if(param.value("r_grid_ctl_time").toString() != "" &&
            deviceCtlTable.is_data_send(mSql.db(),param.value("emu_cid").toString(),param.value("r_grid_ctl_time").toString()))
    {
        miExtraPropertyTable.r_grid(mSql.db(),param.value("mi_cid").toString(),r_grid_data);
        ret.insert("r_grid",r_grid_data);
    }
    if(param.value("r_cer_ctl_time").toString() != "" &&
            deviceCtlTable.is_data_send(mSql.db(),param.value("emu_cid").toString(),param.value("r_cer_ctl_time").toString()))
    {
        miExtraPropertyTable.r_certification(mSql.db(),param.value("mi_cid").toString(),r_cer_data);
        ret.insert("r_certification",r_cer_data);
    }

    if(param.value("maxop_ctl_time").toString() != "" &&
            deviceCtlTable.is_data_send(mSql.db(),param.value("emu_cid").toString(),param.value("maxop_ctl_time").toString()))
    {
        miExtraPropertyTable.r_max_power(mSql.db(),param.value("mi_cid").toString(),maxop_data);
        ret.insert("max_power",maxop_data);
    }
    if(param.value("cop_ctl_time").toString() != "" &&
            deviceCtlTable.is_data_send(mSql.db(),param.value("emu_cid").toString(),param.value("cop_ctl_time").toString()))
    {
        miExtraPropertyTable.r_temporary_power(mSql.db(),param.value("mi_cid").toString(),cop_data);
        ret.insert("temporary_power",cop_data);
    }
    if(param.value("grid_ctl_time").toString() != "" &&
            deviceCtlTable.is_data_send(mSql.db(),param.value("emu_cid").toString(),param.value("grid_ctl_time").toString()))
    {
        miExtraPropertyTable.r_grid(mSql.db(),param.value("mi_cid").toString(),grid_data);
        ret.insert("grid",grid_data);
    }
    if(param.value("cer_ctl_time").toString() != "" &&
            deviceCtlTable.is_data_send(mSql.db(),param.value("emu_cid").toString(),param.value("cer_ctl_time").toString()))
    {
        miExtraPropertyTable.r_certification(mSql.db(),param.value("mi_cid").toString(),cer_data);
        ret.insert("certification",cer_data);
    }

    ret.insert("station",param.value("station").toString());
    ret.insert("emu_cid",param.value("emu_cid").toString());
    ret.insert("mi_cid",param.value("mi_cid").toString());

    ret.insert("r_maxop_ctl_time",param.value("r_maxop_ctl_time").toString());
    ret.insert("r_cop_ctl_time",param.value("r_cop_ctl_time").toString());
    ret.insert("r_grid_ctl_time",param.value("r_grid_ctl_time").toString());
    ret.insert("r_cer_ctl_time",param.value("r_cer_ctl_time").toString());
    ret.insert("maxop_ctl_time",param.value("maxop_ctl_time").toString());
    ret.insert("cop_ctl_time",param.value("cop_ctl_time").toString());
    ret.insert("grid_ctl_time",param.value("grid_ctl_time").toString());
    ret.insert("cer_ctl_time",param.value("cer_ctl_time").toString());

    return true;
}

bool user_service::getEmuAction(const QJsonObject &param, QJsonObject &ret)
{
    ag_gateway_data_table emuDataTable;

    return emuDataTable.read_data(mSql.db(),param,ret);

}

bool user_service::getEmuStatus(const QJsonObject &param, QJsonObject &ret)
{
    ag_emu_status_table emuStatusTable;

    return emuStatusTable.r_data(mSql.db(),param,ret);
}

bool user_service::getRackData(const QJsonObject &param, QJsonObject &ret)
{
    ag_rack_data_table rackDataTable;

    return rackDataTable.read_data(mSql.db(),param,ret);

}

bool user_service::getMIPower(const QJsonObject &param,bool is_one, QJsonObject &ret)
{
    ag_power_data_table powerDataTable;

    return powerDataTable.read_data(mSql.db(),param,ret,is_one);

}

bool user_service::getRoomTemp(const QJsonObject &param, QJsonObject &ret)
{
    ag_temp_table tempTable;

    return tempTable.read_temp(mSql.db(),param,ret);

}

bool user_service::getWorkOrder(const QJsonObject &param, QJsonObject &ret)
{
    ag_workorder_table workOrderTable;

    return workOrderTable.read_workorder(mSql.db(),param,ret);
}

bool user_service::setWorkOrder(const QJsonObject &param)
{
    ag_workorder_table workOrderTable;

    return workOrderTable.write_workorder(mSql.db(),param);
}

bool user_service::addStation(QString userName, const QJsonObject &param)
{
    QString totalStation = getTotalStation(userName);
    QJsonArray stationArray = param.value("station").toArray();
    QString userActStr;
    ag_user_station_table userStationTable;

    userActStr = "添加电站列表 ";
    for(int i=0;i<stationArray.size();i++)
    {
        userActStr.append(stationArray[i].toString() + ",");
    }

    mSql.w_user_act(userName,totalStation,2,userActStr);

    return userStationTable.write_station(mSql.db(),totalStation,param);
}

bool user_service::delStation(QString userName, const QJsonObject &param)
{
    QString totalStation = getTotalStation(userName);
    QString userActStr;

    userActStr = "删除电站" + param.value("station").toString();

    mSql.w_user_act(userName,totalStation,1,userActStr);

    return mSql.del_station(userName,param);
}

bool user_service::addEmu(QString userName, const QJsonObject &param)
{
    QString totalStation = getTotalStation(userName);
    QString userActStr;
    QJsonArray emuArray = param.value("emus").toArray();
    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;

    QString clidStation;

    userActStr.append("添加电站 ");
    userActStr.append(param.value("station").toString());
    userActStr.append("下的网关 ");
    for(int i=0;i<emuArray.size();i++)
    {
        userActStr.append(emuArray[i].toObject().value("emu_cid").toString() + ",");
    }
    mSql.w_user_act(userName,totalStation,2,userActStr);

    //添加关系
    //如果当前电站存在
    if(userStationTable.is_station_exist(mSql.db(),totalStation,clidStation))
    {
        return stationEmuTable.w_emu(mSql.db(),clidStation,param);
    }

    return false;
}

bool user_service::delEmu(QString userName, const QJsonObject &param)
{
    QString totalStation = getTotalStation(userName);
    QString userActStr;
    userActStr.append("删除");
    userActStr.append(param.value("station").toString());
    userActStr.append("下的网关:");
    userActStr.append(param.value("emu_cid").toString());

    mSql.w_user_act(userName,totalStation,1,userActStr);

    return mSql.del_emu(userName,param);
}

bool user_service::addPlcMi(QString userName, const QJsonObject &param)
{
    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;
    ag_emu_mi_table emuMiTable;
    QString totalStation = getTotalStation(userName);
    QString userActStr;
    QJsonArray miArray = param.value("mis").toArray();
    userActStr.append("添加电站:");
    userActStr.append(param.value("station").toString());
    userActStr.append("网关:");
    userActStr.append(param.value("emu").toString());
    userActStr.append("下的PLC版微逆:");
    for(int i=0;i<miArray.size();i++)
    {
        userActStr.append(miArray[i].toObject().value("mi_cid").toString() + ",");
    }

    mSql.w_user_act(userName,totalStation,2,userActStr);

    if(userStationTable.is_station_exist(mSql.db(),totalStation,param.value("station").toString()))
    {
        if(stationEmuTable.is_emu_exist(mSql.db(),param.value("station").toString(),param.value("emu").toString()))
        {
            mSql.update_dev_control(totalStation,param.value("station").toString(),
                               param.value("emu").toString(),QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),0x01,"");

            return emuMiTable.w_mi(mSql.db(),param.value("emu").toString(),param);
        }
    }

    return false;
}

bool user_service::addWiFiMi(QString userName, const QJsonObject &param)
{
    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;
    ag_emu_mi_table emuMiTable;

    QJsonArray emuArray = param.value("emus").toArray();
    QString totalStation = getTotalStation(userName);
    QString userActStr;

    userActStr.append("添加电站:");
    userActStr.append(param.value("station").toString());
    userActStr.append("下的WIFI版微逆:");

    for(int i=0;i<emuArray.size();i++)
    {
        userActStr.append(emuArray[i].toObject().value("mi").toObject().value("mi_cid").toString() + ",");
    }
    mSql.w_user_act(userName,totalStation,2,userActStr);


    //写入编号
    for(int i=0;i<emuArray.size();i++)
    {
        QString tmpEmuCID = emuArray[i].toObject().value("emu").toString();;
        if(userStationTable.is_station_exist(mSql.db(),totalStation,param.value("station").toString()))
        {
            stationEmuTable.w_one_emu(mSql.db(),param.value("station").toString(),tmpEmuCID,"");
            //写入成功
            if(stationEmuTable.is_emu_exist(mSql.db(),param.value("station").toString(),tmpEmuCID))
            {
                emuMiTable.w_one_mi(mSql.db(),tmpEmuCID,emuArray[i].toObject().value("mi").toObject().value("mi_cid").toString(),
                                       emuArray[i].toObject().value("mi").toObject().value("mi_desc").toString());
            }
        }
    }

    return true;
}

bool user_service::delMi(QString userName, const QJsonObject &param)
{
    ag_user_station_table userStationTable;
    ag_station_emu_table stationEmuTable;
    ag_emu_mi_table emuMiTable;
    QString totalStation = getTotalStation(userName);
    QString userActStr;

    userActStr.append("删除");
    userActStr.append(param.value("station").toString());
    userActStr.append(" ");
    userActStr.append(param.value("emu_cid").toString());
    userActStr.append("下的微逆:");
    userActStr.append(param.value("mi_cid").toString());

    mSql.w_user_act(userName,totalStation,1,userActStr);

    if(userStationTable.is_station_exist(mSql.db(),totalStation,param.value("station").toString()))
    {

        if(stationEmuTable.is_emu_exist(mSql.db(),param.value("station").toString(),
                                        param.value("emu_cid").toString()))
        {
            mSql.update_dev_control(totalStation,param.value("station").toString(),param.value("emu_cid").toString(),
                               QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),0x01,"");
            return emuMiTable.del_one_mi_by_micid(mSql.db(),param.value("emu_cid").toString(),
                                           param.value("mi_cid").toString());

        }
    }

    return false;
}







