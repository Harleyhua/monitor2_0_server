#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H

#include <QObject>
//#include <QTcpServer>
#include <QSettings>
#include <QApplication>

#include "httplistener.h"
#include "filelogger.h"

#include "cs_client.h"
#include "requestmapper.h"

using namespace stefanfrings;

class client_server : public QObject
{
    Q_OBJECT
public:
    explicit client_server(QObject *parent = nullptr);
    ~client_server();
    bool open();
    bool open(QString ip,QString port);
    void close();

public slots:
    void onm_start();
private:
    //QString m_ip = "192.168.27.245";   //服务器监听端口
    QString m_ip;
    quint16 m_port = 38700;
    int m_minthreads = 4;                //空闲最小线程数
    int m_maxthreads = 100;              //最大线程数
    int m_clearninterval = 60000;        //空线程最大清空间隔 ms
    int m_readtimeout = 60000;           //连接超时时间      ms
    int m_maxrequestsize = 10000;         //
    int m_maxmultipartsize = 10000000;   //上传的最大数   byte

    bool m_runing = false;

    HttpListener *m_listener = nullptr;
    QSettings *m_listener_setting = nullptr;

    FileLogger *m_logging = nullptr;
    QSettings *m_logging_setting = nullptr;

    cs_client *m_client = nullptr;
signals:

};

#endif // CLIENT_SERVER_H
