/**
  @file
  @author Stefan Frings
*/

#ifndef POWERSTATIONCONTROLLER_H
#define POWERSTATIONCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include <QHash>
#include "common.h"
#include "mysql.h"
#include "sqlconnectpool.h"
#include <QSqlDatabase>
using namespace stefanfrings;

/**
  设备列表
*/

class powerStationController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(powerStationController)
public:

    /** Constructor */
    powerStationController(QSqlDatabase &DataBase){
        mDataBase = DataBase;
    };
    ~powerStationController(){

    };
    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
private:
    QSqlDatabase mDataBase; //数据库

    HttpRequest *mRequest = nullptr;
    HttpResponse *mResponse = nullptr;

    QString mName;
    QJsonObject mJsBody;

    QHash<QString, void (powerStationController::*)()> mMethodMapper=
    {
        {"/add_station",&powerStationController::addStation},
        {"/del_station",&powerStationController::delStation},

        {"/add_emu_cid",&powerStationController::addEmu},
        {"/del_emu",&powerStationController::delEmu},

        {"/add_plcmi_cid",&powerStationController::addPlcMi},
        {"/add_wifimi_cid",&powerStationController::addWiFiMi},
        {"/del_mi",&powerStationController::delMi}
    };
    //设备节点关系操作
    //添加电站
    void addStation();
    //删除电站
    void delStation();
    //添加网关
    void addEmu();
    //删除网关
    void delEmu();
    //添加PLC版微逆
    void addPlcMi();
    //添加WiFi版微逆
    void addWiFiMi();
    //删除微逆
    void delMi();
};

#endif // LOGINCONTROLLER_H
