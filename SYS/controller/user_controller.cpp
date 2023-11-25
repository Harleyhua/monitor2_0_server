#include "user_controller.h"





void user_controller::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray path=request.getPath();

    void (user_controller::*deal_method)() = nullptr;

    setRequestAndResponse(request,response);

    if(mRequest == nullptr || mRequest == nullptr)
        return;
    //身份验证
    if(0)
    {
        response.setStatus(500,"账户不存在或密码错误");
        response.write(QString("{}").toUtf8(),true);
        return;
    }

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

void user_controller::getUserProperty()
{

}

void user_controller::getUserOperationRecord()
{

}

void user_controller::setUserAlgorithm()
{

}

void user_controller::setEmuProperty()
{

}

void user_controller::setMiTemporaryPower()
{

}

void user_controller::getMiTemporaryPower()
{

}

void user_controller::setMiMaxPower()
{

}

void user_controller::getMiMaxPower()
{

}

void user_controller::setMiGridParam()
{

}

void user_controller::getMiGridParam()
{

}

void user_controller::setMiCertification()
{

}

void user_controller::getMiCertification()
{

}

void user_controller::getControlResultBack()
{

}

void user_controller::getEmuAction()
{

}

void user_controller::getEmuStatus()
{

}

void user_controller::getRackData()
{

}

void user_controller::getMIPower()
{

}

void user_controller::getRoomTemp()
{

}

void user_controller::getWorkOrder()
{

}

void user_controller::setWorkOrder()
{

}

void user_controller::addStation()
{

}

void user_controller::delStation()
{

}

void user_controller::addEmu()
{

}

void user_controller::delEmu()
{

}

void user_controller::addPlcMi()
{

}

void user_controller::addWiFiMi()
{

}

void user_controller::delMi()
{

}

void user_controller::getUserDeviceMapperAndLastData()
{

}

void user_controller::getMiAgingReport()
{

}

void user_controller::getMiAgingBatch()
{

}
