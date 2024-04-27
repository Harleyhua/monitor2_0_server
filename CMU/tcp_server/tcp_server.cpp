#include "tcp_server.h"
//#include "open_lib/QsLog/include/QsLog.h"
//#include "common.h"
//注册元属性
Q_DECLARE_METATYPE(tcp_socket*)

tcp_server::tcp_server(QObject *parent)
    : QTcpServer(parent)
{


}

tcp_server::~tcp_server()
{
    //
    this->close();
    for(int i=0;i<m_socket_list.size();i++)
    {
        m_socket_list[i]->disconnect();

        m_socket_thread_list[i]->quit();
        m_socket_thread_list[i]->wait();
        delete m_socket_list[i];
        delete m_socket_thread_list[i];
        //delete m_analysis_list[i];
    }
}

void tcp_server::open_server()
{
    m_status = true;
}

void tcp_server::close_server()
{
    m_status = false;
}

void tcp_server::close_socket(tcp_socket *socket)
{
    for(int i=0;i<m_socket_list.size();i++)
    {
        if(socket == m_socket_list[i])
        {
            //断开连接
            socket->disconnect();

            m_socket_thread_list[i]->quit();
            m_socket_thread_list[i]->wait();

            delete m_socket_list[i];
            delete m_socket_thread_list[i];

            m_socket_thread_list.removeAt(i);
            m_socket_list.removeAt(i);
            //m_analysis_list.removeAt(i);
            //QLOG_INFO() << QString("断开一个socket 当前连接数: %1").arg(m_socket_list.size());
            break;
        }
    }
}


void tcp_server::incomingConnection(qintptr socketDescriptor)
{
    QThread* tmp_socket_thread = new QThread();
    tcp_socket* tmp_socket = new tcp_socket();

    m_list_mutex.lock();

    m_socket_thread_list.append(tmp_socket_thread);
    m_socket_list.append(tmp_socket);
    //初始化socket
    tmp_socket->setSocketDescriptor(socketDescriptor);
    tmp_socket->moveToThread(tmp_socket_thread);   //数据接收线程

    tmp_socket_thread->start();
    m_list_mutex.unlock();


    //QString tmp_ip = tmp_socket->peerAddress().toString();
    //netdata_analysis *tmp_analysis = new netdata_analysis(tmp_ip);  //构造时传入ip  参数
    //m_analysis_list.append(tmp_analysis);

    //移至新线程中
    //tmp_analysis->moveToThread(tmp_send_thread); // 数据发送线程
    //tmp_analysis->move_member_to_thread(tmp_send_thread);


    //连接信号槽
    //tcp缓冲区收到数据
    connect(tmp_socket,&tcp_socket::readyRead,tmp_socket,&tcp_socket::onm_new_msg);
    //connect(tmp_socket,&tcp_socket::s_new_msg,tmp_analysis,&netdata_analysis::onm_new_msg);

    // -> 发送
    //connect(tmp_analysis,&netdata_analysis::s_send_msg,tmp_socket,&tcp_socket::onm_send_msg);
    //connect(tmp_analysis,&netdata_analysis::s_send_hand_time,tmp_socket,&tcp_socket::onm_update_hand);
    //connect(tmp_socket,&)
    //socket 断开处理
    //connect(tmp_socket,&tcp_socket::disconnected,this,&tcp_server::onm_close_socket);
    //QLOG_INFO() << "新socket连接 当前socket数:" + QString("%1").arg(m_socket_list.size());
}

//void tcp_server::onm_close_socket()
//{
//    tcp_socket *tmp_socket = static_cast<tcp_socket*>(sender());
//    QString tmp_ip = tmp_socket->peerAddress().toString();
//    close_socket(tmp_socket);

//    emit s_socket_close(tmp_ip);
//}
