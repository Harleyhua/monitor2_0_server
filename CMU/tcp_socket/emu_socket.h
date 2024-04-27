#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDateTime>
#include <QTimer>

//#include "abstract_bym.h"
//#include "abstract_emu.h"


class abstract_emu;
class abstract_bym;

//class tcp_server;

class emu_socket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit emu_socket(QObject *parent = nullptr);
    ~emu_socket();

    bool m_offline_switch = true;
    uint32_t m_offline_limit = 300;  //离线删除 5分钟

    QDateTime m_offline_date;

    QTimer *m_timer = nullptr;

    QByteArray m_data_cache;   //socket_data_cache

public slots:   
    void onm_deal_socket_data(); //获取到新数据
    void onm_transfer_socket_data();

    void onm_start_timer();
    void onm_timer_callback();
    void onm_stop_timer();

    void onm_close();
private:
    bool m_close_req = false;
signals:
    void s_new_msg(QByteArray data);   //得到新数据

    void s_new_emu(QString name,emu_socket *socket ,abstract_emu * emu);//得到一个网关

    void s_close(emu_socket *ptr);//当前处于离线状态
};

#endif // TCP_SOCKET_H
