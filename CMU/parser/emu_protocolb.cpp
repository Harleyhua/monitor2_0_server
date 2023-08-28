#include "emu_protocolb.h"

#include "common.h"
#include "QsLog.h"

#include "ag_gateway_data_table.h"
#include "ag_power_data_table.h"
#include "ag_rack_mi_table.h"
#include "ag_mi_property_table.h"
#include "ag_emu_property_table.h"

const quint8 emu_protocolb::C_HEAD = 0X64;
const quint8 emu_protocolb::C_TAIL = 0X23;

const quint8 emu_protocolb::C_LOGIN_CMD = 0x01;
const quint8 emu_protocolb::C_HAND_CMD = 0x02;
const quint8 emu_protocolb::C_POWER_CMD = 0x03;
const quint8 emu_protocolb::C_MAPPING_CMD = 0x04;
const quint8 emu_protocolb::C_REISSUE_MSG_CMD = 0x05;
const quint8 emu_protocolb::C_REISSUE_DATA_MSG_CMD = 0X85;
const quint8 emu_protocolb::C_SET_MAPPING_CMD = 0X84;

const quint8 emu_protocolb::C_GET_TEMPORARY_POWER = 0X06; //网关向服务器查 临时功率
const quint8 emu_protocolb::C_SET_TEMPORARY_POWER = 0X86; //网关发送 临时功率

const quint8 emu_protocolb::C_GET_MAX_POWER = 0X07; //
const quint8 emu_protocolb::C_SET_MAX_POWER = 0X87;

const quint8 emu_protocolb::C_GET_GRID = 0X08;
const quint8 emu_protocolb::C_SET_GRID = 0X88;

const quint8 emu_protocolb::C_GET_CERTIFICATION = 0X09;
const quint8 emu_protocolb::C_SET_CERTIFICATION = 0X89;

const quint8 emu_protocolb::C_GET_COUNTERCURRENT = 0X71; //0X71
const quint8 emu_protocolb::C_SET_COUNTERCURRENT = 0XE1;

const quint8 emu_protocolb::C_EMU_STATUS = 0XE0;  //网关自身状态

const QHash<quint8, quint8> emu_protocolb::c_server_to_client = {
    {0x00,0x00},
    {0x01,0x04},
    //{0x02,0x06},
    {0x03,0x06},
    {0x04,0x06},
    {0x05,0x07},
    {0x06,0x07},
    {0x07,0x08},
    {0x08,0x09},
};

emu_protocolb::emu_protocolb(QObject *parent)
    : QObject{parent}
{
}

bool emu_protocolb::data_analysis(QByteArray &netdata, uint8_t &rt_cmd, quint32 &rt_length)
{
    //获取缓存区当前数据长度
    uint32_t data_size = netdata.size();
    if(netdata[0] == char(0x64) && netdata[1] == char(0x64))
    {
        uint8_t cmd = 0;
        uint16_t length = 0;

        //如果缓冲数据长度  小于 本次报文长度
        if(data_size< 15)
        {
            QLOG_WARN() << tr("协议B:网关数据 还未收全 继续接收");
            return false;
        }
        else
        {
            cmd = common::qbtarray_to_u8(netdata,14);
            //数据域为NULL
//            if(cmd == HAND_CMD_B || cmd == MAPPING_CMD_B)
//            {
//                length = 18;
//            }

            if(data_size< 18)
            {
                QLOG_WARN() << tr("协议B:网关数据 还未收全 继续接收");
                return false;
            }
            //数据域长度字节 1
            if(cmd == C_LOGIN_CMD || cmd == C_REISSUE_MSG_CMD || cmd == C_GET_TEMPORARY_POWER||
               cmd == C_SET_TEMPORARY_POWER || cmd == C_GET_MAX_POWER || cmd == C_GET_GRID ||
               cmd == C_GET_CERTIFICATION || cmd == C_SET_CERTIFICATION || cmd == C_EMU_STATUS ||
               cmd ==  C_GET_COUNTERCURRENT || cmd == C_SET_COUNTERCURRENT)
            {
                //一帧数据长度  B协议 固定字节长度20
                length = common::qbtarray_to_u8(netdata,15) + 19;
            }
            else if(cmd == C_POWER_CMD || cmd == C_SET_MAPPING_CMD || cmd == C_REISSUE_DATA_MSG_CMD ||
                    cmd == C_SET_MAX_POWER || cmd == C_SET_GRID)
            {
                length = common::qbtarray_to_u16(netdata,15) + 20;
            }//数据域全是空
            else if(cmd == C_HAND_CMD || cmd == C_MAPPING_CMD)
            {
                length = 18;
            }
            else {
                QLOG_INFO() << "服务器接收: " + QString(netdata.toHex(' '));
                QLOG_WARN() << "协议B指令异常";
                netdata.clear();
                return false;
            }

            if(data_size < length)
            {
                QLOG_WARN() << tr("协议B:网关数据 还未收全 继续接收");
                return false;
            }
            if(netdata[length-1] != char(0x23) ||netdata[length-2] != char(0x23))
            {
                QLOG_INFO() << "服务器接收: " + QString(netdata.toHex(' '));
                QLOG_WARN() << tr("协议B:网关下发的数据结尾 0x23 0x23 错误");
                netdata.clear();
                return false;
            }

            //校验数据
            uint8_t cs_ret = csCalculate_B(netdata,length);
            if(cs_ret == (uint8_t)netdata[length-3])
            {
                QLOG_INFO() << "服务器接收: " + QString(netdata.toHex(' '));
                rt_cmd = cmd;
                rt_length = length;
//                    deal_cmd_B(netdata,cmd);
//                    pop_cache(length); //处理过的数据抛出
                return true;
            }
            else {
                QLOG_INFO() << "服务器接收: " + QString(netdata.toHex(' '));
                QLOG_WARN() << "协议B:网关下发的数据校验失败" + QString(netdata.toHex(' '));
                netdata.clear();
                return false;
            }
        }
    }

    return false;
}

