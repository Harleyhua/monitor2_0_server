#include "client_server.h"



extern QString g_http_server_ini_path;

#define LOG qDebug()<<__FILE__<<__LINE__<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")


client_server::client_server(QObject *parent)
  : QObject{parent}
{


}

client_server::~client_server()
{
    delete m_listener;
    delete m_listener_setting;

    delete m_logging;
    delete m_logging_setting;

    delete m_client;
}

//bool client_server::open()
//{
//    QHostAddress tmp_ip(m_ip);
//    //监听  无法开启
//    if(listen(tmp_ip,m_port.toUInt()))
//    {
//        return true;
//    }

//    return false;
//}

//bool client_server::open(QString ip, QString port)
//{
//    bool ret = false;
//    m_ip = ip;
//    m_port = port;
//    ret = open();

//    return ret;
//}

//void client_server::close()
//{

//}

void client_server::onm_start()
{
    if(m_runing)
    {
        return;
    }

    m_listener_setting = new QSettings(g_http_server_ini_path,QSettings::IniFormat);
    m_logging_setting = new QSettings(g_http_server_ini_path,QSettings::IniFormat);

    //if(m_listener_setting != nullptr)
    {
        //m_logging_setting->sync();
        m_logging_setting->beginGroup("logging");
//        qDebug() << m_logging_setting->allKeys();
//        qDebug() << m_logging_setting->value("fileName").toString();
        m_logging = new FileLogger(m_logging_setting,10000);
        m_logging->installMsgHandler();

        m_listener_setting->beginGroup("listener");
//        m_listener_setting->setValue("host","");
//        m_listener_setting->setValue("port",m_port);
//        m_listener_setting->setValue("minThreads",m_minthreads);
//        m_listener_setting->setValue("maxThreads",m_maxthreads);
//        m_listener_setting->setValue("cleanupInterval",m_clearninterval);
//        m_listener_setting->setValue("readTimeout",m_readtimeout);
//        m_listener_setting->setValue("maxRequestSize",m_maxrequestsize);
//        m_listener_setting->setValue("maxMultiPartSize",m_maxmultipartsize);
        m_listener = new HttpListener(m_listener_setting,new RequestMapper);
        //m_listener = new HttpListener(m_listener_setting,new cs_client);
        m_runing = true;
    }
}
