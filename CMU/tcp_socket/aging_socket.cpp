#include "aging_socket.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "mysql.h"
#include "common.h"
#include "QsLog.h"

aging_socket::aging_socket(QObject *parent)
{

    //定时任务
    connect(&mtimer,&QTimer::timeout,this,&aging_socket::onm_timer_callback);
    mtimer.start(5000);
}

aging_socket::~aging_socket()
{

}

void aging_socket::onm_del_data()
{
    //common com;
    //按字节流加入
    mTimerCount = 0;
    m_buff.append(readAll());
    qDebug() << m_buff.size();
    //common::writeFile("d:/1.txt",m_buff);
    while (m_buff.size() > 0) {
        //数据异常则会清空  若未接收完全则跳出继续接收
        //可能会同时收到多条指令 处理完后继续处理下一条
        if(!del_buff_data())
        {
            break;
        }
    }
}



bool aging_socket::del_buff_data()
{
    //获取缓存区当前数据长度
    uint32_t data_size = m_buff.size();
    qDebug() << data_size;
    if(data_size < CS_HEAD_FORMAT_LENGTH)
    {
        //QLOG_WARN() << tr("CS通讯:数据还未收全 继续接收");
        return false;
    }
    QString str_netdata = m_buff.mid(0,CS_HEAD_FORMAT_LENGTH);
    //与客户端的协议解析 以HEAD55AA开头
    if(str_netdata.startsWith(CS_DEFAULT_HEARD))
    {
        bool ok = false;
        uint32_t length = str_netdata.mid(16,11).toUInt(&ok);
        QString cmd;
        if(!ok)
        {
            //QLOG_WARN() << tr("CS通讯:数据长度字符异常(非数字类型)");
            m_buff.clear();
            QJsonObject send_js;
            send_cs_msg(send_js,CS_MI_HISTORY_RESPONSE,CS_JSON_FORMAET_ERROR);
            return false;
        }
        //数据长度异常
        if(data_size < length + CS_HEAD_FORMAT_LENGTH)
        {
            //QLOG_WARN() << tr("CS通讯: 数据还未收全 继续接收");
            //m_buff.clear();
            //QJsonObject send_js;
            //send_cs_msg(send_js,CS_ERR_RESPONSE,CS_JSON_LENGTH_ERROR);
            return false;
        }
        cmd = str_netdata.mid(8,4);
        deal_cmd_CS(m_buff,cmd);

        m_buff.remove(0,length + CS_HEAD_FORMAT_LENGTH);//处理过的数据抛出
        //m_buff.remove(0,data_size);//处理过的数据抛出

        return true;
    }
    else {
        m_buff.clear();

        QJsonObject send_js;
        send_cs_msg(send_js,CS_ERR_RESPONSE,CS_HEAD_ERROR);
        //QLOG_WARN() << "CS通讯: buffer异常 清空" ;
        return false;
    }
}