bool emu_protocolb::emu_type_analysis(QByteArray &netdata, QString &name, emu_type &type)
{
    //获取缓存区当前数据长度
    uint32_t data_size = netdata.size();
    if(netdata[0] == char(0x64) && netdata[1] == char(0x64))
    {
        uint8_t cmd = 0;
        uint16_t length = 0;

        //如果缓冲数据长度  小于 本次报文长度
        if(data_size< 15)
        {
            QLOG_WARN() << tr("协议B:网关数据 还未收全 继续接收");
            return false;
        }
        else
        {
            cmd = common::qbtarray_to_u8(netdata,14);
            //数据域为NULL
//            if(cmd == HAND_CMD_B || cmd == MAPPING_CMD_B)
//            {
//                length = 18;
//            }

            if(data_size< 18)
            {
                QLOG_WARN() << tr("协议B:网关数据 还未收全 继续接收");
                return false;
            }
            //数据域长度字节 1
            //数据域长度字节 1
            if(cmd == C_LOGIN_CMD || cmd == C_REISSUE_MSG_CMD || cmd == C_GET_TEMPORARY_POWER||
               cmd == C_SET_TEMPORARY_POWER || cmd == C_GET_MAX_POWER || cmd == C_GET_GRID ||
               cmd == C_GET_CERTIFICATION || cmd == C_SET_CERTIFICATION || cmd == C_EMU_STATUS ||
               cmd ==  C_GET_COUNTERCURRENT || cmd == C_SET_COUNTERCURRENT)
            {
                //一帧数据长度  B协议 固定字节长度20
                length = common::qbtarray_to_u8(netdata,15) + 19;
            }
            else if(cmd == C_POWER_CMD || cmd == C_SET_MAPPING_CMD || cmd == C_REISSUE_DATA_MSG_CMD ||
                                       cmd == C_SET_MAX_POWER || cmd == C_SET_GRID)
            {
                length = common::qbtarray_to_u16(netdata,15) + 20;
            }//数据域全是空
            else if(cmd == C_HAND_CMD || C_MAPPING_CMD)
            {
                length = 18;
            }
            else {
                QLOG_INFO() << "服务器接收: " + QString(netdata.toHex(' '));
                QLOG_WARN() << "协议B指令异常";
                netdata.clear();
                return false;
            }

            if(data_size < length)
            {
                QLOG_WARN() << tr("协议B:网关数据 还未收全 继续接收");
                return false;
            }
            if(netdata[length-1] != char(0x23) ||netdata[length-2] != char(0x23))
            {
                QLOG_INFO() << "服务器接收: " + QString(netdata.toHex(' '));
                QLOG_WARN() << tr("协议B:网关下发的数据结尾 0x23 0x23 错误");
                netdata.clear();
                return false;
            }

            //校验数据
            uint8_t cs_ret = csCalculate_B(netdata,length);
            if(cs_ret == (uint8_t)netdata[length-3])
            {
                name = QString::number(common::qbtarray_to_u32(netdata,10),16);
                return true;

//                if(cmd == C_HAND_CMD ||cmd == C_LOGIN_CMD || cmd== C_POWER_CMD)
//                {
//                    name = QString::number(common::qbtarray_to_u32(netdata,10),16);
//                    return true;
// //                    if(type_analysis(common::qbtarray_to_u16(netdata,30),type))
// //                    {
// //                        return true;
// //                    }
// //                    else
// //                    {
// //                        QLOG_INFO() << "服务器接收: " + QString(netdata.toHex(' '));
// //                        netdata.clear();
// //                        return false;
// //                    }
//                }
//                else
//                {
//                     netdata.clear();
//                }
 //                    deal_cmd_B(netdata,cmd);
 //                    pop_cache(length); //处理过的数据抛出
            }
            else {
                QLOG_INFO() << "服务器接收: " + QString(netdata.toHex(' '));
                QLOG_WARN() << "协议B:网关下发的数据校验失败" + QString(netdata.toHex(' '));
                netdata.clear();
                return false;
            }
        }
    }
    else
    {
        netdata.clear();
    }
    return false;
}


