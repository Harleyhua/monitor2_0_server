#ifndef AGING_SOCKET_H
#define AGING_SOCKET_H
#include <QTcpSocket>
#include <QTimer>

#define CS_HEAD_FORMAT_LENGTH 27
#define CS_DEFAULT_HEARD        "HEAD55AA"
#define CS_MI_HISTORY_RESPONSE  "2002"
#define CS_JSON_FORMAET_ERROR "2103"   //JSON格式错误
#define CS_JSON_LENGTH_ERROR "2104"   //JSON长度错误
#define CS_RACK_DATA_REQUEST    "1006"
#define CS_RACK_DATA_RESPONSE   "2006"
#define CS_ERR_RESPONSE         "9000"
#define CS_HEAD_ERROR         "2101"   //固定头错误
#define CS_TEMP_KEY         "room_temp_cur"   //温度字段名

class aging_socket: public QTcpSocket
{
    Q_OBJECT
public:
    explicit aging_socket(QObject *parent = nullptr);
    ~aging_socket();

    void onm_del_data();
    bool del_buff_data();
    void deal_cmd_CS(QByteArray &data,QString cmd, bool writeTEMP = false);  //处理客户端通讯
    bool get_root_jsonobj(QByteArray &data,QJsonObject &root_obj,uint32_t start_index = 27);
    void send_cs_msg(QJsonObject &obj,QString cmd,QString status);
    QString get_response_cmd(QString request);
    QByteArray m_buff;

    QTimer mtimer;
    uint32_t mTimerCount = 0;
public slots:
    void onm_timer_callback();

};

#endif // AGING_SOCKET_H
