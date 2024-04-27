#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QObject>
#include <QList>
#include <QTcpServer>
#include <QThread>
#include <QHash>
#include <QMutex>
#include "tcp_socket.h"
//#include "communication/data_analysis/netdata_analysis.h"


class tcp_server : public QTcpServer
{
    Q_OBJECT
public:
    explicit tcp_server(QObject *parent = nullptr);
    ~tcp_server();
    QMutex m_list_mutex;
    QList<QThread *> m_socket_thread_list;  //socket线程
    QList<tcp_socket *> m_socket_list;    //连接的socket列表
    void open_server();
    void close_server();
    void close_socket(tcp_socket *socket);
    //QList<netdata_analysis*> m_analysis_list; //解析列表 主要是同步线程槽函数
private:
    bool m_status = false;      //服务器状态
    QString m_ip = "127.0.0.1";
    QString m_port = "38700";

    void incomingConnection(qintptr socketDescriptor);
private:


//signals :
//    void s_disconnect();
//    void s_socket_close(QString ip);
//public slots:
//    void onm_close_socket();
};

#endif // TCP_SERVER_H