bool emu_protocolb::write_frame_head(QByteArray &send_msg, uint16_t msg_index, uint32_t cid, uint8_t server_cmd)
{
    if(send_msg.size() < 15)
    {
        return false;
    }

    QString t_date = common::get_system_detail_time();//格式例：22-10-29 14:02:54

    send_msg[0] = C_HEAD;
    send_msg[1] = C_HEAD;
    send_msg[2] = (msg_index >> 8) | 0x80;
    send_msg[3] = (uint8_t)msg_index;
    send_msg[4] = t_date.midRef(2,2).toUInt();
    send_msg[5] = t_date.midRef(5,2).toUInt();
    send_msg[6] = t_date.midRef(8,2).toUInt();
    send_msg[7] = t_date.midRef(11,2).toUInt();
    send_msg[8] = t_date.midRef(14,2).toUInt();
    send_msg[9] = t_date.midRef(17,2).toUInt();
    send_msg[10] = (uint8_t)(cid >> 24);
    send_msg[11] = (uint8_t)(cid >> 16);
    send_msg[12] = (uint8_t)(cid >> 8);
    send_msg[13] = (uint8_t)cid;
    send_msg[14] = server_cmd;

    return true;
}


void emu_protocolb::to_login_json(const QByteArray &s_data, QJsonObject &rt_data)
{
    QJsonObject js_datas;
    js_datas.insert(ag_gateway_data_table::c_field_emu_cid,QString::number(common::qbtarray_to_u32(s_data,10),16));
    js_datas.insert(ag_gateway_data_table::c_field_action,"login");
    js_datas.insert(ag_gateway_data_table::c_field_version,"");
    js_datas.insert(ag_gateway_data_table::c_field_sys_time,common::get_system_detail_time());

    rt_data.insert("datas",js_datas);
}

void emu_protocolb::to_emu_property_json(const QByteArray &s_data, QJsonObject &rt_data)
{
    QJsonObject js_datas;
    js_datas.insert(ag_emu_property_table::c_field_emu_cid,QString::number(common::qbtarray_to_u32(s_data,10),16));
    js_datas.insert(ag_emu_property_table::c_field_type,
                    QString("%1").arg(common::qbtarray_to_u16(s_data,30),4,16,QLatin1Char('0')));
    js_datas.insert(ag_emu_property_table::c_field_hard_version,
                    QString("%1").arg(common::qbtarray_to_u16(s_data,26),4,16,QLatin1Char('0')));
    js_datas.insert(ag_emu_property_table::c_field_soft_version,
                    QString("%1").arg(common::qbtarray_to_u16(s_data,28),4,16,QLatin1Char('0')));
    rt_data.insert("datas",js_datas);
}

