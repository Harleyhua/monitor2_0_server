/**
  @file
  @author Stefan Frings
*/

#ifndef LOGCONTROLLER_H
#define LOGCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include <QHash>
#include <QSqlDatabase>
#include "common.h"
#include "mysql.h"
#include "sqlconnectpool.h"
using namespace stefanfrings;

/**
  This controller demonstrates how to use HTTP basic login.
*/

class logController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(logController)
public:

    /** Constructor */
    logController(QSqlDatabase &DataBase){
        mDataBase = DataBase;
    };
    ~logController(){

    };
    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
private:
    QSqlDatabase mDataBase; //数据库
    HttpRequest *mRequest = nullptr;
    HttpResponse *mResponse = nullptr;
};

#endif // LOGINCONTROLLER_H
