/**
  @file
  @author Stefan Frings
*/

#ifndef DEVCTLCONTROLLER_H
#define DEVCTLCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

#include <QHash>
#include "common.h"
#include <QSqlDatabase>
#include "sqlconnectpool.h"
#include "mysql.h"

#include "devdataservice.h"
using namespace stefanfrings;

/**
 * http
  设备远程控制
*/

class devCtlController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(devCtlController)
public:

    /** Constructor */
    devCtlController(QSqlDatabase &DataBase){
        mDataBase = DataBase;
    };
    ~devCtlController(){
    };
    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
private:
    QSqlDatabase mDataBase; //数据库

    HttpRequest *mRequest = nullptr;
    HttpResponse *mResponse = nullptr;
    QString mName;
    QJsonObject mJsBody;
    QHash<QString, void (devCtlController::*)()> mMethodMapper=
    {
        {"/w_emu_func",&devCtlController::setEmuProperty},

        {"/w_temporary_power",&devCtlController::setMiTemporaryPower},
        {"/r_temporary_power",&devCtlController::getMiTemporaryPower},
        {"/w_more_temporary_power",&devCtlController::setMiMoreTemporaryPower},
        {"/r_more_temporary_power",&devCtlController::getMiMoreTemporaryPower},

        {"/w_max_power",&devCtlController::setMiMaxPower},
        {"/r_max_power",&devCtlController::getMiMaxPower},
        {"/w_more_max_power",&devCtlController::setMiMoreMaxPower},
        {"/r_more_max_power",&devCtlController::getMiMoreMaxPower},

        {"/w_grid",&devCtlController::setMiGridParam},
        {"/r_grid",&devCtlController::getMiGridParam},
//        {"/w_more_grid",&devCtlController::setMiMoreGridParam},
//        {"/r_more_grid",&devCtlController::getMiMoreGridParam},

        {"/w_certification",&devCtlController::setMiCertification},
        {"/r_certification",&devCtlController::getMiCertification},
//        {"/w_more_certification",&devCtlController::setMiMoreCertification},
//        {"/r_more_certification",&devCtlController::getMiMoreCertification},

        {"/r_select_back",&devCtlController::getControlResultBack}
    };
    //设置微逆属性
    void setEmuProperty();
    //微逆控制  通过类的函数指针传递处理方法  isGET: 控制类型(GET or SET)
    void dealMiCtlData(QString dataField,void (devdataservice::*f)(QString,QString&),QJsonObject &retDataJsObj,bool isGET = true);
    //more微逆控制
    void dealMoreMiCtlData(QString dataField,void (devdataservice::*f)(QString,QString&),QJsonObject &retDataJsObj,bool isGET = true);
    //临时功率
    void setMiTemporaryPower();
    void getMiTemporaryPower();
    void setMiMoreTemporaryPower();
    void getMiMoreTemporaryPower();

    //最大功率
    void setMiMaxPower();
    void getMiMaxPower();
    void setMiMoreMaxPower();
    void getMiMoreMaxPower();
    //并网参数
    void setMiGridParam();
    void getMiGridParam();
    void setMiMoreGridParam();
    void getMiMoreGridParam();
    //安规参数
    void setMiCertification();
    void getMiCertification();
    void setMiMoreCertification();
    void getMiMoreCertification();

    void getControlResultBack();  //远程控制回读

};

#endif // LOGINCONTROLLER_H