void emu_protocolb::to_handshake_json(const QByteArray &s_data, QJsonObject &rt_data)
{
    QJsonObject js_datas;
    js_datas.insert(ag_gateway_data_table::c_field_emu_cid,QString::number(common::qbtarray_to_u32(s_data,10),16));
    js_datas.insert(ag_gateway_data_table::c_field_action,"handshake");
    js_datas.insert(ag_gateway_data_table::c_field_version,"");
    js_datas.insert(ag_gateway_data_table::c_field_sys_time,common::get_system_detail_time());

    rt_data.insert("datas",js_datas);
}

void emu_protocolb::to_powerdata_json(const QByteArray &s_data, QJsonObject &rt_data,uint16_t &date)
{
    QString t_emu_cid = QString::number(common::qbtarray_to_u32(s_data,10),16);
    QString t_emu_date = QString::number((uint8_t)s_data[4]) + "-" + QString::number((uint8_t)s_data[5]) + "-" +
                         QString::number((uint8_t)s_data[6]) + " " + QString::number((uint8_t)s_data[7]) + ":" +
                         QString::number((uint8_t)s_data[8]) + ":" + QString::number((uint8_t)s_data[9]);
    QString t_cur_date = common::get_system_detail_time();
    uint8_t t_data_nums = (((((uint8_t)s_data[15]) << 8) + (uint8_t)s_data[16])-1) / 25; //设备数  //每路数据固定25个字节
    uint32_t start_index = 0;
    QJsonArray mis_jsarray;

    date = common::get_system_yearmon_time();
    //如果设备 pv数不为整数 直接返回
    if(((((((uint8_t)s_data[15]) << 8) + (uint8_t)s_data[16])-1) % 25) != 0)
    {
        return ;
    }

    for(int i=0;i<t_data_nums;i++)
    {
        QJsonObject mi_json;
        start_index = 18 + 25*i;

        mi_json.insert(ag_power_data_table::c_field_emu_cid,t_emu_cid);
        mi_json.insert(ag_power_data_table::c_field_emu_time,t_emu_date);
        mi_json.insert(ag_power_data_table::c_field_mi_cid,QString::number(common::qbtarray_to_u32(s_data,start_index),16)); //index 18-22);
        mi_json.insert(ag_power_data_table::c_field_pv_id,((uint8_t)s_data[start_index+4]) & 0x0F);
        mi_json.insert(ag_power_data_table::c_field_mim_version,QString::number(((uint8_t)s_data[start_index + 5]) >> 4) +
                       "." + QString::number(((uint8_t)s_data[start_index + 5]) &0x0f));
        mi_json.insert(ag_power_data_table::c_field_mis_version,QString::number(((uint8_t)s_data[start_index + 6]) >> 4) +
                       "." + QString::number(((uint8_t)s_data[start_index + 6]) &0x0f));
        mi_json.insert(ag_power_data_table::c_field_pv,((float)(common::qbtarray_to_u16(s_data,start_index + 7)))/512);    //*64/32768);
        mi_json.insert(ag_power_data_table::c_field_power,((float)(common::qbtarray_to_u16(s_data,start_index + 9)))/32);       //*1024/32768;);
        mi_json.insert(ag_power_data_table::c_field_energy,((float)(common::qbtarray_to_u32(s_data,start_index + 11)))/8192);   //*4/32768;);
        mi_json.insert(ag_power_data_table::c_field_temperature,((float)(common::qbtarray_to_u16(s_data,start_index + 15)))/128-40);   //*256/32768 -40;);
        mi_json.insert(ag_power_data_table::c_field_gridv,((float)(common::qbtarray_to_u16(s_data,start_index + 17)))/64);   //512/32768;);
        mi_json.insert(ag_power_data_table::c_field_gridf,((float)(common::qbtarray_to_u16(s_data,start_index + 19)))/256);  //*128/32768;);

        mi_json.insert(ag_power_data_table::c_field_mim_err,QString("%1").arg(common::qbtarray_to_u16(s_data,start_index +21),4,16,QLatin1Char('0')));
        mi_json.insert(ag_power_data_table::c_field_mis_err,QString("%1").arg(common::qbtarray_to_u16(s_data,start_index +23),4,16,QLatin1Char('0')));
        mi_json.insert(ag_power_data_table::c_field_nominal_power,0);
        mi_json.insert(ag_power_data_table::c_field_reissue_data,"NO");
        mi_json.insert(ag_power_data_table::c_field_sys_time,t_cur_date);

        mis_jsarray.append(mi_json);
    }

    rt_data .insert("datas",mis_jsarray);
}

