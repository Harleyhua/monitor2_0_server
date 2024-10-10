#include "devdataservice.h"

#include "mysql.h"
#include "QsLog.h"
devdataservice::devdataservice(QSqlDatabase &DataBase, QObject *parent)
    : QObject{parent}
{
    mDataBase = DataBase;
}

void devdataservice::readEmuAction(const QJsonObject &s, QJsonObject &retData)
{
    ag_gateway_data_table EmuDataTable;

    EmuDataTable.read_data(mDataBase,s,retData);
}

void devdataservice::readEmuStatus(const QJsonObject &s, QJsonObject &retData)
{
    ag_emu_status_table emuStatusTable;

    emuStatusTable.r_data(mDataBase,s,retData);
}

//读取微逆数据
void devdataservice::readMiCurrentPowerData(QStringList Mis, QJsonObject &retData)
{
    ag_power_data_table powerDataTable;

    powerDataTable.read_current_data(mDataBase,Mis,retData);

}

void devdataservice::readMiPowerData(const QJsonObject &s, QJsonObject &retData, bool is_one)
{
    ag_power_data_table powerDataTable;

    powerDataTable.read_data(mDataBase,s,retData,is_one);

}

void devdataservice::readMiProperty(QString Mi, QJsonObject &retData)
{
    ag_mi_property_table miPropertyTable;
    QJsonObject miPropertyJsObj;

    miPropertyTable.read_property(mDataBase,Mi,miPropertyJsObj);
    retData.insert(ag_mi_property_table::c_field_nominal_power,
                      miPropertyJsObj.value("datas").toObject().value(ag_mi_property_table::c_field_nominal_power).toInt());
    retData.insert(ag_mi_property_table::c_field_mim_version,
                      miPropertyJsObj.value("datas").toObject().value(ag_mi_property_table::c_field_mim_version).toString());
    retData.insert(ag_mi_property_table::c_field_mis_version,
                      miPropertyJsObj.value("datas").toObject().value(ag_mi_property_table::c_field_mis_version).toString());
    retData.insert(ag_emu_mi_table::c_field_mi,Mi);
    //tmp_mi_obj.insert(ag_emu_mi_table::c_field_mi_desc,mi_desc[k]);
}

QString devdataservice::readEmuLastActionTime(QString emu)
{
    ag_gateway_data_table emuDataTable;
    ag_emu_handshake_table emu_hs_Table;
    QString lastTime;
    //读取最后异常action的通讯时间
    lastTime = mysql::m_emucid_hand_lastTime.value(emu,"");
    //内存不存在时  读库
    if(lastTime == "")
    {
        //emuDataTable.read_last_hand_data_time(mDataBase,emu,lastTime);
        emu_hs_Table.read_last_hand_data_time(mDataBase,emu,lastTime);
        if(emu_act_cache_lock.tryLock(500))
        {
            mysql::m_emucid_hand_lastTime.insert(emu,lastTime);
            emu_act_cache_lock.unlock();
        }
        //QLOG_INFO() << "no cache read " + emu + "time:" + lastTime;
    }

    return lastTime;

}

void devdataservice::readEmuLastStatus(QString Emu, QJsonObject &retData)
{
    ag_emu_status_table emuStatusTable;
    QString status,statusReserve,runMode,emuSysTime;

    emuStatusTable.r_last_data(mDataBase,Emu,status,statusReserve,runMode,emuSysTime);
    retData.insert(ag_emu_status_table::c_field_status,status);
    retData.insert(ag_emu_status_table::c_field_run_mode,runMode);
    retData.insert("status_time",emuSysTime);
}

void devdataservice::writeEmuProperty(QString emu, QString countercurrent)
{
    ag_emu_extra_property_table emuExtraPropertyTable;

    emuExtraPropertyTable.w_countercurrent(mDataBase,emu,countercurrent);


}

//void devdataservice::writeEmuProperty(const QJsonObject &s, QJsonObject &retData)
//{


//}

void devdataservice::readEmuProperty(QString Emu, QJsonObject &retData)
{
    ag_emu_property_table emuPropertyTable;
    QJsonObject emuPropertyJsObj;

    emuPropertyTable.read_property(mDataBase,Emu,emuPropertyJsObj);
    retData.insert(ag_emu_property_table::c_field_type,
                      emuPropertyJsObj.value("datas").toObject().value(ag_emu_property_table::c_field_type).toString());
    retData.insert(ag_emu_property_table::c_field_hard_version,
                      emuPropertyJsObj.value("datas").toObject().value(ag_emu_property_table::c_field_hard_version).toString());
    retData.insert(ag_emu_property_table::c_field_soft_version,
                      emuPropertyJsObj.value("datas").toObject().value(ag_emu_property_table::c_field_soft_version).toString());
    retData.insert(ag_station_emu_table::c_field_emu,Emu);
    retData.insert(ag_station_emu_table::c_field_emu_desc,Emu);

}

void devdataservice::writeMiTemporaryPower(QString Mi, QString &temporaryPower)
{
    ag_mi_extra_property_table miExtraPropertyTable;

    miExtraPropertyTable.w_temporary_power(mDataBase,Mi,temporaryPower);
}

void devdataservice::readMiTemporaryPower(QString Mi, QString &temporaryPower)
{
    ag_mi_extra_property_table miExtraPropertyTable;

    miExtraPropertyTable.r_temporary_power(mDataBase,Mi,temporaryPower);
}

void devdataservice::writeMiMaxPower(QString Mi, QString &maxPower)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    miExtraPropertyTable.w_max_power(mDataBase,Mi,maxPower);
}

void devdataservice::readMiMaxPower(QString Mi, QString &maxPower)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    miExtraPropertyTable.r_max_power(mDataBase,Mi,maxPower);
}

void devdataservice::writeMiGrid(QString Mi, QString &grid)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    miExtraPropertyTable.w_grid(mDataBase,Mi,grid);
}

void devdataservice::readMiGrid(QString Mi, QString &grid)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    miExtraPropertyTable.r_grid(mDataBase,Mi,grid);
}

void devdataservice::writeMiCertification(QString Mi, QString &certification)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    miExtraPropertyTable.w_certification(mDataBase,Mi,certification);
}

void devdataservice::readMiCertification(QString Mi, QString &certification)
{
    ag_mi_extra_property_table miExtraPropertyTable;
    miExtraPropertyTable.r_certification(mDataBase,Mi,certification);
}
