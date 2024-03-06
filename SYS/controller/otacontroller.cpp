/**
  @file
  @author Stefan Frings
*/


#include "otacontroller.h"



void otaController::service(HttpRequest& request, HttpResponse& response)
{
    mRequest = &request;
    mResponse = &response;
}