void aging_socket::deal_cmd_CS(QByteArray &data, QString cmd)
{
    QJsonObject root_js_recv;
    QJsonObject root_js_send;
    mysql sql(common::rand_str());
//    if(cmd == CS_RACK_HANDSHAKE_REQUEST)
//    {
//        QByteArray msg("HEAD55AA2012210000000000000");
//        emit s_send_msg(msg);
//        return;
//    }

    if(get_root_jsonobj(data,root_js_recv))
    {
        //跟老化架上位机的通讯
        if(cmd == CS_RACK_DATA_REQUEST)
        {
            sql.w_rack_data(root_js_recv);
            if(root_js_recv.value(CS_TEMP_KEY).toDouble(2) > 0 ){
                // QJsonValue room = root_js_recv.value("room_id");
                // QString room_id = room.toString();

                // if(room_id != "room-1")
                // {
                    //识别到有温度上传，直接写入温度
                    ag_temp_table tempTable;
                    QSqlDatabase q1 = sql.getQSqlDatabaseFromMysql();
                    QJsonObject t1;
                    t1.insert("room_id",root_js_recv.value("room_id"));
                    t1.insert("run_status",root_js_recv.value("run_status").toInt(0));
                    t1.insert("cur_temp",(int)(root_js_recv.value("room_temp_cur").toDouble(2)));
                    t1.insert("set_temp",(int)(root_js_recv.value("room_temp_set").toDouble(2)));

                    //修正时间
                    QString min = QDateTime::currentDateTime().toString("mm");
                    uint16_t min_int = (min.toUInt() / 5) *5;
                    QString curr_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:") +
                                       QString("%1").arg(min_int,2,10,QLatin1Char('0')) + ":00";

                    t1.insert("cur_time",curr_time);
                    //t1.insert("cur_time",root_js_recv.value("curr_time"));

                    tempTable.write_temp(q1,t1,true);

                    double temp1 = root_js_recv.value(CS_TEMP_KEY).toDouble(2);
                    qDebug() << temp1;
                //}

            }else{

                qDebug() << 0;
            }
            QByteArray msg("HEAD55AA2006210000000000000");
            this->write(msg);
        }

        // //温度上传
        // if(cmd == CS_TEMP_DATA_REQUEST)
        // {
        //     //sql.w_rack_data(root_js_recv);
        //     if(root_js_recv.value(CS_TEMP_KEY).toDouble(2) > 0 ){
        //         //识别到有温度上传，直接写入温度
        //         ag_temp_table tempTable;
        //         QSqlDatabase q1 = sql.getQSqlDatabaseFromMysql();
        //         QJsonObject t1;
        //         t1.insert("room_id",root_js_recv.value("room_id"));
        //         t1.insert("run_status",root_js_recv.value("run_status").toInt(0));
        //         t1.insert("cur_temp",(int)(root_js_recv.value("room_temp_cur").toDouble(2)));
        //         t1.insert("set_temp",(int)(root_js_recv.value("room_temp_set").toDouble(2)));

        //         //修正时间
        //         QString min = QDateTime::currentDateTime().toString("mm");
        //         uint16_t min_int = (min.toUInt() / 5) *5;
        //         QString curr_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:") +
        //                             QString("%1").arg(min_int,2,10,QLatin1Char('0')) + ":00";

        //         t1.insert("cur_time",curr_time);
        //         //t1.insert("cur_time",root_js_recv.value("curr_time"));

        //         tempTable.write_temp(q1,t1,true);

        //         double temp1 = root_js_recv.value(CS_TEMP_KEY).toDouble(2);
        //         qDebug() << temp1;
        //     }
        //     else
        //     {

        //         qDebug() << 0;
        //     }
        //     QByteArray msg("HEAD55AA2007210000000000000");
        //     this->write(msg);
        // }
    }
    else
    {
        send_cs_msg(root_js_send,get_response_cmd(cmd),CS_JSON_FORMAET_ERROR);
        //QLOG_WARN() << QString("指令:%1 ,json数据格式错误").arg(get_response_cmd(cmd));
    }
}

bool aging_socket::get_root_jsonobj(QByteArray &data, QJsonObject &root_obj, uint32_t start_index)
{
    QJsonParseError err_pt;
    bool ok = false;
    int json_data_length = data.mid(16,11).toUInt(&ok);

    // if(json_data_length + start_index < data.size()){

    //    json_data_length = data.size()-start_index;
    // }

    QJsonDocument root_doc = QJsonDocument::fromJson(data.mid(start_index,json_data_length),&err_pt);
    if(err_pt.error != QJsonParseError::NoError)
    {
        return false;
    }
    else {
        root_obj = root_doc.object();
    }
    return true;
}

void aging_socket::send_cs_msg(QJsonObject &obj, QString cmd, QString status)
{
    int js_length;
    QByteArray msg;
    QJsonDocument root_doc;
    root_doc.setObject(obj);
    msg.append(root_doc.toJson(QJsonDocument::Compact));

    js_length = msg.size();
    msg.insert(0,CS_DEFAULT_HEARD);
    msg.insert(8,cmd.toUtf8());
    msg.insert(12,status.toUtf8());
    msg.insert(16,QString("%1").arg(js_length,11,10,QLatin1Char('0')).toUtf8());

    this->write(msg);
}

QString aging_socket::get_response_cmd(QString request)
{
    return request.replace(0,1,QString::number(request.mid(0,1).toUInt()+1));
}

void aging_socket::onm_timer_callback()
{
    mTimerCount ++;
    if(mTimerCount >= 5)
    {
        mTimerCount = 0;
        m_buff.clear();
    }
}
