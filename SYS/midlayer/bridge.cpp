#include "bridge.h"


// 静态成员初始化放到类外部处理
bridge* bridge::m_ins = new bridge;

void bridge::start()
{
    //网关 Port:40031
    m_emu_server_thd->start();

    //客户端、温度采集 Port:38700
    m_client_server_thd->start();

    //老化房 Port:40032
    m_aging_server_thd->start();
}

bridge *bridge::ins()
{
    return m_ins;
}

client_server *bridge::c_server()
{
    return m_client_server;
}

emu_server *bridge::e_server()
{

    return m_emu_server;
}
