#include "bry1000.h"

#include "emu_protocolb.h"

#include "emu_socket.h"
#include "QsLog.h"
#include "common.h"
#include "abstract_bym.h"
void bry1000::onm_deal_msg(QByteArray data)
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

void bry1000::deal_frame(emu_protocolb &tmp_pcol, QByteArray &frame_data, quint8 cmd)
{
    QJsonObject js_obj;
    QByteArray rt_data;  //生成的回应数据
    ag_emu_mi_table emu_mi;
    mysql sql(m_name + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
    uint8_t tmp_server_cmd = 0;
    //读取对该编号网关的控制
    sql.r_device_ctl_first_cmd_nosend(m_name,tmp_dev_ctl);
    tmp_dev_ctl.is_cmd_send = 1;
    tmp_dev_ctl.cmd_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    sql.update_device_cmd_send_flag(tmp_dev_ctl);
    tmp_server_cmd = tmp_dev_ctl.server_cmd;  //发送服务器指令
    //命令相同  设置is_send
//    if(cmd == tmp_dev_ctl.emu_cmd)
//    {
//        dev_ctl_strc tmp_next_dev_ctl = {"",0,"",0,"",0,"",0,""};

//        tmp_dev_ctl.is_send = 1;
//        tmp_dev_ctl.send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//        //设置当前指令标志已发送
//        sql.update_device_send_flag(tmp_dev_ctl);

//        //寻找下一条服务器指令
//        sql.r_device_ctl_nosend(m_name,tmp_next_dev_ctl);

//        tmp_server_cmd = tmp_next_dev_ctl.server_cmd;
//    }
//    else
//    {
//        tmp_server_cmd = tmp_dev_ctl.server_cmd;  //发送服务器指令
//    }

    if(cmd == emu_protocolb::C_LOGIN_CMD)
    {
        tmp_pcol.to_login_json(frame_data,js_obj);
        //sql.w_login(js_obj);
        sql.w_emu_action(js_obj);

        js_obj = QJsonObject();
        tmp_pcol.to_emu_property_json(frame_data,js_obj);
        sql.w_emu_property(js_obj);

        tmp_pcol.to_login_cmd(frame_data,rt_data,tmp_server_cmd);
    }
    else if(cmd == emu_protocolb::C_HAND_CMD)
    {
        tmp_pcol.to_handshake_json(frame_data,js_obj);
        //sql.w_handshake(js_obj);
        sql.w_emu_action(js_obj);
        tmp_pcol.to_handshake_cmd(frame_data,rt_data,tmp_server_cmd);
    }
    else if(cmd == emu_protocolb::C_POWER_CMD)
    {
        uint16_t date = 0;
        // QString station = s.value("station").toString();
        // QString emu = s.value("emu").toString();
        // emu_mi.w_mi(mDataBase,emu,s);
        tmp_pcol.to_powerdata_json(frame_data,js_obj,date);
        sql.w_power(js_obj,date);
        tmp_pcol.to_powerdata_cmd(frame_data,rt_data,tmp_server_cmd);
    }
    else if(cmd == emu_protocolb::C_MAPPING_CMD)
    {
        QJsonObject mapping_js;
        tmp_pcol.to_mapping_json(frame_data,js_obj);
        sql.r_emu_mapping(js_obj,mapping_js);
        tmp_pcol.to_mapping_cmd(frame_data,mapping_js,rt_data,tmp_server_cmd);
    }
    else if(cmd == emu_protocolb::C_REISSUE_MSG_CMD)
    {
        tmp_pcol.to_reissuemsg_cmd(frame_data,rt_data,tmp_server_cmd);
    }
    else if(cmd == emu_protocolb::C_SET_MAPPING_CMD)
    {
        tmp_pcol.to_mi_property_json(frame_data,js_obj);
        sql.w_mi_property(js_obj);
        tmp_pcol.to_mi_property_cmd(frame_data,rt_data,tmp_server_cmd);
    }
    else if(cmd == emu_protocolb::C_REISSUE_DATA_MSG_CMD)
    {
        uint16_t date = 0;
        tmp_pcol.to_reissuepower_json(frame_data,js_obj,date);
        sql.w_power(js_obj,date);
        tmp_pcol.to_reissuepower_cmd(frame_data,rt_data,tmp_server_cmd);
    }
    else if(cmd == emu_protocolb::C_SET_TEMPORARY_POWER)
    {
        QByteArray send_data;
        QString data;

        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"FFFFFFFF",0,""};

        sql.r_device_ctl_last_data_nosend(m_name,cmd,tmp_dev_ctl);

        if(abstract_bym::is_cid_valid(tmp_dev_ctl.send_data))
        {
            //读取数据
            sql.r_mi_temporary_power(tmp_dev_ctl.send_data,data);
            // 命令+长度(size 1) + app_data(micid data)
            data = "0708" + tmp_dev_ctl.send_data + data;
            send_data = common::str_to_qbytarray_h16(data);
            tmp_pcol.to_set_temporary_power_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);

            //更新 数据发送标记
            tmp_dev_ctl.is_data_send = 1;
            tmp_dev_ctl.data_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            sql.update_device_data_send_flag(tmp_dev_ctl);
        }
    }
    else if(cmd == emu_protocolb::C_MICID_TEMPORARY_POWER)
    {
        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,cmd,tmp_dev_ctl);
        //如果存在未发送的 后进先出
        if(tmp_dev_ctl.is_data_send == 0 && tmp_dev_ctl.emu_cmd == cmd)
        {
            tmp_pcol.to_micid_temporary_power_cmd_v2(frame_data,tmp_dev_ctl.send_data,rt_data,tmp_server_cmd);

        }
    }
    else if(cmd == emu_protocolb::C_GET_TEMPORARY_POWER)
    {
        QString mi_cid = QString::number(common::qbtarray_to_u32(frame_data,16),16).toUpper();
        QString data = frame_data.mid(20,4).toHex();
        QByteArray send_data = common::str_to_qbytarray_h16("8705" + mi_cid + "01");
        sql.w_mi_temporary_power(mi_cid,data);

        tmp_pcol.to_get_temporary_power_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);


        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,emu_protocolb::C_MICID_TEMPORARY_POWER,tmp_dev_ctl);
        if(tmp_dev_ctl.is_data_send == 0 && tmp_dev_ctl.emu_cmd == emu_protocolb::C_MICID_TEMPORARY_POWER)
        {
            //更新 数据发送标记
            tmp_dev_ctl.is_data_send = 1;
            tmp_dev_ctl.data_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            sql.update_device_data_send_flag(tmp_dev_ctl);
        }
    }
    else if(cmd == emu_protocolb::C_SET_MORE_TEMPORARY_POWER)
    {
        QByteArray send_data;
        QString data;

        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"FFFFFFFF",0,""};

        sql.r_device_ctl_last_data_nosend(m_name,cmd,tmp_dev_ctl);


        QString mis_data;
        //读取数据
        if(tmp_dev_ctl.send_data.size()%8 == 0)
        {
            for(int i=0;i<tmp_dev_ctl.send_data.size();i=i+8)
            {
                QString tmp_data;
                QString mi_cid = tmp_dev_ctl.send_data.mid(i,8);

                sql.r_mi_temporary_power(mi_cid,tmp_data);

                mis_data.append(mi_cid);
                mis_data.append(tmp_data);
            }

            QString length = QString("%1").arg(mis_data.size()/2,4,16,QLatin1Char('0'));
            // 命令+长度(size 1) + app_data(micid data)
            data = "0B" + length + mis_data;
            send_data = common::str_to_qbytarray_h16(data);
            tmp_pcol.to_set_more_temporary_power_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);
        }

        //更新 数据发送标记
        tmp_dev_ctl.is_data_send = 1;
        tmp_dev_ctl.data_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        sql.update_device_data_send_flag(tmp_dev_ctl);

    }
    else if(cmd == emu_protocolb::C_MICID_MORE_TEMPORARY_POWER)
    {
        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,cmd,tmp_dev_ctl);
        //如果存在未发送的 后进先出
        if(tmp_dev_ctl.is_data_send == 0 && tmp_dev_ctl.emu_cmd == cmd)
        {
            tmp_pcol.to_micid_more_temporary_power_cmd_v2(frame_data,tmp_dev_ctl.send_data,rt_data,tmp_server_cmd);

        }
    }
    else if(cmd == emu_protocolb::C_GET_MORE_TEMPORARY_POWER)
    {
        //QString mi_cid = QString::number(common::qbtarray_to_u32(frame_data,16),16).toUpper();
        //QString data = frame_data.mid(20,4).toHex();
        uint32_t start_index = 0;
        QByteArray send_data = common::str_to_qbytarray_h16("8f0101");
        uint8_t t_data_nums = (((((uint8_t)frame_data[15]) << 8) + (uint8_t)frame_data[16])) / 8;
        for(int i=0;i<t_data_nums;i++)
        {
            start_index = 17 + 8*i;
            QString mi_cid = QString::number(common::qbtarray_to_u32(frame_data,start_index),16).toUpper();
            QString data = frame_data.mid(start_index +4,4).toHex();
            sql.w_mi_temporary_power(mi_cid,data);
        }


        tmp_pcol.to_get_more_temporary_power_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);


        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,emu_protocolb::C_MICID_MORE_TEMPORARY_POWER,tmp_dev_ctl);
        if(tmp_dev_ctl.is_data_send == 0 && tmp_dev_ctl.emu_cmd == emu_protocolb::C_MICID_MORE_TEMPORARY_POWER)
        {
            //更新 数据发送标记
            tmp_dev_ctl.is_data_send = 1;
            tmp_dev_ctl.data_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            sql.update_device_data_send_flag(tmp_dev_ctl);
        }
    }
    else if(cmd == emu_protocolb::C_SET_MAX_POWER)
    {
        QByteArray send_data;
        QString data;
        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"FFFFFFFF",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,cmd,tmp_dev_ctl);
        if(abstract_bym::is_cid_valid(tmp_dev_ctl.send_data))
        {
            //读取数据
            sql.r_mi_max_power(tmp_dev_ctl.send_data,data);
            // 命令+长度(size 1) + app_data(micid data)
            data = "0806" + tmp_dev_ctl.send_data + data;
            send_data = common::str_to_qbytarray_h16(data);
            tmp_pcol.to_set_max_power_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);

            //更新 数据发送标记
            tmp_dev_ctl.is_data_send = 1;
            tmp_dev_ctl.data_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            sql.update_device_data_send_flag(tmp_dev_ctl);
        }
    }
    else if(cmd == emu_protocolb::C_MICID_MAX_POWER)
    {
        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,cmd,tmp_dev_ctl);
        //如果存在未发送的 后进先出
        if(tmp_dev_ctl.is_data_send == 0 && tmp_dev_ctl.emu_cmd == cmd)
        {
            tmp_pcol.to_micid_max_power_cmd_v2(frame_data,tmp_dev_ctl.send_data,rt_data,tmp_server_cmd);

        }
    }
    else if(cmd == emu_protocolb::C_GET_MAX_POWER)
    {
        QString mi_cid = QString::number(common::qbtarray_to_u32(frame_data,16),16).toUpper();
        QString data = frame_data.mid(20,2).toHex();
        QByteArray send_data = common::str_to_qbytarray_h16("8905" + mi_cid + "01");
        sql.w_mi_max_power(mi_cid,data);

        tmp_pcol.to_get_max_power_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);


        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,emu_protocolb::C_MICID_MAX_POWER,tmp_dev_ctl);
        if(tmp_dev_ctl.is_data_send == 0 && tmp_dev_ctl.emu_cmd == emu_protocolb::C_MICID_MAX_POWER)
        {
            //更新 数据发送标记
            tmp_dev_ctl.is_data_send = 1;
            tmp_dev_ctl.data_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            sql.update_device_data_send_flag(tmp_dev_ctl);
        }

    }
    else if(cmd == emu_protocolb::C_SET_MORE_MAX_POWER)
    {
        QByteArray send_data;
        QString data;
        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"FFFFFFFF",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,cmd,tmp_dev_ctl);

        QString mis_data;
        if(tmp_dev_ctl.send_data.size()%8 == 0)
        {
            for(int i=0;i<tmp_dev_ctl.send_data.size();i=i+8)
            {
                QString tmp_data;
                QString mi_cid = tmp_dev_ctl.send_data.mid(i,8);

                sql.r_mi_max_power(mi_cid,tmp_data);

                mis_data.append(mi_cid);
                mis_data.append(tmp_data);
            }
            QString length = QString("%1").arg(mis_data.size()/2,4,16,QLatin1Char('0'));
            // 命令+长度(size 1) + app_data(micid data)
            data = "0C" +length + mis_data;
            send_data = common::str_to_qbytarray_h16(data);
            tmp_pcol.to_set_more_max_power_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);
        }


        //更新 数据发送标记
        tmp_dev_ctl.is_data_send = 1;
        tmp_dev_ctl.data_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        sql.update_device_data_send_flag(tmp_dev_ctl);

    }
    else if(cmd == emu_protocolb::C_MICID_MORE_MAX_POWER)
    {
        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,cmd,tmp_dev_ctl);
        //如果存在未发送的 后进先出
        if(tmp_dev_ctl.is_data_send == 0 && tmp_dev_ctl.emu_cmd == cmd)
        {
            tmp_pcol.to_micid_more_max_power_cmd_v2(frame_data,tmp_dev_ctl.send_data,rt_data,tmp_server_cmd);

        }
    }
    else if(cmd == emu_protocolb::C_GET_MORE_MAX_POWER)
    {
        //QString mi_cid = QString::number(common::qbtarray_to_u32(frame_data,16),16).toUpper();
        //QString data = frame_data.mid(20,2).toHex();
        uint32_t start_index = 0;
        QByteArray send_data = common::str_to_qbytarray_h16("910101");
        uint8_t t_data_nums = (((((uint8_t)frame_data[15]) << 8) + (uint8_t)frame_data[16])) / 6;
        for(int i=0;i<t_data_nums;i++)
        {
            start_index = 17 + 6*i;
            QString mi_cid = QString::number(common::qbtarray_to_u32(frame_data,start_index),16).toUpper();
            QString data = frame_data.mid(start_index +4,2).toHex();

            sql.w_mi_max_power(mi_cid,data);
        }

        tmp_pcol.to_get_more_max_power_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);


        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,emu_protocolb::C_MICID_MORE_MAX_POWER,tmp_dev_ctl);
        if(tmp_dev_ctl.is_data_send == 0 && tmp_dev_ctl.emu_cmd == emu_protocolb::C_MICID_MORE_MAX_POWER)
        {
            //更新 数据发送标记
            tmp_dev_ctl.is_data_send = 1;
            tmp_dev_ctl.data_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            sql.update_device_data_send_flag(tmp_dev_ctl);
        }

    }
    else if(cmd == emu_protocolb::C_SET_GRID)
    {
        QByteArray send_data;
        QString data;
        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"FFFFFFFF",0,""};

        sql.r_device_ctl_last_data_nosend(m_name,cmd,tmp_dev_ctl);
        if(abstract_bym::is_cid_valid(tmp_dev_ctl.send_data))
        {
            //读取数据
            sql.r_mi_grid(tmp_dev_ctl.send_data,data);
            // 命令+长度(size 1) + app_data(micid data)
            data = "0936" + tmp_dev_ctl.send_data + data;
            send_data = common::str_to_qbytarray_h16(data);
            tmp_pcol.to_set_grid_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);


            //更新 数据发送标记
            tmp_dev_ctl.is_data_send = 1;
            tmp_dev_ctl.data_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            sql.update_device_data_send_flag(tmp_dev_ctl);
        }

    }
    else if(cmd == emu_protocolb::C_MICID_GRID)
    {
        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,cmd,tmp_dev_ctl);
        //如果存在未发送的 后进先出
        if(tmp_dev_ctl.is_data_send == 0 && tmp_dev_ctl.emu_cmd == cmd)
        {
            tmp_pcol.to_micid_grid_cmd_v2(frame_data,tmp_dev_ctl.send_data,rt_data,tmp_server_cmd);

        }
    }
    else if(cmd == emu_protocolb::C_GET_GRID)
    {
        QString mi_cid = QString::number(common::qbtarray_to_u32(frame_data,16),16).toUpper();
        QString data = frame_data.mid(20,50).toHex();
        QByteArray send_data = common::str_to_qbytarray_h16("8B05" + mi_cid + "01");
        sql.w_mi_grid(mi_cid,data);

        tmp_pcol.to_get_grid_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);

        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,emu_protocolb::C_MICID_GRID,tmp_dev_ctl);
        if(tmp_dev_ctl.is_data_send == 0 && tmp_dev_ctl.emu_cmd == emu_protocolb::C_MICID_GRID)
        {
            //更新 数据发送标记
            tmp_dev_ctl.is_data_send = 1;
            tmp_dev_ctl.data_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            sql.update_device_data_send_flag(tmp_dev_ctl);
        }
    }
    else if(cmd == emu_protocolb::C_SET_CERTIFICATION)
    {
        QByteArray send_data;
        QString data;

        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"FFFFFFFF",0,""};

        sql.r_device_ctl_last_data_nosend(m_name,cmd,tmp_dev_ctl);

        if(abstract_bym::is_cid_valid(tmp_dev_ctl.send_data))
        {
            //读取数据
            sql.r_mi_certification(tmp_dev_ctl.send_data,data);
            // 命令+长度(size 1) + app_data(micid data)
            data = "0A34" + tmp_dev_ctl.send_data + data;
            send_data = common::str_to_qbytarray_h16(data);
            tmp_pcol.to_set_certification_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);

            //更新 数据发送标记
            tmp_dev_ctl.is_data_send = 1;
            tmp_dev_ctl.data_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            sql.update_device_data_send_flag(tmp_dev_ctl);
        }

    }
    else if(cmd == emu_protocolb::C_MICID_CERTIFICATION)
    {
        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,cmd,tmp_dev_ctl);
        //如果存在未发送的 后进先出
        if(tmp_dev_ctl.is_data_send == 0 && tmp_dev_ctl.emu_cmd == cmd)
        {
            tmp_pcol.to_micid_certification_cmd_v2(frame_data,tmp_dev_ctl.send_data,rt_data,tmp_server_cmd);

        }
    }
    else if(cmd == emu_protocolb::C_GET_CERTIFICATION)
    {
        QString mi_cid = QString::number(common::qbtarray_to_u32(frame_data,16),16).toUpper();
        QString data = frame_data.mid(20,50).toHex();
        QByteArray send_data = common::str_to_qbytarray_h16("8D05" + mi_cid + "01");
        sql.w_mi_certification(mi_cid,data);

        tmp_pcol.to_get_certification_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);

        dev_ctl_strc tmp_dev_ctl = {"",0,"",0,"",0,"",0,""};
        sql.r_device_ctl_last_data_nosend(m_name,emu_protocolb::C_MICID_CERTIFICATION,tmp_dev_ctl);
        if(tmp_dev_ctl.is_data_send == 0 && tmp_dev_ctl.emu_cmd == emu_protocolb::C_MICID_CERTIFICATION)
        {
            //更新 数据发送标记
            tmp_dev_ctl.is_data_send = 1;
            tmp_dev_ctl.data_send_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            sql.update_device_data_send_flag(tmp_dev_ctl);
        }
    }

    else if(cmd == emu_protocolb::C_SET_EMU_FUNC)
    {
        QByteArray send_data;
        QString data;
        QString emu_cid = QString::number(common::qbtarray_to_u32(frame_data,10),16).toUpper();
        //读取数据
        sql.r_emu_func_code(emu_cid,data);
        // 命令+长度(size 1) + app_data(micid data)
        data = "7005" + data;
        send_data = common::str_to_qbytarray_h16(data);
        tmp_pcol.to_set_emu_func_cmd_v2(frame_data,send_data,rt_data,tmp_server_cmd);
    }
    else if(cmd == emu_protocolb::C_EMU_STATUS)
    {
        QString sys_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QJsonObject act_obj;
        tmp_pcol.to_emu_status_act_json(frame_data,sys_time,act_obj);
        sql.w_emu_action(act_obj);

        tmp_pcol.to_emu_status_json(frame_data,sys_time,js_obj);
        sql.w_emu_status(js_obj);
        tmp_pcol.to_emu_status_cmd(frame_data,rt_data,tmp_server_cmd);
    }
    else
    {
        QLOG_WARN() << "网关:" + m_name + "型号:" + QString::number(m_type)
                       + "不支持指令:" + QString::number(cmd);
    }

    m_socket->write(rt_data);
    QLOG_INFO() << "服务器返回:" + QString(rt_data.toHex(' '));
}


