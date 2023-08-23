#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QApplication>

#include "client_server.h"
#include "emu_server.h"
#include "aging_server.h"
class bridge : public QObject
{
    Q_OBJECT
public:

    bridge(const bridge& obj) = delete;
    bridge& operator = (const bridge &obj) = delete;
    ~bridge(){
        m_emu_server_thd->quit();
        m_emu_server_thd->exit();
        m_client_server_thd->quit();
        m_client_server_thd->exit();

        m_aging_server_thd->quit();
        m_aging_server_thd->exit();


        delete m_emu_server_thd;
        delete m_client_server_thd;
        delete m_client_server;
        delete m_emu_server;

        delete m_aging_server;
        delete m_aging_server_thd;
    };

    void start();
    static bridge *ins();
    client_server *c_server();
    emu_server *e_server();
private:
    bridge(QObject *parent = nullptr): QObject{parent}
    {
        m_client_server = new client_server();
        m_client_server_thd = new QThread;

        m_emu_server = new emu_server();
        m_emu_server_thd = new QThread;

        m_aging_server = new aging_server();
        m_aging_server_thd = new QThread;

        //服务  脱离GUI
        m_emu_server->moveToThread(m_emu_server_thd);
        m_client_server->moveToThread(m_client_server_thd);
        m_aging_server->moveToThread(m_aging_server_thd);


        //异线程 服务启动
        connect(m_emu_server_thd,&QThread::started,m_emu_server,&emu_server::onm_start);
        connect(m_client_server_thd,&QThread::started,m_client_server,&client_server::onm_start);
        connect(m_aging_server_thd,&QThread::started,m_aging_server,&aging_server::onm_start);
    };

    static bridge* m_ins;

    client_server * m_client_server = nullptr;
    emu_server *m_emu_server = nullptr;
    QThread *m_emu_server_thd = nullptr;
    QThread *m_client_server_thd = nullptr;

    aging_server *m_aging_server = nullptr;
    QThread *m_aging_server_thd = nullptr;

signals:
};

#endif // BRIDGE_H
