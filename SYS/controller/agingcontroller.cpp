/**
  @file
  @author Stefan Frings
*/


#include "agingcontroller.h"

#include "QsLog.h"
#include "agingservice.h"
#include "userservice.h"
#include "devmgservice.h"
#include "abstract_emu.h"
#include "abstract_bym.h"

#define AGINGCABINET   6   //老化柜数量
#define CABINETEMUSIZE 6   //柜子下的网关数   默认按行


QString gAgingBitchTime[AGINGCABINET];

void agingController::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    void (agingController::*deal_method)() = nullptr;

    mRequest = &request;
    mResponse = &response;
    mJsBody = common::qbytearray_to_js(mRequest->getBody());
    mName = QString::fromUtf8(mRequest->getParameter("account"));
    //消息处理
    deal_method = mMethodMapper.value(QString::fromUtf8(path),nullptr);

    if(deal_method)
    {
        (this->*deal_method)();  //跳转处理方法
    }
    else
    {
        response.setStatus(401,"没当前指令的解析");
        response.write(QString("{}").toUtf8(),true);
    }
}

void agingController::setRoomAndDevData()
{
    userservice userSv(mDataBase);
    devmgservice devMgSv(mDataBase);
    agingservice agingSv(mDataBase);
    QJsonObject retDataJsObj;
    QString CurrentTime = mJsBody.value("current_time").toString();
    QJsonArray cabinetsArray = mJsBody.value("cabinets").toArray();

    QString totalStation = userSv.readUserTotalStation(mName);

    QString curtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    retDataJsObj.insert("err","receive ok");
    if(cabinetsArray.size() > AGINGCABINET)
    {
        retDataJsObj.insert("err","cabinet num err limit max=" + QString(AGINGCABINET));
        QLOG_ERROR() << "老化柜数量错误 max=" + QString(AGINGCABINET);
        mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
        return;
    }

    for(int i=0;i<cabinetsArray.size();i++)
    {
        QJsonObject cabinetJsObj = cabinetsArray[i].toObject();
        QJsonArray nodesArray = cabinetJsObj.value("nodes").toArray();
        QString cabinetStartTime = cabinetJsObj.value("start_time").toString("00");
        int cabinetAgingTime = cabinetJsObj.value("age_time").toInt();
        QString room = cabinetJsObj.value("room_name").toString();
        QStringList emus;
        //写rack数据的临时变量
        QJsonObject rkDataJsObj;
        QJsonArray rkDataArray;

        //老化起始时间对不上  说明是新的数据 就替换设备对象
        if(gAgingBitchTime[i] != cabinetStartTime && cabinetStartTime != "")
        {
            devMgSv.delStation(totalStation,room);
            devMgSv.addStation(totalStation,room);
            if(cabinetJsObj.value("isopen").toBool(false))
            {
                //加入每个老化柜固定网关
                for(int j=0;j<CABINETEMUSIZE;j++)
                {
                    QString plcEmuCid = "909999" + QString::number(i)+QString::number(j);
                    devMgSv.addOneEmu(room,plcEmuCid,"L:" + QString::number(j));
                    devMgSv.delMiFromEmu(totalStation,room,plcEmuCid);
                    emus.append(plcEmuCid);
                }
            }
            else
            {
                continue;
            }
        }


        //处理节点数据
        for(int j=0;j<nodesArray.size();j++)
        {
            QJsonObject nodeJsObj = nodesArray[j].toObject();
            QString emuCid;
            QString miCid;
            bool iswifi = false;

            QJsonObject miJsObj;

            miCid = nodeJsObj.value("mi_cid").toString();
            //如果是wifi版微逆 //编号ABC 开头
            if(abstract_emu::is_wifiemu_cid_valid(miCid))
            {
                emuCid = nodeJsObj.value("mi_cid").toString();
                miCid = abstract_emu::emu_cid_to_wifi_micid(nodeJsObj.value("mi_cid").toString());
                iswifi = true;
            }
            else if(abstract_bym::is_cid_valid(miCid))
            {
                emuCid = "909999" + QString::number(i)+nodeJsObj.value("col").toString();
                miCid = nodeJsObj.value("mi_cid").toString();
            }
            else {
                continue;
            }

            if(!abstract_emu::is_general_cid_valid(emuCid) && !abstract_emu::is_wifiemu_cid_valid(emuCid))
            {
                QLOG_ERROR() << "agingroom emuID err ID:" + emuCid;
                continue;
            }
            if(!abstract_bym::is_cid_valid(miCid))
            {
                QLOG_ERROR() << "agingroom miID err ID:" + miCid;
                continue;
            }

            //加入关系编号
            if(gAgingBitchTime[i] != cabinetStartTime)
            {
                if(iswifi)
                {
                    devMgSv.addOneWiFiMi(totalStation,room,emuCid,miCid,
                                         nodeJsObj.value("row").toString()+ "-" + nodeJsObj.value("col").toString());
                }
                else
                {
                    devMgSv.addOnePlcMi(totalStation,room,emuCid,miCid,
                                        nodeJsObj.value("row").toString()+ "-" + nodeJsObj.value("col").toString());
                }
            }
            //rack data 数据格式的组包
            miJsObj.insert("room_id",room);
            miJsObj.insert("rack_id","rack 0");
            miJsObj.insert("leakage_status","OFF");
            miJsObj.insert("mi_pos",nodeJsObj.value("row").toString()+ "-" + nodeJsObj.value("col").toString());
            miJsObj.insert("mi_cid",miCid);
            miJsObj.insert("dc_volt_in",nodeJsObj.value("dc_V").toDouble());
            miJsObj.insert("dc_current_in",nodeJsObj.value("dc_A").toDouble());
            miJsObj.insert("dc_power_in",nodeJsObj.value("dc_W").toDouble());
            miJsObj.insert("dc_volt_eff_k",1);
            miJsObj.insert("dc_volt_eff_b",0);
            miJsObj.insert("ac_volt_out",nodeJsObj.value("ac_V").toDouble());
            miJsObj.insert("ac_current_out",nodeJsObj.value("ac_A").toDouble());
            miJsObj.insert("ac_power_out",nodeJsObj.value("ac_W").toDouble());
            miJsObj.insert("ac_volt_eff_k",1);
            miJsObj.insert("ac_volt_eff_b",0);
            //miJsObj.insert("start_time",nodeJsObj.value("aging_start_time").toString());
            miJsObj.insert("start_time",cabinetStartTime);
            miJsObj.insert("age_time",cabinetAgingTime);
            miJsObj.insert("cur_date",CurrentTime);

            rkDataArray.append(miJsObj);
        }
        rkDataJsObj.insert("datas",rkDataArray);

        agingSv.writeRackData(rkDataJsObj);

        //通知网关编号变化 请求同步
        if(gAgingBitchTime[i] != cabinetStartTime)
        {
            for(int j=0;j<emus.size();j++)
            {
                devMgSv.addDevCtl(totalStation,room,emus[j],curtime,0x01,"");
            }
        }

        //写老化柜温度数据
        agingSv.writeNewRoomTemperature(cabinetJsObj.value("room_name").toString(),true,
                                        cabinetJsObj.value("averagetemp").toInt(),
                                        cabinetJsObj.value("settemp").toInt(),
                                        CurrentTime);
        //记录批次时间
        gAgingBitchTime[i] = cabinetStartTime;
    }

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void agingController::getMiAgingReport()
{
    agingservice agingSv(mDataBase);
    QString startTime = mJsBody.value("select_param").toObject().value("start_time").toString();
    int mode = mJsBody.value("select_param").toObject().value("mode").toInt(-1);
    QJsonArray misArray = mJsBody.value("select_param").toObject().value("mis").toArray();
    QJsonArray misReport;
    QJsonObject algorithm;
    QJsonObject miReport;
    QJsonObject retDataJsObj;
    agingSv.readAgingAlgorithm(mName,algorithm);

    for(int i=0;i<misArray.size();i++)
    {
        agingSv.generateMiAgingReport(mode,misArray[i].toString(""),startTime,algorithm,miReport);

        misReport.append(miReport);
    }

    retDataJsObj.insert("report_datas",misReport);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void agingController::getMiAgingBatch()
{
    agingservice agingSv(mDataBase);
    QJsonObject retDataJsObj;
    agingSv.readBatchList(mJsBody,retDataJsObj);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void agingController::getRoomTemp()
{
    agingservice agingSv(mDataBase);
    QJsonObject retDataJsObj;
    agingSv.readRoomTemperature(mJsBody,retDataJsObj);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void agingController::setRoomTemp()
{
    agingservice agingSv(mDataBase);
    agingSv.writeRoomTemperature(mJsBody);

    mResponse->write(common::js_to_qbytearray(QJsonObject()),true);
}

void agingController::getRackData()
{
    agingservice agingSv(mDataBase);
    QJsonObject retDataJsObj;
    agingSv.readRackData(mJsBody,retDataJsObj);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}
