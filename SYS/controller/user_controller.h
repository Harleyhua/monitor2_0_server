#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

#include <QHash>

using namespace stefanfrings;

class user_controller : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(user_controller)
public:
    user_controller(){

    };

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

private:
    HttpRequest *mRequest = nullptr;
    HttpResponse *mRespone = nullptr;
    QHash<QString, void (user_controller::*)()> mMethodMapper=
    {
        {"/r_user_property",&user_controller::getUserProperty},
        {"/r_user_act",&user_controller::getUserOperationRecord},

        {"/update_alg",&user_controller::setUserAlgorithm},

        {"/w_emu_func",&user_controller::setEmuProperty},

        {"/w_temporary_power",&user_controller::setMiTemporaryPower},
        {"/r_temporary_power",&user_controller::getMiTemporaryPower},
        {"/w_max_power",&user_controller::setMiMaxPower},
        {"/r_max_power",&user_controller::getMiMaxPower},
        {"/w_grid",&user_controller::setMiGridParam},
        {"/r_grid",&user_controller::getMiGridParam},
        {"/w_certification",&user_controller::setMiCertification},
        {"/r_certification",&user_controller::getMiCertification},

        {"/r_select_back",&user_controller::getControlResultBack},

        {"/r_emu_action",&user_controller::getEmuAction},
        {"/r_emu_status",&user_controller::getEmuStatus},

        {"/r_rack_data",&user_controller::getRackData},

        {"/r_mi_power",&user_controller::getMIPower},
        {"/r_one_plot_mi_power",&user_controller::getMIPower},
        {"/r_one_mi_power",&user_controller::getMIPower},
        {"/r_ag_room_temp",&user_controller::getRoomTemp},
        {"/r_workorder",&user_controller::getWorkOrder},
        {"/w_workorder",&user_controller::setWorkOrder},

        {"/add_station",&user_controller::addStation},
        {"/del_station",&user_controller::delStation},

        {"/add_emu_cid",&user_controller::addEmu},
        {"/del_emu",&user_controller::delEmu},

        {"/add_plcmi_cid",&user_controller::addPlcMi},
        {"/add_wifimi_cid",&user_controller::addWiFiMi},
        {"/del_mi",&user_controller::delMi},


        {"/r_mi_current_power",&user_controller::getUserDeviceMapperAndLastData},
        {"/aging_report",&user_controller::getMiAgingReport},
        {"/batch_list",&user_controller::getMiAgingBatch}
    };

    void setRequestAndResponse(HttpRequest& request, HttpResponse& response){
        mRequest = &request;
        mRespone = &response;
    };



    void getUserProperty();
    void getUserOperationRecord();

    void setUserAlgorithm();

    void setEmuProperty();

    void setMiTemporaryPower();
    void getMiTemporaryPower();
    void setMiMaxPower();
    void getMiMaxPower();
    void setMiGridParam();
    void getMiGridParam();
    void setMiCertification();
    void getMiCertification();

    void getControlResultBack();  //远程控制回读



    //数据库查询穿透
    void getEmuAction();
    void getEmuStatus();

    void getRackData();

    void getMIPower();
    void getRoomTemp();
    void getWorkOrder();
    void setWorkOrder();

    //设备节点关系操作
    void addStation();
    void delStation();

    void addEmu();
    void delEmu();

    void addPlcMi();
    void addWiFiMi();
    void delMi();


    //综合数据
    void getUserDeviceMapperAndLastData(); //读取用户下的账号关系和最新数据
    //老化相关
    void getMiAgingReport();

    void getMiAgingBatch();

signals:

};

#endif // USER_CONTROLLER_H
