#include "agingservice.h"

#include "abstract_bym.h"
#include "aging_alg.h"
#include "mysql.h"
#include "common.h"
#define SAVE_MI_REPORT 1

agingservice::agingservice(QSqlDatabase &DataBase, QObject *parent)
    : QObject{parent}
{
    mDataBase = DataBase;
}

void agingservice::readAgingAlgorithm(QString user, QJsonObject &retData)
{
    ag_user_table userTable;
    userTable.read_algorithm(mDataBase,user,retData);
}

void agingservice::readRoomTemperature(const QJsonObject &s, QJsonObject &retData)
{
    ag_temp_table tempTable;

    tempTable.read_temp(mDataBase,s,retData);
}

void agingservice::writeRoomTemperature(const QJsonObject &s)
{
    ag_temp_table tempTable;
    tempTable.write_temp(mDataBase,s);
}

void agingservice::writeNewRoomTemperature(QString room, bool runstatus, int curtemp, int settemp, QString curtime)
{
    ag_temp_table tempTable;
    tempTable.write_temp(mDataBase,room,runstatus,curtemp,settemp,curtime);
}

void agingservice::readBatchList(const QJsonObject &s, QJsonObject &retData)
{
    ag_rack_data_table rackDataTable;
    QStringList startTimeList;
    QJsonArray batchArray;
    rackDataTable.read_batch_list(mDataBase,s.value("start_time").toString(),
                                  s.value("stop_time").toString(),startTimeList);
    for(int i=0;i<startTimeList.size();i++)
    {
        QStringList mis;
        QJsonObject oneBatchJsObj;
        QJsonArray misArray;

        rackDataTable.read_mi_list(mDataBase,startTimeList[i],mis);

        for(int j=0;j<mis.size();j++)
        {
            if(abstract_bym::is_cid_valid(mis[j]))
            {
                misArray.append(mis[j]);
            }
        }
        oneBatchJsObj.insert("start_time",startTimeList[i]);
        oneBatchJsObj.insert("mis",misArray);
        batchArray.append(oneBatchJsObj);
    }
    retData.insert("batchs",batchArray);
}

void agingservice::writeRackData(const QJsonObject &s)
{
//    ag_rack_data_table rackDataTable;
//    //QSqlDatabase &m_database,QJsonObject &w_data

//    rackDataTable.write_data(mDataBase,s);

    QString table_name;
    uint16_t date = common::get_system_yearmon_time();
    QJsonArray rack_datas = s.value("datas").toArray();

    for(int i=0;i<rack_datas.size();i++)
    {
        ag_rack_index_table tmp_rk_idx_tb;
        QJsonArray table_list;
        QJsonObject idx_r_data;
        QJsonObject idx_datas;
        QJsonObject rt_data;
        idx_datas.insert("mi_cid",rack_datas[i].toObject().value("mi_cid").toString());
        idx_datas.insert("date",date);
        idx_r_data.insert("params",idx_datas);

        tmp_rk_idx_tb.read_rack_index(mDataBase,idx_r_data,rt_data);

        table_list = rt_data.value("datas").toObject().value("datatable_name").toArray();

        if(table_list.size() >0)
        {
            table_name = table_list[0].toString();
        }
        else
        {
            table_name = "rack_data_" + QString::number(date) + "_1";
            //读写的json  比较接近 直接拿来用了
            idx_r_data.insert("datatable_name",table_name);
            QJsonObject w_data;
            QJsonObject w_datas;
            w_datas.insert("mi_cid",rack_datas[i].toObject().value("mi_cid").toString());
            w_datas.insert("date",date);
            w_datas.insert("datatable_name",table_name);
            w_data.insert("datas",w_datas);
            //第一次写入时
            tmp_rk_idx_tb.write_rack_index(mDataBase,w_data);
        }
    }
    ag_rack_data_table tmp_rk_data_tb(table_name);
    ag_rack_extra_data_table tmp_rk_extra_tb;
    //写发电数据
    tmp_rk_data_tb.write_data(mDataBase,s);
    tmp_rk_extra_tb.write_data(mDataBase,s);
}

void agingservice::readRackData(const QJsonObject &s, QJsonObject &retData)
{
    ag_rack_data_table rackDataTable;

    rackDataTable.read_data(mDataBase,s,retData);
}