void emu_protocolb::to_mapping_json(const QByteArray &s_data, QJsonObject &rt_data)
{
    QJsonObject param_json;

    param_json.insert(ag_rack_mi_table::c_field_room,"");
    param_json.insert(ag_rack_mi_table::c_field_emu_cid,
                      QString::number(common::qbtarray_to_u32(s_data,10),16));

    rt_data.insert("params",param_json);
}

//void emu_protocolb::to_reissuemsg_json(const QByteArray &s_data, QJsonObject &rt_data)
//{

//}

void emu_protocolb::to_reissuepower_json(const QByteArray &s_data, QJsonObject &rt_data,uint16_t &date)
{
    QString t_emu_cid = QString::number(common::qbtarray_to_u32(s_data,10),16);
    QString t_emu_date = QString::number((uint8_t)s_data[17]) + "-" + QString::number((uint8_t)s_data[18]) + "-" +
                         QString::number((uint8_t)s_data[19]) + " " + QString::number((uint8_t)s_data[20]) + ":" +
                         QString::number((uint8_t)s_data[21]) + ":" + QString::number((uint8_t)s_data[22]);

    QString t_cur_date = common::get_system_detail_time();
    uint8_t t_data_nums = (((((uint8_t)s_data[15]) << 8) + (uint8_t)s_data[16])-1-6) / 25; //设备数  //每路数据固定25个字节
    uint32_t start_index = 0;
    QJsonArray mis_jsarray;

    date = (uint8_t)s_data[17]*100 + (uint8_t)s_data[18];
    //如果设备 pv数不为整数 直接返回
    if(((((((uint8_t)s_data[15]) << 8) + (uint8_t)s_data[16]) -1-6) % 25) != 0)
    {
        return ;
    }

    for(int i=0;i<t_data_nums;i++)
    {
        QJsonObject mi_json;
        start_index = 24 + 25*i;

        mi_json.insert(ag_power_data_table::c_field_emu_cid,t_emu_cid);
        mi_json.insert(ag_power_data_table::c_field_emu_time,t_cur_date);
        mi_json.insert(ag_power_data_table::c_field_mi_cid,QString::number(common::qbtarray_to_u32(s_data,start_index),16)); //index 18-22);
        mi_json.insert(ag_power_data_table::c_field_pv_id,((uint8_t)s_data[start_index+4]) & 0x0F);
        mi_json.insert(ag_power_data_table::c_field_mim_version,QString::number(((uint8_t)s_data[start_index + 5]) >> 4) +
                       "." + QString::number(((uint8_t)s_data[start_index + 5]) &0x0f));
        mi_json.insert(ag_power_data_table::c_field_mis_version,QString::number(((uint8_t)s_data[start_index + 6]) >> 4) +
                       "." + QString::number(((uint8_t)s_data[start_index + 6]) &0x0f));
        mi_json.insert(ag_power_data_table::c_field_pv,((float)(common::qbtarray_to_u16(s_data,start_index + 7)))/512);    //*64/32768);
        mi_json.insert(ag_power_data_table::c_field_power,((float)(common::qbtarray_to_u16(s_data,start_index + 9)))/32);       //*1024/32768;);
        mi_json.insert(ag_power_data_table::c_field_energy,((float)(common::qbtarray_to_u32(s_data,start_index + 11)))/8192);   //*4/32768;);
        mi_json.insert(ag_power_data_table::c_field_temperature,((float)(common::qbtarray_to_u16(s_data,start_index + 15)))/128-40);   //*256/32768 -40;);
        mi_json.insert(ag_power_data_table::c_field_gridv,((float)(common::qbtarray_to_u16(s_data,start_index + 17)))/64);   //512/32768;);
        mi_json.insert(ag_power_data_table::c_field_gridf,((float)(common::qbtarray_to_u16(s_data,start_index + 19)))/256);  //*128/32768;);

        mi_json.insert(ag_power_data_table::c_field_mim_err,QString("%1").arg(common::qbtarray_to_u16(s_data,start_index +21),4,16,QLatin1Char('0')));
        mi_json.insert(ag_power_data_table::c_field_mis_err,QString("%1").arg(common::qbtarray_to_u16(s_data,start_index +23),4,16,QLatin1Char('0')));
        mi_json.insert(ag_power_data_table::c_field_nominal_power,0);
        mi_json.insert(ag_power_data_table::c_field_reissue_data,"YES");
        mi_json.insert(ag_power_data_table::c_field_sys_time,t_emu_date);

        mis_jsarray.append(mi_json);
    }

    rt_data .insert("datas",mis_jsarray);

}

