/**
  @file
  @author Stefan Frings
*/


#include "devdatacontroller.h"

#include "common.h"

#include "userservice.h"
#include "devdataservice.h"
#include "devmgservice.h"
void devDataController::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    void (devDataController::*deal_method)() = nullptr;

    mRequest = &request;
    mResponse = &response;

    mJsBody = common::qbytearray_to_js(mRequest->getBody());
    mName = QString::fromUtf8(mRequest->getParameter("account"));
    //消息处理
    deal_method = mMethodMapper.value(QString::fromUtf8(path),nullptr);

    if(deal_method != nullptr)
    {
        (this->*deal_method)();  //跳转处理方法
    }
    else
    {
        response.setStatus(401,"没当前指令的解析");
        response.write(QString("{}").toUtf8(),true);
    }
}

void devDataController::getUserDeviceMapperAndLastData()
{
    devmgservice devMgSv(mDataBase);
    devdataservice devDataSv(mDataBase);
    userservice userSv(mDataBase);
    QString totalStation;
    QStringList Mis;
    QJsonObject retData;
    totalStation = userSv.readUserTotalStation(mName);
    devMgSv.DevMapping(totalStation,Mis,retData);
    devDataSv.readMiCurrentPowerData(Mis,retData);

    mResponse->write(common::js_to_qbytearray(retData),true);
}

void devDataController::getEmuAction()
{
    devdataservice devDataSv(mDataBase);
    QJsonObject retData;
    devDataSv.readEmuAction(mJsBody,retData);

    mResponse->write(common::js_to_qbytearray(retData),true);
}

void devDataController::getEmuStatus()
{
    devdataservice devDataSv(mDataBase);
    QJsonObject retData;
    devDataSv.readEmuStatus(mJsBody,retData);

    mResponse->write(common::js_to_qbytearray(retData),true);
}

//void devDataController::getOneEmuStatus()
//{

//}

void devDataController::getMIPower()
{
    devdataservice devDataSv(mDataBase);
    QJsonObject retData;
    devDataSv.readMiPowerData(mJsBody,retData);

    mResponse->write(common::js_to_qbytearray(retData),true);
}

void devDataController::getOneMiPower()
{
    devdataservice devDataSv(mDataBase);
    QJsonObject retData;
    devDataSv.readMiPowerData(mJsBody,retData,true);

    mResponse->write(common::js_to_qbytearray(retData),true);
}
