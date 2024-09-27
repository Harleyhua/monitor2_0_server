/**
  @file
  @author Stefan Frings
*/

#ifndef AGINGCONTROLLER_H
#define AGINGCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

#include <QHash>
#include "common.h"
#include "sqlconnectpool.h"
#include "mysql.h"
#include <QSqlDatabase>
#include <QString>

using namespace stefanfrings;

/**
  老化相关业务逻辑控制器
*/

class agingController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(agingController)
public:

    /** Constructor */
    agingController(QSqlDatabase &DataBase){
        mDataBase = DataBase;
    };
    ~agingController(){
    };
    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
    bool writeRoomTemp(QString strJsonData = "");

private:
    QSqlDatabase mDataBase; //数据库
    HttpRequest *mRequest = nullptr;
    HttpResponse *mResponse = nullptr;
    QString mName;
    QJsonObject mJsBody;
    QHash<QString, void (agingController::*)()> mMethodMapper=
    {
        {"/aging_report",&agingController::getMiAgingReport},
        {"/history_report",&agingController::getMiHistoryReport},
        {"/batch_list",&agingController::getMiAgingBatch},
        {"/r_ag_room_temp",&agingController::getRoomTemp},
        //{"/write_room_temp",&agingController::setRoomTemp},
        {"/r_rack_data",&agingController::getRackData},
        {"/aging/newroomdevdata",&agingController::setRoomAndDevData}
    };
    //处理老化房运行数据
    void setRoomAndDevData();
    //获取微逆老化报告
    void getMiAgingReport();
    //获取微逆历史老化报告
    void getMiHistoryReport();
    //获取微逆老化批次
    void getMiAgingBatch();
    //获取老化房温度
    void getRoomTemp();
    //设置老化房温度  该数据由  温度采集程序上报的
    void setRoomTemp();
    //获取老化架的相关数据
    void getRackData();
};

#endif // LOGINCONTROLLER_H
