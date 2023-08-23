#ifndef AGING_SERVER_H
#define AGING_SERVER_H
#include <QThread>
#include <QTcpServer>
#include "aging_socket.h"
class aging_server:public QTcpServer
{
    Q_OBJECT
public:
    explicit aging_server(QObject *parent = nullptr);
    ~aging_server()
    {

    };

    QList<QThread *> m_thd_list;
    QList<aging_socket *> m_socket_list;

public slots:
    void onm_close_socket();

    void onm_start(); //服务器启动
private :
    QString m_ip = "127.0.0.1";
    QString m_port = "40032";




    void incomingConnection(qintptr socketDescriptor);
};

#endif // AGING_SERVER_H
