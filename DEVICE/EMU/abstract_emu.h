#ifndef ABSTRACT_EMU_H
#define ABSTRACT_EMU_H

#include <QObject>
#include <QHash>
#include <QTcpSocket>
#include <QByteArray>

#include "mysql.h"

/*
 * 网关基类
*/





class abstract_bym;  //前置声明
class emu_socket;
//class mysql;

enum emu_type
{
    bry1000_type = 1,
    bry2000_type = 2
};



class abstract_emu : public QObject
{
    Q_OBJECT
public:
    explicit abstract_emu(QString name, emu_type type,QObject *parent = nullptr);
    virtual ~abstract_emu();

    virtual bool is_mi_exist(QString name)
    {
        if(m_mis.contains(name))
        {
            return true;
        }
        return false;
    };
    virtual QStringList get_mis_name()
    {
        return m_mis.keys();
    };


    virtual quint32 get_mi_size()
    {
        return m_mis.size();
    };
    virtual void append_mi(abstract_bym * mi);
    virtual void remove_mi(QString name);
    virtual void set_socket(emu_socket * socket){
        m_socket = socket;
    };

    virtual void onm_deal_msg(QByteArray data) = 0;

    virtual void onm_timer_callback();

    virtual void onm_close();

    void set_mapping(); //获取组网对象

    void get_server_cmd(uint8_t &cmd,QByteArray &rt_data);
    static bool is_general_cid_valid(QString emu_cid);
    static bool is_wifiemu_cid_valid(QString wifiemu_cid);
    static QString emu_cid_to_wifi_micid(QString wifiemu_cid);
signals:
    void s_close(QString name);  //通知上层删除
private:
    QString m_version = "";

    bool m_close_req = false;        //断开同步请求标志
    uint32_t m_offline_limit = 300;  //离线删除 设定时间5分钟

    QTimer m_timer;

    QHash<QString,abstract_bym*> m_mis;  //网关下绑定的微逆列表

    void clear_mis();
protected:
    QString m_name = "";
    emu_type m_type;           //网关类型
    QDateTime m_offline_date;  //离线日期

    emu_socket *m_socket = nullptr;
    QByteArray m_data_cache;   //socket_data_cache

signals:

};

//}
#endif // ABSTRACT_EMU_H
