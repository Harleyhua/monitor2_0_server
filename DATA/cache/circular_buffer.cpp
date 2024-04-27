#include "circular_buffer.h"
#include "common.h"
#include "open_lib/QsLog/include/QsLog.h"
circular_buffer::circular_buffer(QObject *parent)
    : QObject{parent}
{
    m_buffer_length = 2001;
    m_buffer.resize(m_buffer_length);
    m_buffer_head = 0;
    m_buffer_tail = 0;
}

uint32_t circular_buffer::size()
{
    int tmp_ret= 0;
    if(m_buffer_tail >= m_buffer_head)
        tmp_ret = m_buffer_tail - m_buffer_head;
    else if(m_buffer_tail < m_buffer_head)
        tmp_ret = m_buffer_tail - m_buffer_head + m_buffer.size();

    return tmp_ret;
}

void circular_buffer::set_buffer_size(uint32_t size)
{
    m_buffer.resize(size);
}

void circular_buffer::write_buffer(QByteArray data)
{
    //写入数据
    if(data.size() <= m_buffer_length -1 - m_buffer.size())
    {


    }
    else if(data.size() > m_buffer_length -1 - m_buffer.size())
    {
        //写入数据超过缓冲区剩余长度 报错

        QLOG_INFO() << "circular buffer over";
    }
}






