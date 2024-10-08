/**
  @file
  @author Stefan Frings
*/
#include "devctlcontroller.h"
#include "userservice.h"
#include "devmgservice.h"
#include "devdataservice.h"


void devCtlController::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    void (devCtlController::*deal_method)() = nullptr;

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

void devCtlController::setEmuProperty()
{
    userservice userSv(mDataBase);
    devmgservice devMgSv(mDataBase);
    devdataservice devDataSv(mDataBase);
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString totalStation;
    totalStation = userSv.readUserTotalStation(mName);

    devDataSv.writeEmuProperty(mJsBody.value("emu_cid").toString(),
                               mJsBody.value("func").toString());

    devMgSv.addDevCtl(totalStation,
                      mJsBody.value("station").toString(),
                      mJsBody.value("emu_cid").toString(),
                      time,
                      mJsBody.value("server_cmd").toInt(),
                      mJsBody.value("data").toString());

    mResponse->write(common::js_to_qbytearray(QJsonObject()),true);
}

void devCtlController::dealMiCtlData(QString dataField, void (devdataservice::*f)(QString, QString&), QJsonObject &retDataJsObj, bool isGET)
{
    userservice userSv(mDataBase);
    devdataservice devDataSv(mDataBase);
    devmgservice devMgSv(mDataBase);
    QString totalStation;
    QString data;
    QString ctlSendData;
    //
    totalStation = userSv.readUserTotalStation(mName);

    if(isGET)
    {

        ctlSendData = mJsBody.value("data").toString();
    }
    else
    {
        data = mJsBody.value(dataField).toString();
        ctlSendData = mJsBody.value("mi_cid").toString();
    }

    (devDataSv.*f)(mJsBody.value("mi_cid").toString(),data);

    devMgSv.addDevCtl(totalStation,
                      mJsBody.value("station").toString(),
                      mJsBody.value("emu_cid").toString(),
                      mJsBody.value("ctl_time").toString(),
                      mJsBody.value("server_cmd").toInt(),
                      ctlSendData);

    retDataJsObj.insert("station",mJsBody.value("station").toString());
    retDataJsObj.insert("emu_cid",mJsBody.value("emu_cid").toString());
    retDataJsObj.insert("mi_cid",mJsBody.value("mi_cid").toString());
    if(isGET)
    {
        retDataJsObj.insert(dataField,data);
    }

}

void devCtlController::dealMoreMiCtlData(QString dataField, void (devdataservice::*f)(QString, QString &), QJsonObject &retDataJsObj, bool isGET)
{
    userservice userSv(mDataBase);
    devdataservice devDataSv(mDataBase);
    devmgservice devMgSv(mDataBase);
    QString totalStation;
    QJsonArray data;
    QJsonArray mis = mJsBody.value("mis").toArray();

    QString datactlstr;
    //
    totalStation = userSv.readUserTotalStation(mName);
    if(isGET)
    {
        for(int i=0;i<mis.size();i++)
        {
            QJsonObject mi;
            QString onedata;
            (devDataSv.*f)(mis[i].toString(),onedata);
            mi.insert("mi_cid",mis[i].toString());
            mi.insert(dataField,onedata);

            data.append(mi);
        }

        datactlstr = mJsBody.value("data").toString();
    }
    else
    {
        for(int i=0;i<mis.size();i++)
        {
            QString oneMi = mis[i].toObject().value("mi_cid").toString();
            QString onedata = mis[i].toObject().value(dataField).toString();
            (devDataSv.*f)(oneMi,onedata);
            datactlstr.append(oneMi);
        }

    }

    devMgSv.addDevCtl(totalStation,
                      mJsBody.value("station").toString(),
                      mJsBody.value("emu_cid").toString(),
                      mJsBody.value("ctl_time").toString(),
                      mJsBody.value("server_cmd").toInt(),
                      datactlstr);

    retDataJsObj.insert("station",mJsBody.value("station").toString());
    retDataJsObj.insert("emu_cid",mJsBody.value("emu_cid").toString());

    if(isGET)
    {
        retDataJsObj.insert("mis",data);
    }
    else
    {
        retDataJsObj.insert("mis",mis);
    }

}

