/**
  @file
  @author Stefan Frings
*/


#include "ordercontroller.h"

#include "agingservice.h"


void orderController::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    void (orderController::*deal_method)() = nullptr;

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

void orderController::getWorkOrder()
{
    agingservice agingSv(mDataBase);
    QJsonObject retDataJsObj;
    agingSv.readOrderData(mJsBody,retDataJsObj);

    mResponse->write(common::js_to_qbytearray(retDataJsObj),true);
}

void orderController::setWorkOrder()
{
    agingservice agingSv(mDataBase);
    agingSv.writeOrderData(mJsBody);

    mResponse->write(common::js_to_qbytearray(QJsonObject()),true);
}