void emu_protocolb::to_mi_property_json(const QByteArray &s_data, QJsonObject &rt_data)
{
    uint8_t t_data_nums = ((((uint8_t)s_data[15] << 8) + (uint8_t)s_data[16])-1) / 10; //设备数  //每路数据固定25个字节
    uint32_t start_index = 0;

    QJsonArray mis_jsarray;
    for(int i=0;i<t_data_nums;i++)
    {
        QJsonObject mi_property_json;
        start_index = 18 + 10*i;
        mi_property_json.insert(ag_mi_property_table::c_field_cid,QString::number(common::qbtarray_to_u32(s_data,start_index+1),16));
        mi_property_json.insert(ag_mi_property_table::c_field_nominal_power,(uint8_t)s_data[start_index +5] *10);
        mi_property_json.insert(ag_mi_property_table::c_field_mim_version,QString::number(((uint8_t)s_data[start_index + 6]) >> 4) +
                       "." + QString::number(((uint8_t)s_data[start_index + 6]) &0x0f));
        mi_property_json.insert(ag_mi_property_table::c_field_mis_version,QString::number(((uint8_t)s_data[start_index + 7]) >> 4) +
                       "." + QString::number(((uint8_t)s_data[start_index + 7]) &0x0f));

        mis_jsarray.append(mi_property_json);
    }

    rt_data.insert("datas",mis_jsarray);
}

void emu_protocolb::to_emu_status_json(const QByteArray &s_data, QJsonObject &rt_data)
{
    QString t_emu_cid = QString::number(common::qbtarray_to_u32(s_data,10),16);
    QString status = QString("%1").arg(common::qbtarray_to_u32(s_data,16),8,16,QLatin1Char('0'));
    QString status_reserve = QString("%1").arg(common::qbtarray_to_u8(s_data,20),2,16,QLatin1Char('0'));
    QString run_mode = QString("%1").arg(common::qbtarray_to_u8(s_data,21),2,16,QLatin1Char('0'));

    rt_data.insert("emu_cid",t_emu_cid);
    rt_data.insert("status",status);
    rt_data.insert("status_reserve",status_reserve);
    rt_data.insert("run_mode",run_mode);
    rt_data.insert("sys_time",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void emu_protocolb::to_emu_status_act_json(const QByteArray &s_data, QJsonObject &rt_data)
{
    QJsonObject js_datas;
    js_datas.insert(ag_gateway_data_table::c_field_emu_cid,QString::number(common::qbtarray_to_u32(s_data,10),16));
    js_datas.insert(ag_gateway_data_table::c_field_action,"emu_status");
    js_datas.insert(ag_gateway_data_table::c_field_version,"");
    js_datas.insert(ag_gateway_data_table::c_field_sys_time,common::get_system_detail_time());

    rt_data.insert("datas",js_datas);
}

void emu_protocolb::to_login_cmd(const QByteArray &s_data,QByteArray &rt_data,quint8 server_cmd)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);


    rt_data.resize(21);

    write_frame_head(rt_data,msg_index,emu_cid,server_cmd);
    rt_data[15] = C_LOGIN_CMD;
    rt_data[16] = 0X01;//长度
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}

void emu_protocolb::to_handshake_cmd(const QByteArray &s_data,QByteArray &rt_data,quint8 server_cmd)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    rt_data.resize(21);

    write_frame_head(rt_data,msg_index,emu_cid,server_cmd);
    rt_data[15] = C_HAND_CMD;
    rt_data[16] = 0X01;
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}

void emu_protocolb::to_powerdata_cmd(const QByteArray &s_data,QByteArray &rt_data,quint8 server_cmd)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    rt_data.resize(21);

    write_frame_head(rt_data,msg_index,emu_cid,server_cmd);
    rt_data[15] = C_POWER_CMD;
    rt_data[16] = 0X01;
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}

