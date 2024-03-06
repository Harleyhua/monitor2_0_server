/**
  @file
  @author Stefan Frings
*/


#include "logcontroller.h"



void logController::service(HttpRequest& request, HttpResponse& response)
{
    mRequest = &request;
    mResponse = &response;
}
