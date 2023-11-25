#ifndef EMU_PROTOCOLB_H
#define EMU_PROTOCOLB_H

#include <QObject>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "abstract_emu.h"


class emu_protocolb : public QObject
{
    Q_OBJECT
public:
    explicit emu_protocolb(QObject *parent = nullptr);

    bool data_analysis(QByteArray &netdata,uint8_t &cmd,quint32 &length);
    bool emu_type_analysis(QByteArray &netdata,QString &name,emu_type &type);

    void to_login_json(const QByteArray &s_data,QJsonObject &rt_data);
    void to_emu_property_json(const QByteArray &s_data,QJsonObject &rt_data);

    void to_handshake_json(const QByteArray &s_data,QJsonObject &rt_data);
    void to_powerdata_json(const QByteArray &s_data,QJsonObject &rt_data,uint16_t &date);
    void to_mapping_json(const QByteArray &s_data,QJsonObject &rt_data);   //获取设备列表
    void to_reissuemsg_json(const QByteArray &s_data,QJsonObject &rt_data);
    void to_reissuepower_json(const QByteArray &s_data,QJsonObject &rt_data,uint16_t &date);
    void to_mi_property_json(const QByteArray &s_data,QJsonObject &rt_data);
    void to_emu_status_json(const QByteArray &s_data,QString sys_time ,QJsonObject &rt_data);
    void to_emu_status_act_json(const QByteArray &s_data,QString sys_time,QJsonObject &rt_data);

    void to_login_cmd(const QByteArray &s_data,QByteArray &rt_data,quint8 server_cmd);
    void to_handshake_cmd(const QByteArray &s_data,QByteArray &rt_data,quint8 server_cmd);
    void to_powerdata_cmd(const QByteArray &s_data,QByteArray &rt_data,quint8 server_cmd);
    void to_mapping_cmd(const QByteArray &s_data,const QJsonObject &s_jsdata,QByteArray &rt_data,quint8 server_cmd);   //获取设备列表
    void to_reissuemsg_cmd(const QByteArray &s_data,QByteArray &rt_data,quint8 server_cmd);
    void to_reissuepower_cmd(const QByteArray &s_data,QByteArray &rt_data,quint8 server_cmd);
    void to_mi_property_cmd(const QByteArray &s_data,QByteArray &rt_data,quint8 server_cmd);
    void to_emu_status_cmd(const QByteArray &s_data,QByteArray &rt_data, quint8 server_cmd);


    void to_login_cmd_v2(const QByteArray &s_data,uint8_t cmd,QByteArray &rt_data);
    void to_handshake_cmd_v2(const QByteArray &s_data,uint8_t cmd,QByteArray &rt_data);
    void to_powerdata_cmd_v2(const QByteArray &s_data,uint8_t cmd,QByteArray &rt_data);
    void to_mapping_cmd_v2(const QByteArray &s_data,uint8_t cmd,const QJsonObject &s_jsdata,QByteArray &rt_data);   //获取设备列表
    void to_reissuemsg_cmd_v2(const QByteArray &s_data,uint8_t cmd,QByteArray &rt_data);
    void to_reissuepower_cmd_v2(const QByteArray &s_data,uint8_t cmd,QByteArray &rt_data);
    void to_mi_property_cmd_v2(const QByteArray &s_data,uint8_t cmd,QByteArray &rt_data);

    void to_set_temporary_power_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);
    void to_micid_temporary_power_cmd_v2(const QByteArray &s_data,const QString &server_data,QByteArray &rt_data,quint8 server_cmd);
    void to_get_temporary_power_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);

    void to_set_more_temporary_power_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);
    void to_micid_more_temporary_power_cmd_v2(const QByteArray &s_data,const QString &server_data,QByteArray &rt_data,quint8 server_cmd);
    void to_get_more_temporary_power_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);

    void to_set_max_power_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);
    void to_micid_max_power_cmd_v2(const QByteArray &s_data,const QString &server_data,QByteArray &rt_data,quint8 server_cmd);
    void to_get_max_power_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);

    void to_set_more_max_power_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);
    void to_micid_more_max_power_cmd_v2(const QByteArray &s_data,const QString &server_data,QByteArray &rt_data,quint8 server_cmd);
    void to_get_more_max_power_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);

    void to_set_grid_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);
    void to_micid_grid_cmd_v2(const QByteArray &s_data,const QString &server_data,QByteArray &rt_data,quint8 server_cmd);
    void to_get_grid_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);

    void to_set_certification_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);
    void to_micid_certification_cmd_v2(const QByteArray &s_data,const QString &server_data,QByteArray &rt_data,quint8 server_cmd);
    void to_get_certification_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);

    void to_set_emu_func_cmd_v2(const QByteArray &s_data,const QByteArray &server_data,QByteArray &rt_data,quint8 server_cmd);

    //void to_server_cmd();
    //b1.0
    static const quint8 C_HEAD;
    static const quint8 C_TAIL;
    static const quint8 C_LOGIN_CMD;
    static const quint8 C_HAND_CMD;
    static const quint8 C_POWER_CMD;
    static const quint8 C_MAPPING_CMD;
    static const quint8 C_REISSUE_MSG_CMD;
    static const quint8 C_REISSUE_DATA_MSG_CMD;
    static const quint8 C_SET_MAPPING_CMD;  //0X84
    //b2.0
    static const QHash<quint8, quint8> c_server_to_client;
    static bool is_b1_3_valid(QString type,QString soft_version);

    static const quint8 C_SET_TEMPORARY_POWER;
    static const quint8 C_MICID_TEMPORARY_POWER;
    static const quint8 C_GET_TEMPORARY_POWER;

    static const quint8 C_SET_MORE_TEMPORARY_POWER; //多台
    static const quint8 C_MICID_MORE_TEMPORARY_POWER;
    static const quint8 C_GET_MORE_TEMPORARY_POWER;

    static const quint8 C_SET_MAX_POWER;
    static const quint8 C_MICID_MAX_POWER;
    static const quint8 C_GET_MAX_POWER;

    static const quint8 C_SET_MORE_MAX_POWER;
    static const quint8 C_MICID_MORE_MAX_POWER;
    static const quint8 C_GET_MORE_MAX_POWER;

    static const quint8 C_SET_GRID;
    static const quint8 C_MICID_GRID;
    static const quint8 C_GET_GRID;

    static const quint8 C_SET_CERTIFICATION;
    static const quint8 C_MICID_CERTIFICATION;
    static const quint8 C_GET_CERTIFICATION;

    static const quint8 C_SET_EMU_FUNC;  //设置网关功能码
    static const quint8 C_EMU_STATUS;
private:
    bool type_analysis(quint16 type_valid,emu_type &type);

    bool write_frame_head(QByteArray &send_msg,uint16_t msg_index,uint32_t cid,uint8_t server_cmd);
    uint8_t csCalculate_B(const QByteArray &dataarr, uint16_t dataLenth);

};

#endif // EMU_PROTOCOLB_H
