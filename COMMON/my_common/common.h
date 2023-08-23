#ifndef COMMON_H
#define COMMON_H

#include <QObject>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class common : public QObject
{
    Q_OBJECT
public:
    explicit common(QObject *parent = nullptr);

    static bool is_ip_valid(QString ip);
    static bool is_port_valid(QString port);
    static bool is_port_valid(uint32_t port);
    static void qstring_to_chars(QString str);
    static QString Get_LocalIp();//获取本地IP
    static uint8_t qbtarray_to_u8(const QByteArray &ay,uint16_t index);
    static uint16_t qbtarray_to_u16(const QByteArray &ay,uint16_t index);
    static uint32_t qbtarray_to_u32(const QByteArray &ay,uint16_t index);
    static QByteArray u32_to_qbytearray(uint32_t i);
    static QByteArray u16_to_qbytearray(uint16_t i);

    static void qbtarray_to_string(const QByteArray &ay,QString &str);
    //static QByteArray u8_to_qbytearray(uint8_t i);

    static qint64 cal_time_interval(QString start_time);
    //static QByteArray s32_to_qbyteattay(int i);
    static QString get_system_detail_time();
    static uint16_t get_system_yearmon_time();
    static QString get_system_detail_offset_time(QString date_offset_type,int64_t offset);  //
    static void qbytearray_set_value(QByteArray &data,uint32_t start,uint32_t size,uint8_t value =0);

    static QString qbtarray_to_str(QByteArray &array);
    static QByteArray str_to_qbtarray(QString str);
    static QByteArray str_to_qbtarray(QString &str);

    static QByteArray uni_to_gbk(QString str);
    static QString gbk_to_uni(QByteArray str);

    static QJsonObject qbytearray_to_js(QByteArray data);
    static QByteArray js_to_qbytearray(QJsonObject root_obj);


    static QString rand_str();


    //QString str = QString::fromUtf8(tmp_array.toBase64());
    //QByteArray retrievedData = QByteArray::fromBase64(str.toUtf8());
signals:

};

#endif // COMMON_H