void emu_protocolb::to_mapping_cmd(const QByteArray &s_data,const QJsonObject &s_jsdata,QByteArray &rt_data,quint8 server_cmd)
{
    QJsonArray maps_array;
    bool ok = false;
    quint16 cmd_size = 0;

    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    maps_array = s_jsdata.value("datas").toArray();

    cmd_size = 22 + 5*maps_array.size();
    rt_data.resize(cmd_size);

    write_frame_head(rt_data,msg_index,emu_cid,server_cmd);
    rt_data[15] = C_MAPPING_CMD;
    rt_data[16] = (5*maps_array.size()+1) >> 8;//长度
    rt_data[17] = (5*maps_array.size()+1) & 0x00FF;
    rt_data[18] = maps_array.size();   //微逆编号数量

    for(int i=0;i<maps_array.size();i++)
    {
        quint32 mi_cid = 0;
        uint8_t pv_num = 0;
        mi_cid = maps_array[i].toObject().value(ag_rack_mi_table::c_field_mi_cid).toString().toUInt(&ok,16);

        rt_data[19 +i*5] = uint8_t(mi_cid >> 24);
        rt_data[20 +i*5] = uint8_t(mi_cid >> 16);
        rt_data[21 +i*5] = uint8_t(mi_cid >>8);
        rt_data[22 +i*5] = uint8_t(mi_cid);

        pv_num = (rt_data[19 +i*5] & 0xF0) >>4;
        if(pv_num == 1)
        {
            rt_data[23 +i*5] = 0x00;
        }
        else if(pv_num == 2){
            rt_data[23 +i*5] = 0x10;
        }
        else if(pv_num == 4)
        {
            rt_data[23 +i*5] = 0x20;
        }
        else {
            rt_data[23 +i*5] = 0x00;
        }
    }
    rt_data[cmd_size -3] = csCalculate_B(rt_data,cmd_size);
    rt_data[cmd_size -2] = C_TAIL;
    rt_data[cmd_size -1] = C_TAIL;
}

void emu_protocolb::to_reissuemsg_cmd(const QByteArray &s_data, QByteArray &rt_data,quint8 server_cmd)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    rt_data.resize(21);

    write_frame_head(rt_data,msg_index,emu_cid,server_cmd);
    rt_data[15] = C_REISSUE_MSG_CMD;
    rt_data[16] = 0X01;//长度
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}

void emu_protocolb::to_reissuepower_cmd(const QByteArray &s_data,QByteArray &rt_data,quint8 server_cmd)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    rt_data.resize(21);
    write_frame_head(rt_data,msg_index,emu_cid,server_cmd);
    rt_data[15] = C_REISSUE_DATA_MSG_CMD;
    rt_data[16] = 0X01;
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}

void emu_protocolb::to_mi_property_cmd(const QByteArray &s_data,QByteArray &rt_data,quint8 server_cmd)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    rt_data.resize(21);

    write_frame_head(rt_data,msg_index,emu_cid,server_cmd);
    rt_data[15] = C_SET_MAPPING_CMD;
    rt_data[16] = 0X01;//长度
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}

void emu_protocolb::to_emu_status_cmd(const QByteArray &s_data, QByteArray &rt_data, quint8 server_cmd)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    rt_data.resize(21);

    write_frame_head(rt_data,msg_index,emu_cid,server_cmd);
    rt_data[15] = C_EMU_STATUS;
    rt_data[16] = 0X01;//长度
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}
void emu_protocolb::to_login_cmd_v2(const QByteArray &s_data,uint8_t cmd,QByteArray &rt_data)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);


    rt_data.resize(21);

    write_frame_head(rt_data,msg_index,emu_cid,cmd);
    rt_data[15] = C_LOGIN_CMD;
    rt_data[16] = 0X01;//长度
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}

void emu_protocolb::to_handshake_cmd_v2(const QByteArray &s_data,uint8_t cmd,QByteArray &rt_data)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    rt_data.resize(21);

    write_frame_head(rt_data,msg_index,emu_cid,cmd);
    rt_data[15] = C_HAND_CMD;
    rt_data[16] = 0X01;
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}

void emu_protocolb::to_powerdata_cmd_v2(const QByteArray &s_data,uint8_t cmd,QByteArray &rt_data)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    rt_data.resize(21);

    write_frame_head(rt_data,msg_index,emu_cid,cmd);
    rt_data[15] = C_POWER_CMD;
    rt_data[16] = 0X01;
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}

