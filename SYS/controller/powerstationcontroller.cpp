/**
  @file
  @author Stefan Frings
*/


#include "powerstationcontroller.h"

#include "userservice.h"
#include "logservice.h"
#include "devmgservice.h"
void powerStationController::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    void (powerStationController::*deal_method)() = nullptr;
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

void powerStationController::addStation()
{
    userservice userSv(mDataBase);
    logservice logSv(mDataBase);
    devmgservice devmgSv(mDataBase);
    QString totalStation;
    QJsonArray stationArray;
    QString logStr;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    totalStation = userSv.readUserTotalStation(mName);

    stationArray = mJsBody.value("station").toArray();
    for(int i=0;i<stationArray.size();i++)
    {
        logStr.append(stationArray[i].toString() + ",");
    }
    logStr = "添加电站列表 " + logStr;
    //记录当前操作到日志
    logSv.writeUserAction(mName,totalStation,2,logStr,time);
    //加入电站
    devmgSv.addStation(totalStation,mJsBody);

    mResponse->write(common::js_to_qbytearray(QJsonObject()),true);

}

void powerStationController::delStation()
{
    logservice logSv(mDataBase);
    userservice userSv(mDataBase);
    devmgservice devmgSv(mDataBase);
    QString totalStation;

    QString logStr;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    totalStation = userSv.readUserTotalStation(mName);
    logStr = "删除电站" + mJsBody.value("station").toString();
    //
    logSv.writeUserAction(mName,totalStation,1,logStr,time);

    devmgSv.delStation(totalStation,mJsBody);

    mResponse->write(common::js_to_qbytearray(QJsonObject()),true);
}

void powerStationController::addEmu()
{
    userservice userSv(mDataBase);
    logservice logSv(mDataBase);
    devmgservice devmgSv(mDataBase);
    QString totalStation;
    QJsonArray emuArray;
    QString logStr;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    totalStation = userSv.readUserTotalStation(mName);

    emuArray = mJsBody.value("emus").toArray();
    for(int i=0;i<emuArray.size();i++)
    {
        logStr.append(emuArray[i].toObject().value("emu_cid").toString() + ",");
    }
    logStr.insert(0,"添加电站 " + mJsBody.value("station").toString() + "下的网关 ");
    //记录当前操作到日志
    logSv.writeUserAction(mName,totalStation,2,logStr,time);
    //加入网关
    devmgSv.addEmu(totalStation,mJsBody);

    mResponse->write(common::js_to_qbytearray(QJsonObject()),true);
}

void powerStationController::delEmu()
{
    userservice userSv(mDataBase);
    logservice logSv(mDataBase);
    devmgservice devmgSv(mDataBase);
    QString totalStation;
    QString logStr;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    totalStation = userSv.readUserTotalStation(mName);
    logStr = "删除" + mJsBody.value("station").toString() +
            "下的网关:" + mJsBody.value("emu_cid").toString();

    logSv.writeUserAction(mName,totalStation,1,logStr,time);

    devmgSv.delEmu(totalStation,mJsBody);

    mResponse->write(common::js_to_qbytearray(QJsonObject()),true);
}

void powerStationController::addPlcMi()
{
    userservice userSv(mDataBase);
    logservice logSv(mDataBase);
    devmgservice devmgSv(mDataBase);
    QString totalStation;
    QJsonArray misArray;
    QString logStr;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    totalStation = userSv.readUserTotalStation(mName);
    misArray = mJsBody.value("mis").toArray();
    for(int i=0;i<misArray.size();i++)
    {
        logStr.append(misArray[i].toObject().value("mi_cid").toString() + ",");
    }
    logStr.insert(0, "添加电站:" + mJsBody.value("station").toString() +
                  "网关:" + mJsBody.value("emu").toString() + "下的PLC版微逆:" );
    //记录当前操作到日志
    logSv.writeUserAction(mName,totalStation,2,logStr,time);
    //加入PLC版微逆
    devmgSv.addPlcMi(totalStation,mJsBody);

    mResponse->write(common::js_to_qbytearray(QJsonObject()),true);
}

void powerStationController::addWiFiMi()
{
    userservice userSv(mDataBase);
    logservice logSv(mDataBase);
    devmgservice devmgSv(mDataBase);
    QString totalStation;
    QJsonArray emuArray;
    QString logStr;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");


    totalStation = userSv.readUserTotalStation(mName);
    emuArray = mJsBody.value("emus").toArray();
    for(int i=0;i<emuArray.size();i++)
    {
        logStr.append(emuArray[i].toObject().value("mi").toObject().value("mi_cid").toString() + ",");
    }
    logStr.insert(0,"添加电站:" + mJsBody.value("station").toString() + "下的WIFI版微逆:");

    //记录当前操作到日志
    logSv.writeUserAction(mName,totalStation,2,logStr,time);
    //加入WiFi版微逆
    devmgSv.addWiFiMi(totalStation,mJsBody);

    mResponse->write(common::js_to_qbytearray(QJsonObject()),true);
}

void powerStationController::delMi()
{
    userservice userSv(mDataBase);
    logservice logSv(mDataBase);
    devmgservice devmgSv(mDataBase);
    QString totalStation;
    QString logStr;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    totalStation = userSv.readUserTotalStation(mName);

    logStr = "删除" + mJsBody.value("station").toString() + " " +
                 mJsBody.value("emu_cid").toString() + "下的微逆:" +
                 mJsBody.value("mi_cid").toString();
    logSv.writeUserAction(mName,totalStation,1,logStr,time);

    devmgSv.delMi(totalStation,mJsBody);

    mResponse->write(common::js_to_qbytearray(QJsonObject()),true);
}
