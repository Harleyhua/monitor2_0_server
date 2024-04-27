#include "abstract_emu.h"

#include "abstract_bym.h"
#include "emu_socket.h"



#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "common.h"

#include "ag_rack_mi_table.h"
#include "ag_mi_property_table.h"
#include "ag_mi_cmd_table.h"

#include "basic_bym_factory.h"

#include "abstract_bym.h"

abstract_emu::abstract_emu(QString name, emu_type type, QObject *parent)
    :QObject{parent}
{
    m_name = name;
    m_type = type;

    m_offline_date = QDateTime::currentDateTime();

    //set_mapping();

    connect(&m_timer,&QTimer::timeout,this,&abstract_emu::onm_timer_callback);

    m_timer.start(60000);
}

abstract_emu::~abstract_emu()
{

    clear_mis();
}

void abstract_emu::append_mi(abstract_bym *mi)
{
    if(mi != nullptr)
    {
        m_mis.insert(mi->get_name(),mi);
    }
}

void abstract_emu::remove_mi(QString name)
{
    delete m_mis.value(name);

    m_mis.remove(name);
}

void abstract_emu::onm_timer_callback()
{
    QDateTime current_date = QDateTime::currentDateTime();
    //如果离线时间大于设置值
    if(m_offline_date.secsTo(current_date) > m_offline_limit)
    {
        onm_close();

    }
}

void abstract_emu::onm_close()
{
    m_timer.stop();
    disconnect(&m_timer,&QTimer::timeout,this,&abstract_emu::onm_timer_callback);
    //计数器不会被抢断   无需互斥
    if(!m_close_req)
    {
        m_close_req = true;
        emit s_close(m_name);  //向上层报告socket离线
    }
}

void abstract_emu::set_mapping()
{
    mysql sql(m_name);
    QJsonObject s_data;
    QJsonObject r_data;
    QJsonObject params;

    QJsonObject sp_data;
    QJsonObject rp_data;
    QJsonObject p_params;
    QJsonArray mis_cid;

    QStringList mis;

    params.insert(ag_rack_mi_table::c_field_emu_cid,m_name);
    s_data.insert("params",params);

    sql.r_emu_mapping(s_data,r_data);

    for(int i=0;i<r_data.value("datas").toArray().size();i++)
    {
        mis.append(r_data.value("datas").toArray()[i].toObject().value(ag_rack_mi_table::c_field_mi_cid).toString());
    }

    for(int i=0;i<mis.size();i++)
    {
        mis_cid.append(mis[i]);
    }
    p_params.insert(ag_mi_property_table::c_field_cid,mis_cid);

    sp_data.insert("params",p_params);

    sql.r_mi_property(sp_data,rp_data);

    //清空当前的绑定关系
    clear_mis();

    QJsonArray pty_data;
    basic_bym_factory bym_factory;
    pty_data = rp_data.value("datas").toArray();
    for(int i=0;i< pty_data.size();i++)
    {
        bym_type tmp_type;
        QString mi_name ;
        abstract_bym *tmp_bym = nullptr;
        mi_name = pty_data[i].toObject().value(ag_mi_property_table::c_field_cid).toString();
        //编号符合   允许创建对象
        tmp_type = abstract_bym::analysis_type(mi_name);
        if(tmp_type != bym_type::bym_empty_type)
        {
            tmp_bym = bym_factory.create_emu(mi_name,tmp_type);
            if(tmp_bym != nullptr)
            {
                tmp_bym->set_property(QString::number(pty_data[i].toObject().value(ag_mi_property_table::c_field_nominal_power).toInt()),
                                      pty_data[i].toObject().value(ag_mi_property_table::c_field_mim_version).toString(),
                                      pty_data[i].toObject().value(ag_mi_property_table::c_field_mis_version).toString());
                m_mis.insert(mi_name,tmp_bym);
            }
        }
        else
        {

        }
    }
}

void abstract_emu::clear_mis()
{
    for (QHash<QString,abstract_bym *>::const_iterator it = m_mis.cbegin(), end = m_mis.cend(); it != end; ++it)
    {
        if(it.value() != nullptr)
        {
            delete it.value();
        }
    }
    m_mis.clear();
}

void abstract_emu::get_server_cmd(uint8_t &cmd, QByteArray &rt_data)
{
    mysql sql(m_name);
    for(int i=0;i<m_mis.keys().size();i++)
    {
        QJsonObject s_data;
        QJsonObject r_data;
        QJsonObject params;
        params.insert(ag_mi_cmd_table::c_field_id,m_name);
        s_data.insert("params",params);
        //当前微逆编号有控制命令
        //if(sql.r_mi_cmd(s_data,r_data))
        {
            //
            QJsonObject datas;
            datas = r_data.value("datas").toObject();
            cmd = datas.value(ag_mi_cmd_table::c_field_cmd).toInt();
            rt_data = QByteArray::fromBase64(datas.value(ag_mi_cmd_table::c_field_data).toString().toUtf8());
            break;
        }
    }
}

bool abstract_emu::is_general_cid_valid(QString emu_cid)
{
    if(emu_cid =="")
    {
        return false;
    }

    //16进制数 8位
    QRegExp hexMatcher("^[0-9A-Fa-f]{8}$");
    if(hexMatcher.exactMatch(emu_cid) && (emu_cid[0] == '9'))
        return true;
    return false;
}

bool abstract_emu::is_wifiemu_cid_valid(QString wifiemu_cid)
{
    if(wifiemu_cid =="")
    {
        return false;
    }
    //16进制数 8位
    QRegExp hexMatcher("^[0-9A-Fa-f]{8}$");
    if(hexMatcher.exactMatch(wifiemu_cid) && (wifiemu_cid[0] == 'A' || wifiemu_cid[0] == 'B'
       || wifiemu_cid[0] == 'C' || wifiemu_cid[0] == 'a' || wifiemu_cid[0] == 'b' || wifiemu_cid[0] == 'c'))
        return true;
    return false;
//    //16进制数 8位
//    QRegExp hexMatcher("^[0-9A-Fa-f]{8}$");
//    if(hexMatcher.exactMatch(wifiemu_cid) && (wifiemu_cid[0] == 'A' || wifiemu_cid[0] == 'B'
//       || wifiemu_cid[0] == 'C' || wifiemu_cid[0] == 'a' || wifiemu_cid[0] == 'b' || wifiemu_cid[0] == 'c'))
//        return true;
//    return false;
}
QString abstract_emu::emu_cid_to_wifi_micid(QString wifiemu_cid)
{
    if(is_wifiemu_cid_valid(wifiemu_cid))
    {
        if(wifiemu_cid[0] == "A")
        {
            wifiemu_cid[0] = '1';
        }
        else if(wifiemu_cid[0] == "B")
        {
            wifiemu_cid[0] = '2';
        }
        else if(wifiemu_cid[0] == "C")
        {
            wifiemu_cid[0] = '4';
        }
    }
    else
    {
        wifiemu_cid = "";
    }
    return wifiemu_cid;
}
