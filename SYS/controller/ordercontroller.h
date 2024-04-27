/**
  @file
  @author Stefan Frings
*/

#ifndef ORDERCONTROLLER_H
#define ORDERCONTROLLER_H

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
  工单
*/

class orderController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(orderController)
public:

    /** Constructor */
    orderController(QSqlDatabase &DataBase){
        mDataBase = DataBase;
    };
    ~orderController(){

    };
    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
private:
    QSqlDatabase mDataBase; //数据库
    HttpRequest *mRequest = nullptr;
    HttpResponse *mResponse = nullptr;
    QString mName;
    QJsonObject mJsBody;
    QHash<QString, void (orderController::*)()> mMethodMapper=
    {
        {"/r_workorder",&orderController::getWorkOrder},
        {"/w_workorder",&orderController::setWorkOrder}
    };

    void getWorkOrder();
    void setWorkOrder();
};

#endif // LOGINCONTROLLER_H
