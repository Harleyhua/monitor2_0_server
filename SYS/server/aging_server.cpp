#include "aging_server.h"


#include "common.h"
#include "QsLog.h"



aging_server::aging_server(QObject *parent)
    : QTcpServer(parent)
{
    m_ip = common::Get_LocalIp();

    // 初始化定时器
    checkListeningTimer = new QTimer(this);
    connect(checkListeningTimer, &QTimer::timeout, this, &aging_server::checkServerListening);
    checkListeningTimer->start(5000); // 每5秒检查一次监听状态
}


void aging_server::checkServerListening() {
    if (!isListening()) {
        qDebug() << "服务器未在监听，尝试重新监听...";
        if (!listen(QHostAddress(m_ip), m_port.toUInt())) {
            qDebug() << "服务器重新监听失败:" << this->errorString();
        } else {
            qDebug() << "服务器重新监听成功";
        }
    }
}


void aging_server::onm_close_socket()
{
    aging_socket *tmp_socket = static_cast<aging_socket*>(sender());
    QString tmp_ip = tmp_socket->peerAddress().toString();

    if(m_thd_list.size() != m_socket_list.size())
    {
        for(int i=0;i<m_thd_list.size();i++)
        {
            m_thd_list[i]->quit();
            m_thd_list[i]->wait();//等待退出
            delete m_thd_list[i];
        }

        for(int i=0;i<m_socket_list.size();i++)
        {
            delete m_socket_list[i];
        }

        m_thd_list.clear();
        m_socket_list.clear();
    }

    for(int i=0;i<m_socket_list.size();i++)
    {
        if(tmp_socket == m_socket_list[i])
        {
            //断开连接
            tmp_socket->disconnect();

            m_thd_list[i]->quit();
            m_thd_list[i]->wait();//等待退出

            delete m_socket_list[i];
            delete m_thd_list[i];

            m_thd_list.removeAt(i);
            m_socket_list.removeAt(i);


            QLOG_INFO() << QString("断开一个socket 当前连接数: %1").arg(m_socket_list.size());
            break;
        }
    }

    //emit s_socket_close(tmp_ip);
}

void aging_server::onm_start()
{
    QHostAddress tmp_ip(m_ip);
    //监听  无法开启
    // if(listen(tmp_ip,m_port.toUInt()))
    // {

    // }
    if (!listen(tmp_ip, m_port.toUInt())) {
        QLOG_INFO() << "监听失败:" << this->errorString();
    } else {
        QLOG_INFO() << "服务器已成功监听";
        //checkListeningTimer->stop();
    }
}

void aging_server::incomingConnection(qintptr socketDescriptor)
{
    QThread *tmp_thd = new QThread();
    aging_socket *tmp_socket = new aging_socket();

    m_thd_list.append(tmp_thd);
    m_socket_list.append(tmp_socket);

    QLOG_INFO() << QString("一个socket连接 当前连接数: %1").arg(m_socket_list.size());

    tmp_socket->setSocketDescriptor(socketDescriptor);

    tmp_socket->moveToThread(tmp_thd);

    connect(tmp_socket,&aging_socket::readyRead,tmp_socket,&aging_socket::onm_del_data);
    connect(tmp_socket,&aging_socket::disconnected,this,&aging_server::onm_close_socket);

    tmp_thd->start();
}
