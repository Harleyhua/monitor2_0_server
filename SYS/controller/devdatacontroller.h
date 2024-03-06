/**
  @file
  @author Stefan Frings
*/

#ifndef DEVDATACONTROLLER_H
#define DEVDATACONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include <QHash>
#include <QByteArray>
#include <QJsonObject>
#include <QSqlDatabase>
#include "common.h"
#include "sqlconnectpool.h"
#include "mysql.h"
using namespace stefanfrings;

/**
  This controller demonstrates how to use HTTP basic login.
*/

class devDataController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(devDataController)
public:

    /** Constructor */
    devDataController(QSqlDatabase &DataBase){
        mDataBase = DataBase;
    };
    ~devDataController(){

    };
    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
private:
    QSqlDatabase mDataBase; //数据库
    HttpRequest *mRequest = nullptr;
    HttpResponse *mResponse = nullptr;

    QString mName;
    QJsonObject mJsBody;

    QHash<QString, void (devDataController::*)()> mMethodMapper=
    {
        {"/r_mi_current_power",&devDataController::getUserDeviceMapperAndLastData},
        {"/r_emu_action",&devDataController::getEmuAction},
        {"/r_emu_status",&devDataController::getEmuStatus},
        {"/r_one_emu_status",&devDataController::getEmuStatus},
        {"/r_mi_power",&devDataController::getMIPower},
        {"/r_one_plot_mi_power",&devDataController::getOneMiPower},
        {"/r_one_mi_power",&devDataController::getOneMiPower}
    };
    //读取用户下的账号关系和最新数据
    void getUserDeviceMapperAndLastData();
    //数据库查询穿透
    //获取微逆指令
    void getEmuAction();
    //获取网关状态
    void getEmuStatus();
    //void getOneEmuStatus();
    //获取微逆功率
    void getMIPower();
    void getOneMiPower();
};

#endif // LOGINCONTROLLER_H
