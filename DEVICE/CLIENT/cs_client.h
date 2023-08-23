#ifndef CS_CLIENT_H
#define CS_CLIENT_H

#include "httprequesthandler.h"

#include "mysql.h"

using namespace stefanfrings;

class cs_client :public HttpRequestHandler
{
public:
    explicit cs_client(QObject *parent = nullptr);

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

private:
    bool is_user_valid(mysql &sql,QString name,QString password);

};

#endif // CS_CLIENT_H