void devCtlController::setMiTemporaryPower()
{
    QJsonObject retDataJsObj;
    dealMiCtlData("temporary_power",&devdataservice::writeMiTemporaryPower,retDataJsObj,false);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::getMiTemporaryPower()
{
    QJsonObject retDataJsObj;

    dealMiCtlData("temporary_power",&devdataservice::readMiTemporaryPower,retDataJsObj);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::setMiMoreTemporaryPower()
{
    QJsonObject retDataJsObj;
    dealMoreMiCtlData("temporary_power",&devdataservice::writeMiTemporaryPower,retDataJsObj,false);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::getMiMoreTemporaryPower()
{
    QJsonObject retDataJsObj;

    dealMoreMiCtlData("temporary_power",&devdataservice::readMiTemporaryPower,retDataJsObj);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::setMiMaxPower()
{
    QJsonObject retDataJsObj;

    dealMiCtlData("max_power",&devdataservice::writeMiMaxPower,retDataJsObj,false);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::getMiMaxPower()
{
    QJsonObject retDataJsObj;

    dealMiCtlData("max_power",&devdataservice::readMiMaxPower,retDataJsObj);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::setMiMoreMaxPower()
{
    QJsonObject retDataJsObj;

    dealMoreMiCtlData("max_power",&devdataservice::writeMiMaxPower,retDataJsObj,false);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::getMiMoreMaxPower()
{
    QJsonObject retDataJsObj;

    dealMoreMiCtlData("max_power",&devdataservice::readMiMaxPower,retDataJsObj);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::setMiGridParam()
{
    QJsonObject retDataJsObj;
    dealMiCtlData("grid",&devdataservice::writeMiGrid,retDataJsObj,false);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::getMiGridParam()
{
    QJsonObject retDataJsObj;

    dealMiCtlData("grid",&devdataservice::readMiGrid,retDataJsObj);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::setMiMoreGridParam()
{
    QJsonObject retDataJsObj;
    dealMoreMiCtlData("grid",&devdataservice::writeMiGrid,retDataJsObj,false);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::getMiMoreGridParam()
{
    QJsonObject retDataJsObj;

    dealMoreMiCtlData("grid",&devdataservice::readMiGrid,retDataJsObj);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::setMiCertification()
{
    QJsonObject retDataJsObj;
    dealMiCtlData("certification",&devdataservice::writeMiCertification,retDataJsObj,false);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::getMiCertification()
{
    QJsonObject retDataJsObj;

    dealMiCtlData("certification",&devdataservice::readMiCertification,retDataJsObj);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::setMiMoreCertification()
{
    QJsonObject retDataJsObj;
    dealMoreMiCtlData("certification",&devdataservice::writeMiCertification,retDataJsObj,false);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::getMiMoreCertification()
{
    QJsonObject retDataJsObj;

    dealMoreMiCtlData("certification",&devdataservice::readMiCertification,retDataJsObj);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void devCtlController::getControlResultBack()
{
    devmgservice devMgSv(mDataBase);
    QJsonObject retDataJsObj;

    devMgSv.getOneControlResuletBack("r_maxop",mJsBody,retDataJsObj);
    devMgSv.getOneControlResuletBack("r_cop",mJsBody,retDataJsObj);
    devMgSv.getOneControlResuletBack("r_grid",mJsBody,retDataJsObj);
    devMgSv.getOneControlResuletBack("r_cer",mJsBody,retDataJsObj);
    devMgSv.getOneControlResuletBack("w_maxop",mJsBody,retDataJsObj);
    devMgSv.getOneControlResuletBack("w_cop",mJsBody,retDataJsObj);
    devMgSv.getOneControlResuletBack("w_grid",mJsBody,retDataJsObj);
    devMgSv.getOneControlResuletBack("w_cer",mJsBody,retDataJsObj);

    retDataJsObj.insert("station",mJsBody.value("station").toString());
    retDataJsObj.insert("emu_cid",mJsBody.value("emu_cid").toString());

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}


