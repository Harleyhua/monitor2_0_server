#include "usercontroller.h"


#include "userservice.h"
#include "logservice.h"

void usercontroller::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray path=request.getPath();
    void (usercontroller::*deal_method)() = nullptr;

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

void usercontroller::getUserProperty()
{
    QJsonObject retJsObj;
    userservice userSv(mDataBase);

    userSv.readUserInfo(mJsBody,retJsObj);

    mResponse->write(common::js_to_qbytearray(retJsObj),true);
}

void usercontroller::getUserOperationRecord()
{
    QJsonObject retJsObj;
    userservice userSv(mDataBase);
    logservice logSv(mDataBase);
    QString totalStation;


    totalStation = userSv.readUserTotalStation(mName);

    mJsBody.insert(ag_user_act_table::c_field_total_station,totalStation);

    logSv.readUserAction(mJsBody,retJsObj);

    mResponse->write(common::js_to_qbytearray(retJsObj),true);
}

void usercontroller::setUserAlgorithm()
{
    QJsonObject retJsObj;
    userservice userSv(mDataBase);

    bool ret = false;

    ret = userSv.writeUserAlgorithm(mName,mJsBody);

    if(ret)
    {
        retJsObj.insert("ret",true);
    }
    else
    {
        retJsObj.insert("ret",false);
    }

    mResponse->write(common::js_to_qbytearray(retJsObj),true);
}

