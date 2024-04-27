#ifndef EMU_SERVER_H
#define EMU_SERVER_H

#include <QObject>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>

#include "emu_socket.h"
#include "abstract_bym.h"
#include "abstract_emu.h"
class emu_server :public QTcpServer
{
    Q_OBJECT
public:
    explicit emu_server(QObject *parent = nullptr);
    ~emu_server()
    {
        for (QHash<QString,abstract_emu *>::const_iterator it = m_emus.cbegin(), end = m_emus.cend(); it != end; ++it)
        {
            if(it.value() != nullptr)
            {
                delete it.value();
            }
        }
        for (QHash<QString,emu_socket *>::const_iterator it = m_sockets.cbegin(), end = m_sockets.cend(); it != end; ++it)
        {
            if(it.value() != nullptr)
            {
                delete it.value();
            }
        }
        for (QHash<QString,QThread *>::const_iterator it = m_threads.cbegin(), end = m_threads.cend(); it != end; ++it)
        {
            if(it.value() != nullptr)
            {
                delete it.value();
            }
        }
        for (QHash<emu_socket *,QThread *>::const_iterator it = m_sockets_threads.cbegin(), end = m_sockets_threads.cend(); it != end; ++it)
        {
            if(it.value() != nullptr)
            {
                delete it.value();
            }
            if(it.key() != nullptr)
            {
                delete it.key();
            }
        }
    };


    void close();

private:
    QString m_ip = "127.0.0.1";
    QString m_port = "40031";

    QHash<QString,abstract_emu *> m_emus;
    QHash<QString,emu_socket *> m_sockets;
    QHash<QString,QThread *>m_threads;

    QHash<emu_socket *,QThread *> m_sockets_threads;

    void incomingConnection(qintptr socketDescriptor);
signals:
    void s_socket_init();  //初始化工作  （定时器）
private slots:
    void onm_new_emu(QString name,emu_socket *socket,abstract_emu *emu_ptr);

    void onm_close_socket(emu_socket *ptr);//关闭socket

    void onm_close_emu(QString emu_name);  //关闭网关

public slots:
    void onm_start(); //服务器启动
};

#endif // EMU_SERVER_H
