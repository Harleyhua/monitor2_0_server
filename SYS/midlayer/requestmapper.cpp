#include <QCoreApplication>
#include "requestmapper.h"

#include "agingcontroller.h"
#include "devctlcontroller.h"
#include "devdatacontroller.h"
#include "logcontroller.h"
#include "ordercontroller.h"
#include "otacontroller.h"
#include "powerstationcontroller.h"
#include "usercontroller.h"
#include "userservice.h"

#include "sqlconnectpool.h"
static QHash<QString,int> gPathMapping = {
    {"/aging_report",1},
    {"/batch_list",1},
    {"/r_ag_room_temp",1},
    {"/write_room_temp",1},
    {"/r_rack_data",1},
    {"/aging/newroomdevdata",1},

    {"/w_emu_func",2},
    {"/w_temporary_power",2},
    {"/r_temporary_power",2},
    {"/w_more_temporary_power",2},
    {"/r_more_temporary_power",2},
    {"/w_max_power",2},
    {"/r_max_power",2},
    {"/w_more_max_power",2},
    {"/r_more_max_power",2},
    {"/w_grid",2},
    {"/r_grid",2},
    {"/w_more_grid",2},
    {"/r_more_grid",2},
    {"/w_certification",2},
    {"/r_certification",2},
    {"/w_more_certification",2},
    {"/r_more_certification",2},
    {"/r_select_back",2},

    {"/r_mi_current_power",3},
    {"/r_emu_action",3},
    {"/r_emu_status",3},
    {"/r_one_emu_status",3},
    {"/r_mi_power",3},
    {"/r_one_plot_mi_power",3},
    {"/r_one_mi_power",3},


    {"/r_workorder",4},
    {"/w_workorder",4},

    {"/add_station",5},
    {"/del_station",5},
    {"/add_emu_cid",5},
    {"/del_emu",5},
    {"/add_plcmi_cid",5},
    {"/add_wifimi_cid",5},
    {"/del_mi",5},

    {"/r_user_property",6},
    {"/r_user_act",6},
    {"/update_alg",6}

};

RequestMapper::RequestMapper(QObject* parent)
    :HttpRequestHandler(parent)
{

}


RequestMapper::~RequestMapper()
{

}


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QString db_name = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + common::rand_str();
    QByteArray path=request.getPath();
    int pathCode = gPathMapping.value(path,0);
    QString name = QString::fromUtf8(request.getParameter("account"));
    QString password = QString::fromUtf8(request.getParameter("password"));
    QSqlDatabase db = sqlconnectpool::openConnection_clh(db_name);


    //身份验证
    //临时调试，注释下面代码块
    //userservice userSv(db);
    // if(!userSv.checkAccountValid(name,password))
    // {
    //     response.setStatus(500,"账户不存在或密码错误");
    //     response.write(QString("{}").toUtf8(),true);
    //     //sqlconnectpool::closeConnection(db);
    //     sqlconnectpool::closeConnection_clh(db,db_name);
    //     return;
    // }


    if(pathCode == 1)
    {
        agingController(db).service(request,response);
    }
    else if(pathCode == 2)
    {
        devCtlController(db).service(request,response);
    }
    else if(pathCode == 3)
    {
        devDataController(db).service(request,response);
    }
    else if(pathCode == 4)
    {
        orderController(db).service(request,response);
    }
    else if(pathCode == 5)
    {
        powerStationController(db).service(request,response);
    }
    else if(pathCode == 6)
    {
        usercontroller(db).service(request,response);
    }


    sqlconnectpool::closeConnection_clh(db,db_name);
}
