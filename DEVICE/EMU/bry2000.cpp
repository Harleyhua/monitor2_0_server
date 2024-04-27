#include "bry2000.h"

#include "emu_protocolb.h"

#include "QsLog.h"
#include "emu_socket.h"
void bry2000::onm_deal_msg(QByteArray data)
{
    emu_protocolb tmp_pcol;
    quint8 frame_cmd = 0;
    quint32 frame_length = 0;

    m_offline_date = QDateTime::currentDateTime();

    //加入缓存区
    m_data_cache.append(data);

    while (m_data_cache.size() > 0) {
        frame_cmd = 0;
        frame_length = 0;
        //数据异常则会清空  若未接收完全则跳出继续接收
        //可能会同时收到多条指令 处理完后继续处理下一条
        if(tmp_pcol.data_analysis(m_data_cache,frame_cmd,frame_length))
        {
            deal_frame(tmp_pcol,m_data_cache,frame_cmd);
            //处理完后移除当前帧
            m_data_cache.remove(0,frame_length);
        }
        else
        {
            break;
        }
    }


}

void bry2000::deal_frame(emu_protocolb &tmp_pcol, QByteArray &frame_data, quint8 cmd)
{
    QJsonObject js_obj;
    QByteArray rt_data;  //生成的回应数据

    uint8_t server_cmd;
    QByteArray cmd_data;

    mysql sql(m_name);
    get_server_cmd(server_cmd,cmd_data);

    if(cmd == emu_protocolb::C_LOGIN_CMD)
    {
        tmp_pcol.to_login_json(frame_data,js_obj);
        sql.w_login(js_obj);
        tmp_pcol.to_login_cmd_v2(frame_data,server_cmd,rt_data);
    }
    else if(cmd == emu_protocolb::C_HAND_CMD)
    {
        tmp_pcol.to_handshake_json(frame_data,js_obj);
        sql.w_handshake(js_obj);
        tmp_pcol.to_handshake_cmd_v2(frame_data,server_cmd,rt_data);
    }
    else if(cmd == emu_protocolb::C_POWER_CMD)
    {
        uint16_t date = 0;
        tmp_pcol.to_powerdata_json(frame_data,js_obj,date);
        sql.w_power(js_obj,date);
        tmp_pcol.to_powerdata_cmd_v2(frame_data,server_cmd,rt_data);
    }
    else if(cmd == emu_protocolb::C_MAPPING_CMD)
    {
        QJsonObject mapping_js;
        tmp_pcol.to_mapping_json(frame_data,js_obj);
        sql.r_emu_mapping(js_obj,mapping_js);
        tmp_pcol.to_mapping_cmd_v2(frame_data,server_cmd,mapping_js,rt_data);
    }
    else if(cmd == emu_protocolb::C_REISSUE_MSG_CMD)
    {
        tmp_pcol.to_reissuemsg_cmd_v2(frame_data,server_cmd,rt_data);
    }
    else if(cmd == emu_protocolb::C_SET_MAPPING_CMD)
    {
        tmp_pcol.to_mi_property_json(frame_data,js_obj);
        sql.w_mi_property(js_obj);
        tmp_pcol.to_mi_property_cmd_v2(frame_data,server_cmd,rt_data);
    }
    else if(cmd == emu_protocolb::C_REISSUE_DATA_MSG_CMD)
    {
        uint16_t date = 0;
        tmp_pcol.to_reissuepower_json(frame_data,js_obj,date);
        sql.w_power(js_obj,date);
        tmp_pcol.to_reissuepower_cmd_v2(frame_data,server_cmd,rt_data);
    }
    else if(cmd == emu_protocolb::C_GET_TEMPORARY_POWER)
    {

    }
    else if(cmd == emu_protocolb::C_SET_TEMPORARY_POWER)
    {

    }
    else if(cmd == emu_protocolb::C_GET_MAX_POWER)
    {

    }
    else if(cmd == emu_protocolb::C_SET_MAX_POWER)
    {

    }
    else if(cmd == emu_protocolb::C_GET_GRID)
    {

    }
    else if(cmd == emu_protocolb::C_SET_GRID)
    {

    }
    else if(cmd == emu_protocolb::C_GET_CERTIFICATION)
    {

    }
    else if(cmd == emu_protocolb::C_SET_CERTIFICATION)
    {

    }
    else
    {
        QLOG_WARN() << "网关:" + m_name + "型号:" + QString::number(m_type)
                       + "不支持指令:" + QString::number(cmd);
    }

    m_socket->write(rt_data);


}
