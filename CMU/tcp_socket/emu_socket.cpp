#include "emu_socket.h"
//#include "open_lib/QsLog/include/QsLog.h"

#include <QThread>
#include "QsLog.h"
//#include "common.h"

#include "emu_protocolb.h"
#include "basic_emu_factory.h"

emu_socket::emu_socket(QObject *parent)
    : QTcpSocket{parent}
{
    connect(this,&emu_socket::readyRead,this,&emu_socket::onm_deal_socket_data);

    m_offline_date = QDateTime::currentDateTime();

}

emu_socket::~emu_socket()
{

}

void emu_socket::onm_deal_socket_data()
{
    QByteArray data = readAll();

    emu_protocolb tmp_pol;

    QString name;
    emu_type type;

    //加入缓存区
    m_data_cache.append(data);

    m_offline_date = QDateTime::currentDateTime(); //更新最新时间

    while(m_data_cache.size() > 0)
    {
        //数据异常则会清空  若未接收完全则跳出继续接收
        //可能会同时收到多条指令 处理完后继续处理下一条
        if(tmp_pol.emu_type_analysis(m_data_cache,name,type))
        {
            basic_emu_factory factory;
            abstract_emu *tmp_emu = nullptr;

            tmp_emu = factory.create_emu(name,emu_type::bry1000_type);
            if(tmp_emu != nullptr)
            {
                //断开socket的独立处理 转移到emu内部
                disconnect(this,&emu_socket::readyRead,this,&emu_socket::onm_deal_socket_data);
                disconnect(this,&emu_socket::disconnected,this,&emu_socket::onm_close);
                //停掉socket的所有事件,转移到emu
                if(m_timer != nullptr)
                {
                    m_timer->stop();
                }
                else
                {
                    onm_start_timer();
                }
                //切换处理函数
                connect(this,&emu_socket::readyRead,this,&emu_socket::onm_transfer_socket_data);
                connect(this,&emu_socket::s_new_msg,tmp_emu,&abstract_emu::onm_deal_msg);
                //socket 断开处理
                connect(this,&emu_socket::disconnected,this,&emu_socket::onm_stop_timer);
                connect(this,&emu_socket::disconnected,tmp_emu,&abstract_emu::onm_close);

                //把socket绑定到 emu
                tmp_emu->set_socket(this);
                tmp_emu->onm_deal_msg(m_data_cache);

                m_offline_switch = false;  //取消离线删除开关

                emit s_new_emu(name,this,tmp_emu);
            }
            else
            {
                QLOG_WARN() << "当前资源不足:无法创建网关 " + name;
            }

            m_data_cache.clear();
        }
        else
        {
            break;
        }
    }

    //QLOG_DEBUG() << QStringLiteral("socket接收 当前线程id : %1").arg(quintptr(QThread::currentThreadId()));
}

void emu_socket::onm_transfer_socket_data()
{
    QByteArray data = readAll();

    emit s_new_msg(data);
}

void emu_socket::onm_start_timer()
{
    m_timer = new QTimer();
    connect(m_timer,&QTimer::timeout,this,&emu_socket::onm_timer_callback);
    m_timer->start(60000);  //60秒处理一次
}

void emu_socket::onm_timer_callback()
{
    QDateTime current_date = QDateTime::currentDateTime();
    //m_timer_count ++;
    //如果离线时间大于设置值
    if((m_offline_date.secsTo(current_date) > m_offline_limit) && m_offline_switch == true)
    {
        onm_close();
    }
}

void emu_socket::onm_stop_timer()
{
    if(m_timer != nullptr)
    {
        m_timer->stop();
        delete m_timer;

        m_timer = nullptr;
    }
}

void emu_socket::onm_close()
{
    onm_stop_timer();

    if(!m_close_req)
    {
        m_close_req = true;
        emit s_close(this);  //向上层报告socket断开
    }
}


