#ifndef CS_CONTROLLER_H
#define CS_CONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

using namespace stefanfrings;


class cs_controller : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(cs_controller)
public:
    cs_controller();

    void service(HttpRequest& request, HttpResponse& response);
private:

signals:

};

#endif // CS_CONTROLLER_H
