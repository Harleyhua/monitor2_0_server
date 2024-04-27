#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <QObject>

class circular_buffer : public QObject
{
    Q_OBJECT
public:
    explicit circular_buffer(QObject *parent = nullptr);
    QByteArray m_buffer;
    uint32_t m_buffer_head;
    uint32_t m_buffer_tail;
    uint32_t m_buffer_length;
    uint32_t size();
    void set_buffer_size(uint32_t size);
    void write_buffer(QByteArray data);
    void read_buffer(QByteArray &data,uint32_t size = 1);
signals:

};

#endif // CIRCULAR_BUFFER_H
