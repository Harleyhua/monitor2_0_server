/**
  @file
  @author Stefan Frings
*/

#ifndef OTACONTROLLER_H
#define OTACONTROLLER_H

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
  This controller demonstrates how to use HTTP basic login.
*/

class otaController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(otaController)
public:

    /** Constructor */
    otaController(QSqlDatabase &DataBase){
        mDataBase = DataBase;
    };
    ~otaController(){
    };
    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
private:
    QSqlDatabase mDataBase; //数据库
    HttpRequest *mRequest = nullptr;
    HttpResponse *mResponse = nullptr;
};

#endif // LOGINCONTROLLER_H