void emu_protocolb::to_mapping_cmd_v2(const QByteArray &s_data,uint8_t cmd,const QJsonObject &s_jsdata,QByteArray &rt_data)
{
    QJsonArray maps_array;
    bool ok = false;
    quint16 cmd_size = 0;

    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    maps_array = s_jsdata.value("datas").toArray();

    cmd_size = 22 + 5*maps_array.size();
    rt_data.resize(cmd_size);

    write_frame_head(rt_data,msg_index,emu_cid,cmd);
    rt_data[15] = C_MAPPING_CMD;
    rt_data[16] = (5*maps_array.size()+1) >> 8;//长度
    rt_data[17] = (5*maps_array.size()+1) & 0x00FF;
    rt_data[18] = maps_array.size();   //微逆编号数量

    for(int i=0;i<maps_array.size();i++)
    {
        quint32 mi_cid = 0;
        uint8_t pv_num = 0;
        mi_cid = maps_array[i].toObject().value(ag_rack_mi_table::c_field_mi_cid).toString().toUInt(&ok,16);

        rt_data[19 +i*5] = uint8_t(mi_cid >> 24);
        rt_data[20 +i*5] = uint8_t(mi_cid >> 16);
        rt_data[21 +i*5] = uint8_t(mi_cid >>8);
        rt_data[22 +i*5] = uint8_t(mi_cid);

        pv_num = (rt_data[19 +i*5] & 0xF0) >>4;
        if(pv_num == 1)
        {
            rt_data[23 +i*5] = 0x00;
        }
        else if(pv_num == 2){
            rt_data[23 +i*5] = 0x10;
        }
        else if(pv_num == 4)
        {
            rt_data[23 +i*5] = 0x20;
        }
        else {
            rt_data[23 +i*5] = 0x00;
        }
    }
    rt_data[cmd_size -3] = csCalculate_B(rt_data,cmd_size);
    rt_data[cmd_size -2] = C_TAIL;
    rt_data[cmd_size -1] = C_TAIL;
}

void emu_protocolb::to_reissuemsg_cmd_v2(const QByteArray &s_data,uint8_t cmd,QByteArray &rt_data)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    rt_data.resize(21);

    write_frame_head(rt_data,msg_index,emu_cid,cmd);
    rt_data[15] = C_REISSUE_MSG_CMD;
    rt_data[16] = 0X01;//长度
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}

void emu_protocolb::to_reissuepower_cmd_v2(const QByteArray &s_data,uint8_t cmd,QByteArray &rt_data)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    rt_data.resize(21);
    write_frame_head(rt_data,msg_index,emu_cid,cmd);
    rt_data[15] = C_REISSUE_DATA_MSG_CMD;
    rt_data[16] = 0X01;
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}

void emu_protocolb::to_mi_property_cmd_v2(const QByteArray &s_data,uint8_t cmd,QByteArray &rt_data)
{
    uint16_t msg_index = common::qbtarray_to_u16(s_data,2);
    uint32_t emu_cid = common::qbtarray_to_u32(s_data,10);

    rt_data.resize(21);

    write_frame_head(rt_data,msg_index,emu_cid,cmd);
    rt_data[15] = C_SET_MAPPING_CMD;
    rt_data[16] = 0X01;//长度
    rt_data[17] = 0X01;
    rt_data[18] = csCalculate_B(rt_data,21);
    rt_data[19] = C_TAIL;
    rt_data[20] = C_TAIL;
}

bool emu_protocolb::is_b1_3_valid(QString type, QString soft_version)
{




    return false;
}



bool emu_protocolb::type_analysis(quint16 type_valid, emu_type &type)
{
    if(type_valid == 0x0010)
    {
        type = emu_type::bry1000_type;
        return true;
    }
    else if(type_valid == 0x0020)
    {
        type = emu_type::bry2000_type;
        return true;
    }
    else
    {
        type = emu_type::bry1000_type;
        return true;
    }

    return false;
}

uint8_t emu_protocolb::csCalculate_B(const QByteArray &dataarr, uint16_t dataLenth)
{
    uint32_t tmpCsCheckSum = 0;
    for(int i = 2; i < dataLenth-3; i++)
    {
        tmpCsCheckSum += (uint8_t)dataarr[i];
    }
    return (uint8_t)(tmpCsCheckSum & 0xFF);
}


