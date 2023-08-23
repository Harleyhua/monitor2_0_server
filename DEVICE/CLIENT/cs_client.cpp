#include "cs_client.h"

#include <QByteArray>
#include <QTextCodec>
#include <QJsonDocument>
#include "common.h"
#include "QsLog.h"

//#include "ag_temp_table.h"
#include "ag_user_table.h"
#include "ag_mi_property_table.h"
#include "aging_alg.h"
#include "abstract_bym.h"
cs_client::cs_client(QObject *parent)
    :HttpRequestHandler(parent)
{

}

void cs_client::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray body;
    mysql sql(common::rand_str());
    QString name = QString::fromUtf8(request.getParameter("account"));
    QString password = QString::fromUtf8(request.getParameter("password"));

    //身份验证
    if(!is_user_valid(sql,name,password))
    {
        response.setStatus(500,"账户不存在或密码错误");
        response.write(body,true);
        return;
    }
    else
    {
        QJsonObject rev_data;
        QJsonObject ret_data;

        rev_data = common::qbytearray_to_js(request.getBody());

        if(request.getPath().startsWith("/r_user_property"))
        {
            //登录指令
            sql.r_user_property(rev_data,ret_data);
        }
//        else if(request.getPath().startsWith("/r_mapping"))
//        {
//            //读关系列表数据
//            //sql.r_mapping(rev_data,ret_data);
//        }
        else if(request.getPath().startsWith("/r_emu_action"))
        {
            //读网关action
            sql.r_emu_action(rev_data,ret_data);
        }
        else if(request.getPath().startsWith("/r_rack_data"))
        {
            sql.r_mi_rack_data(rev_data,ret_data);
        }
        else if(request.getPath().startsWith("/r_mi_current_power"))
        {
            QStringList mi_list;
            //读关系列表数据
            //QLOG_INFO() << "1:" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            sql.r_mapping(name,ret_data,mi_list);
            //QLOG_INFO() << "2:" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            //读实时发电数据
            sql.r_mi_current_power(mi_list,ret_data);
            //QLOG_INFO() << "3:" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        }
        else if(request.getPath().startsWith("/r_mi_power"))
        {
            //读发电数据
            sql.r_mi_power(rev_data,ret_data);
        }
        else if(request.getPath().startsWith("/r_ag_room_temp"))
        {
            //读温度数据
            sql.r_temp(rev_data,ret_data);
        }
        else if(request.getPath().startsWith("/r_workorder"))
        {
            sql.r_workorder(rev_data,ret_data);
        }
        else if(request.getPath().startsWith("/w_workorder"))
        {
            sql.w_workorder(rev_data);
        }
        else if(request.getPath().startsWith("/r_one_plot_mi_power"))
        {
            sql.r_mi_power(rev_data,ret_data,true);
        }
        else if(request.getPath().startsWith("/r_one_mi_power"))
        {
            sql.r_mi_power(rev_data,ret_data,true);
        }
        else if(request.getPath().startsWith("/add_station"))
        {
            sql.add_station(name,rev_data);
        }
        else if(request.getPath().startsWith("/add_emu_cid"))
        {
            sql.add_emu_cid(name,rev_data);
        }
        else if(request.getPath().startsWith("/add_plcmi_cid"))
        {
            sql.add_plcmi_cid(name,rev_data);
        }
        else if(request.getPath().startsWith("/add_wifimi_cid"))
        {
            sql.add_wifimi_cid(name,rev_data);
        }
        else if(request.getPath().startsWith("/del_mi"))
        {
            sql.del_mi(name,rev_data);
        }
        else if(request.getPath().startsWith("/del_emu"))
        {
            sql.del_emu(name,rev_data);
        }
        else if(request.getPath().startsWith("/del_station"))
        {
            sql.del_station(name,rev_data);
        }
        else if(request.getPath().startsWith("/aging_report"))
        {
            QJsonObject select_param = rev_data.value("select_param").toObject();
            QJsonArray mis_array = select_param.value("mis").toArray();
            QJsonArray mis_report;

            //最终都会转化为编号
            for(int i=0;i<mis_array.size();i++)
            {
                QJsonObject mi_report;
                //要进行分析的老化数据
                QJsonObject aging_data;
                //读取数据
                QString room;
                QString start_time;
                QString stop_time;
                QString pos_desc;            //微逆老化时的位置信息

                QJsonObject jud_params;      //当前账户下的判断参数
                QString tmp_mi_judge_params; //当前微逆判断参数
                QString history_report;      //当前参数下的历史报告
                if(select_param.value("mode").toInt() == 0)     //按编号解析
                {
                    //读取最后一次老化的  参数  用于读取老化数据
                    sql.r_mi_last_aging_param(mis_array[i].toString(""),room,start_time,stop_time,pos_desc);
                }
                else if(select_param.value("mode").toInt() == 1)//按老化批次
                {
                    start_time = select_param.value("start_time").toString();
                    pos_desc = select_param.value("pos_desc").toString();

                    sql.r_aging_batch_by_room_start_time(mis_array[i].toString(""),room,start_time,stop_time);
                }

                //读取当前用户使用的判断参数
                sql.r_user_judge_params(name,jud_params);
                //读取微逆老化数据
                sql.r_aging_data_by_mi_date(mis_array[i].toString(""),start_time,stop_time,aging_data);
                //只记载某个微逆型号的judge_param
                tmp_mi_judge_params = QString(QJsonDocument(jud_params.value("bym" + QString::number(aging_data.value("total_nominal_power").toInt())).toObject()).toJson());


                if(sql.r_mi_report(aging_data.value("mi_cid").toString(),aging_data.value("start_time").toString(),
                                   aging_data.value("stop_time").toString(),tmp_mi_judge_params,history_report))
                {
                    mi_report = QJsonDocument::fromJson(history_report.toUtf8()).object();
                }
                else
                {
                    aging_alg tmp_aging_alg;
                    QHash<QString, uint16_t> room_temp;
                    //读取该微逆的温度数据
                    sql.r_temp(room,aging_data.value("start_time").toString(),aging_data.value("stop_time").toString(),room_temp);

                    //进行老化分析
                    tmp_aging_alg.aging_report(aging_data,jud_params.value("bym" + QString::number(aging_data.value("total_nominal_power").toInt())).toObject(),
                                               room_temp,mi_report);
                    //将当前生成的报告写入数据库
                    sql.w_mi_report(aging_data.value("mi_cid").toString(),aging_data.value("start_time").toString(),
                                    aging_data.value("stop_time").toString(),tmp_mi_judge_params,QString(QJsonDocument(mi_report).toJson()));
                }

                //顺带记录 老化房间  回溯老化环境温度
                mi_report.insert("room",room);
                mi_report.insert("pos_desc",pos_desc);
                mis_report.append(mi_report);
            }
            ret_data.insert("report_datas",mis_report);
        }
        else if(request.getPath().startsWith("/write_room_temp"))
        {
            //老化房的温度采集
            sql.w_room_temp(rev_data);
        }
        else if(request.getPath().startsWith("/update_alg"))
        {
            if(sql.update_alg(name,rev_data))
            {
                ret_data.insert("ret",true);
            }
            else
            {
                ret_data.insert("ret",false);
            }
        }
        else if(request.getPath().startsWith("/batch_list"))
        {
            sql.r_batch_list(rev_data,ret_data);
        }
//        else if(request.getPath().startsWith("/device_ctl"))
//        {
//            sql.w_device_ctl(name,rev_data);
//        }
        else
        {
            response.setStatus(401,"没当前指令的解析");
        }

        response.write(common::js_to_qbytearray(ret_data),true);
    }
}

bool cs_client::is_user_valid(mysql &sql, QString name, QString password)
{
    QJsonObject user_obj;
    QJsonObject user_data_obj;

    sql.r_user(name,user_obj);

    user_data_obj = user_obj.value("datas").toObject();

    if(password == user_data_obj.value(ag_user_table::c_field_password).toString())
    {
        return true;
    }
    return false;
}
