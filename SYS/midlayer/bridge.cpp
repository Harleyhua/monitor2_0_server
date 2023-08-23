#include "bridge.h"


// 静态成员初始化放到类外部处理
bridge* bridge::m_ins = new bridge;

void bridge::start()
{
    m_emu_server_thd->start();
    m_client_server_thd->start();
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
