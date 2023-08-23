#include "emu_server.h"

#include "common.h"

emu_server::emu_server(QObject *parent)
    : QTcpServer(parent)
{
    m_ip = common::Get_LocalIp();
}

void emu_server::close()
{
    //关闭服务器监听
    //QTcpServer::close();
}

void emu_server::incomingConnection(qintptr socketDescriptor)
{
    QThread *tmp_thd = new QThread();
    emu_socket *tmp_socket = new emu_socket();

    if((tmp_thd !=nullptr) && (tmp_socket !=nullptr))
    {
        //创建未知线程
        m_sockets_threads.insert(tmp_socket,tmp_thd);

        tmp_socket->setSocketDescriptor(socketDescriptor);

        tmp_socket->moveToThread(tmp_thd);   //socket线程

        connect(tmp_socket,&emu_socket::s_new_emu,this,&emu_server::onm_new_emu);   //确认连接身份

        connect(tmp_socket,&emu_socket::s_close,this,&emu_server::onm_close_socket);//socket请求关闭

        connect(tmp_thd,&QThread::started,tmp_socket,&emu_socket::onm_start_timer);//socket在线程内部初始化

        //socket 主动断开处理
        connect(tmp_socket,&emu_socket::disconnected,tmp_socket,&emu_socket::onm_close);//socket连接主动或异常断开

        tmp_thd->start();
    }
    else
    {
        //对nullptr进行  delete  也是安全的
        delete tmp_thd;
        delete tmp_socket;
    }
}

void emu_server::onm_new_emu(QString name, emu_socket *socket, abstract_emu *emu_ptr)
{
    //如果当前网关编号已经存在 顶替掉
    if(m_emus.value(name,nullptr) != nullptr)
    {
        onm_close_emu(name);
    }

    //插入新网关
    if(emu_ptr != nullptr)
    {
        m_emus.insert(name,emu_ptr);

        m_sockets.insert(name,socket);

        m_threads.insert(name,m_sockets_threads.value(socket));

        m_sockets_threads.remove(socket);

        disconnect(socket,&emu_socket::s_close,this,&emu_server::onm_close_socket);

        connect(emu_ptr,&abstract_emu::s_close,this,&emu_server::onm_close_emu);
    }
}

void emu_server::onm_close_socket(emu_socket *ptr)
{
    QThread *cur_thd = m_sockets_threads.value(ptr,nullptr);

    if(cur_thd != nullptr)
    {
        m_sockets_threads.remove(ptr);

        cur_thd->quit();
        cur_thd->wait();

        delete cur_thd;
    }

    if(ptr != nullptr)
    {
        delete ptr;
    }
}

void emu_server::onm_close_emu(QString name)
{
    QThread *cur_thd = m_threads.value(name,nullptr);
    if(cur_thd != nullptr)
    {
        cur_thd->quit();
        cur_thd->wait();
    }

    delete m_emus.value(name,nullptr);
    delete m_sockets.value(name,nullptr);
    delete m_threads.value(name,nullptr);

    m_emus.remove(name);
    m_sockets.remove(name);
    m_threads.remove(name);
}

void emu_server::onm_start()
{
    QHostAddress tmp_ip(m_ip);
    //监听  无法开启
    if(listen(tmp_ip,m_port.toUInt()))
    {

    }
}