void agingservice::readOrderData(const QJsonObject &s, QJsonObject &retData)
{
    ag_workorder_table workOrderTable;
    workOrderTable.read_workorder(mDataBase,s,retData);
}

void agingservice::writeOrderData(const QJsonObject &s)
{
    ag_workorder_table workOrderTable;
    workOrderTable.write_workorder(mDataBase,s);
}

void agingservice::generateMiAgingReport(int mode,QString Mi,QString startTime,const QJsonObject &algorithm, QJsonObject &retReport)
{
    //ag_user_table userTable;
    ag_temp_table tempTable;
    ag_rack_data_table rackDataTable;
    ag_rack_extra_data_table rackExtraDataTable;
    QString room;
    QString stopTime;
    QString posDesc;            //微逆老化时的位置信息
    QString judgeParam;
    QJsonObject agingData;
    QString historyReport;     //当前参数下的历史报告

    aging_alg agingAlg;
    QHash<QString, uint16_t> roomTemp;

    int agingMinute = -1;
    qint64 setStopTime = 0; //预设的停止时间
    qint64 nextStopTime = 0;//实际的下次的停止时间


    if(mode == 0)//按编号解析
    {
        //读取最后一次老化的  参数  用于读取老化数据
        rackDataTable.read_mi_last_aging_time_by_mi(mDataBase,Mi,room,startTime,stopTime,posDesc);
    }
    else if(mode == 1)//按老化批次
    {
        rackDataTable.read_mi_stop_time_after_start_time(mDataBase,Mi,room,startTime,stopTime,posDesc);
    }


    //读取该微逆  当前老化的老化时间
    rackExtraDataTable.read_data(mDataBase,Mi,startTime,agingMinute);
    setStopTime = QDateTime::fromString(startTime,"yyyy-MM-dd hh:mm:ss").toSecsSinceEpoch() + agingMinute *60;
    nextStopTime = QDateTime::fromString(stopTime,"yyyy-MM-dd hh:mm:ss").toSecsSinceEpoch();
    //修正结束时间
    if(agingMinute != -1 && setStopTime <= nextStopTime)
    {
        stopTime = QDateTime::fromSecsSinceEpoch(setStopTime).toString("yyyy-MM-dd hh:mm:ss");
    }
    //userTable.read_algorithm(mDataBase,);
    //读取微逆老化数据
    readMiAgingData(Mi,startTime,stopTime,agingData);
    //只记载某个微逆型号的judge_param
    judgeParam = QString(QJsonDocument(algorithm.value("bym" + QString::number(agingData.value("total_nominal_power").toInt())).toObject()).toJson());

    if(readMiAgingReport(agingData.value("mi_cid").toString(),agingData.value("start_time").toString(),
                         agingData.value("stop_time").toString(),judgeParam,historyReport))
    {
        retReport = QJsonDocument::fromJson(historyReport.toUtf8()).object();
    }
    else
    {
        //读取该微逆的温度数据
        tempTable.read_temp(mDataBase,room,agingData.value("start_time").toString(),agingData.value("stop_time").toString(),roomTemp);
        //进行老化分析
        agingAlg.aging_report(agingData,algorithm.value("bym" + QString::number(agingData.value("total_nominal_power").toInt())).toObject(),
                              roomTemp,retReport);
#if(SAVE_MI_REPORT)
        //已经老化结束的情况下
        if(stopTime < QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
        {
            //将当前生成的报告写入数据库
            writeMiAgingReport(agingData.value("mi_cid").toString(),
                               agingData.value("start_time").toString(),
                               agingData.value("stop_time").toString(),
                               judgeParam,QString(QJsonDocument(retReport).toJson()));

        }
#endif
    }
    //顺带记录 老化房间  回溯老化环境温度
    retReport.insert("room",room);
    retReport.insert("pos_desc",posDesc);
}

void agingservice::readMiAgingData(QString mi, QString startTime, QString stopTime, QJsonObject &rt_data)
{
    //创建一个存储功率数据表的实例
    ag_power_data_table pw_dt_tb;
    //创建一个存储微逆属性表的实例
    ag_mi_property_table mi_pty_tb;

    //创建一个JSON对象，用于存储查询条件
    QJsonObject s_data;

    //创建一个JSON数组，用于存储微逆编号
    QJsonArray mi_cid;

    //将变量mi（微逆编号）添加到数组中
    mi_cid.append(mi);

    //将微逆编号数组作为"mi_cid"字段插入到s_data对象中
    s_data.insert("mi_cid",mi_cid);

    //将开始时间作为"start_date"字段插入到s_data对象中
    s_data.insert("start_date",startTime);

    //将结束时间作为"stop_date"字段插入到s_data对象中
    s_data.insert("stop_date",stopTime);

    // 插入一个字段，可能用于指定查询的数据量或其他用途
    s_data.insert("nums","-1");


    //根据微逆编号分析并获取微逆类型
    bym_type mi_type = abstract_bym::analysis_type(mi);

    //创建一个JSON对象，用于存储从数据库读取的光伏数据
    QJsonObject read_pv_data;
    QJsonArray datas;

    //创建一个哈希表，用于存储微逆额定功率
    QHash<QString,QString> mi_nominal_pw;

    QHash<QString,QString> mi_mim_version;
    //用于存储状态时间的字符串变量
    QString st_time;

    //从微逆属性表中读取额定功率并存储到哈希表中
    mi_pty_tb.read_nominal_power(mDataBase,QStringList() << mi,mi_nominal_pw);

    //4.22修改
    mi_pty_tb.read_nominal_power(mDataBase,QStringList() << mi,mi_mim_version);

    //根据微逆类型（单路、双路或四路）进行循环
    for(int i=0;i<mi_type;i++)
    {
        QJsonObject pv_data;
        // 将光伏编号作为"pv_id"字段插入到s_data对象中
        s_data.insert("pv_id",QString::number(i+1));

        //从功率数据表中读取数据
        pw_dt_tb.read_data(mDataBase,s_data,read_pv_data);

        //将读取到的光伏数据作为"pv_datas"字段插入到pv_data对象中
        pv_data.insert("pv_datas",read_pv_data.value("datas").toArray());

        //将光伏编号作为"pv_id"字段插入到pv_data对象中
        pv_data.insert("pv_id",i+1);

        //获取状态时间
        st_time = read_pv_data.value("stop_date").toString();

        //将光伏数据对象添加到datas数组中
        datas.append(pv_data);

    }

    //准备最终的响应JSON对象
    // 插入微逆编号
    rt_data.insert("mi_cid", mi);
    // 插入微逆类型的数量
    rt_data.insert("all_pv", QString::number(mi_type));
    // 插入单个微逆的额定功率
    rt_data.insert("pv_nominal_power", mi_nominal_pw.value(mi, "1000000").toInt());
    // 插入总的额定功率
    rt_data.insert("total_nominal_power", mi_nominal_pw.value(mi, "1000000").toInt() * mi_type);
    // 插入开始时间
    rt_data.insert("start_time", startTime);
    // 插入结束时间
    rt_data.insert("stop_time", st_time);
    // 插入光伏数据数组
    rt_data.insert("datas", datas);

    //版本号  4.22修改
    rt_data.insert("mim_version",mi_mim_version.value("mim_version", "0.0"));
    rt_data.insert("mis_version",mi_mim_version.value("mis_version", "0.0"));
}

void agingservice::writeMiAgingReport(QString mi, QString startTime, QString stopTime, QString alg, QString report)
{
    ag_mi_report_table mi_report_tb;
    aging_report_strc s_data;
    s_data.mi_cid = mi;
    s_data.start_time = startTime;
    s_data.stop_time = stopTime;
    s_data.alg = alg;
    s_data.report = report;
    s_data.update_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    mi_report_tb.write_mi_report(mDataBase,s_data);
}

bool agingservice::readMiAgingReport(QString mi, QString startTime, QString stopTime, QString alg, QString &report)
{
    ag_mi_report_table mi_report_tb;
    QVector<aging_report_strc> r_data;
    mi_report_tb.read_mi_report(mDataBase,mi,startTime,stopTime,r_data);

    for(int i=0;i<r_data.size();i++)
    {
        if(alg == r_data[i].alg)
        {
            report = r_data[i].report;
            return true;
        }
    }
    return false;
}
