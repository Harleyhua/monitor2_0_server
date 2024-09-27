#include "cs_client.h"

#include <QByteArray>
#include <QTextCodec>
#include <QJsonDocument>
#include <QDir>

#include "common.h"
#include "QsLog.h"

//#include "ag_temp_table.h"
#include "ag_user_table.h"
//#include "ag_mi_property_table.h"
#include "aging_alg.h"
//#include "abstract_bym.h"
#include "emu_protocolb.h"
/**
 * 该类的功能已经被重构了  暂无使用
 */
#define SAVE_MI_REPORT 1

extern QString g_ota_beta_dir_path;     //ota测试文件目录
extern QString g_ota_official_dir_path; //ota正式文件目录

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
            QLOG_INFO() << "读实时数据列表节点start:" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            sql.r_mapping(name,ret_data,mi_list);
            QLOG_INFO() << "列表stop 读实时发电数据start:" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            //读实时发电数据
            sql.r_mi_current_power(mi_list,ret_data);
            QLOG_INFO() << "读实时发电数据列表节点:" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
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
            QString us_act_str;
            QString total_station = sql.r_total_station(name);

            QJsonArray stas_array = rev_data.value("station").toArray();
            QString sta_str;
            for(int i=0;i<stas_array.size();i++)
            {
                sta_str.append(stas_array[i].toString() + ",");
            }
            us_act_str = "添加电站列表 " + sta_str;
            sql.w_user_act(name,total_station,2,us_act_str);

            sql.add_station(name,rev_data);
        }
        else if(request.getPath().startsWith("/add_emu_cid"))
        {
            QString us_act_str;
            QString total_station = sql.r_total_station(name);
            QString station = rev_data.value("station").toString();
            QJsonArray emus_array = rev_data.value("emus").toArray();

            QString emu_str;
            for(int i=0;i<emus_array.size();i++)
            {
                emu_str.append(emus_array[i].toObject().value("emu_cid").toString() + ",");
            }

            us_act_str = "添加电站 " + station + "下的网关 " + emu_str;

            sql.w_user_act(name,total_station,2,us_act_str);

            sql.add_emu_cid(name,rev_data);
        }
        else if(request.getPath().startsWith("/add_plcmi_cid"))
        {
            QString us_act_str;
            QString total_station = sql.r_total_station(name);
            QString station = rev_data.value("station").toString();
            QString emu = rev_data.value("emu").toString();
            QJsonArray mis_array = rev_data.value("mis").toArray();

            QString mi_str;
            for(int i=0;i<mis_array.size();i++)
            {
                mi_str.append(mis_array[i].toObject().value("mi_cid").toString() + ",");
            }
            us_act_str = "添加电站:" + station + "网关:" + emu + "下的PLC版微逆:" + mi_str;

            sql.w_user_act(name,total_station,2,us_act_str);

            sql.add_plcmi_cid(name,rev_data);
        }
        else if(request.getPath().startsWith("/add_wifimi_cid"))
        {
            QString us_act_str;
            QString total_station = sql.r_total_station(name);
            QString station = rev_data.value("station").toString();
            //QString emu = rev_data.value("emu").toString();
            QJsonArray emus_array = rev_data.value("emus").toArray();

            QString mi_str;
            for(int i=0;i<emus_array.size();i++)
            {
                mi_str.append(emus_array[i].toObject().value("mi").toObject().value("mi_cid").toString() + ",");
            }
            us_act_str = "添加电站:" + station + "下的WIFI版微逆:" + mi_str;

            sql.w_user_act(name,total_station,2,us_act_str);

            sql.add_wifimi_cid(name,rev_data);
        }
        else if(request.getPath().startsWith("/del_mi"))
        {
            QString us_act_str;
            QString total_station = sql.r_total_station(name);
            us_act_str = "删除" + rev_data.value("station").toString() + " " +
                         rev_data.value("emu_cid").toString() + "下的微逆:" +
                         rev_data.value("mi_cid").toString();
            sql.w_user_act(name,total_station,1,us_act_str);

            sql.del_mi(name,rev_data);
        }
        else if(request.getPath().startsWith("/del_emu"))
        {
            QString us_act_str;
            QString total_station = sql.r_total_station(name);
            us_act_str = "删除" + rev_data.value("station").toString() + "下的网关:" +
                         rev_data.value("emu_cid").toString();
            sql.w_user_act(name,total_station,1,us_act_str);

            sql.del_emu(name,rev_data);
        }
        else if(request.getPath().startsWith("/del_station"))
        {
            QString us_act_str;
            QString total_station = sql.r_total_station(name);
            us_act_str = "删除电站" + rev_data.value("station").toString();
            sql.w_user_act(name,total_station,1,us_act_str);

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
                    //pos_desc = select_param.value("pos_desc").toString();

                    sql.r_aging_batch_by_room_start_time(mis_array[i].toString(""),room,start_time,stop_time,pos_desc);
                }

                int v_aging_time = -1;
                qint64 v_set_stop_time = 0;
                qint64 v_next_stop_time = 0;
                //读取该微逆  当前老化的老化时间
                sql.r_aging_time(mis_array[i].toString(""),start_time,v_aging_time);
                //预设的停止时间
                v_set_stop_time = QDateTime::fromString(start_time,"yyyy-MM-dd hh:mm:ss").toSecsSinceEpoch() + v_aging_time *60;
                //实际的下次的停止时间
                v_next_stop_time = QDateTime::fromString(stop_time,"yyyy-MM-dd hh:mm:ss").toSecsSinceEpoch();

                //修正结束时间
                if(v_aging_time != -1 && v_set_stop_time <= v_next_stop_time)
                {
                    stop_time = QDateTime::fromSecsSinceEpoch(v_set_stop_time).toString("yyyy-MM-dd hh:mm:ss");
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

                    int agetime = 0;
                    //进行老化分析
                    tmp_aging_alg.aging_report(aging_data,jud_params.value("bym" + QString::number(aging_data.value("tot al_nominal_power").toInt())).toObject(),
                                               room_temp,mi_report,agetime);

#if(SAVE_MI_REPORT)
                    //已经老化结束的情况下
                    if(stop_time < QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                    {
                        //将当前生成的报告写入数据库
                        sql.w_mi_report(aging_data.value("mi_cid").toString(),aging_data.value("start_time").toString(),
                                       aging_data.value("stop_time").toString(),tmp_mi_judge_params,QString(QJsonDocument(mi_report).toJson()));
                    }
#endif
                }
                //顺带记录 老化房间  回溯老化环境温度
                mi_report.insert("room",room);
                mi_report.insert("pos_desc",pos_desc);
                mis_report.append(mi_report);
            }
            ret_data.insert("report_datas",mis_report);
        }
        // else if(request.getPath().startsWith("/write_room_temp"))
        // {
        //     //老化房的温度采集
        //     sql.w_room_temp(rev_data);
        // }
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
        else if(request.getPath().startsWith("/r_user_act"))
        {
            QString total_station = sql.r_total_station(name);
            rev_data.insert("total_station",total_station);

            sql.r_user_act(rev_data,ret_data);
        }
        else if(request.getPath().startsWith("/r_temporary_power"))
        {
            QString data;
            QString total_station = sql.r_total_station(name);
            //读临时功率
            sql.r_mi_temporary_power(rev_data.value("mi_cid").toString(),data);

            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   rev_data.value("ctl_time").toString(),
                                   rev_data.value("server_cmd").toInt(),
                                   rev_data.value("data").toString());

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
            ret_data.insert("mi_cid",rev_data.value("mi_cid").toString());
            ret_data.insert("temporary_power",data);
        }
        else if(request.getPath().startsWith("/r_more_temporary_power"))//
        {
            QString data;
            QString total_station = sql.r_total_station(name);
            QJsonArray mis = rev_data.value("mis").toArray();
            QJsonArray ret_mis; //返回的值
            for(int i=0;i<mis.size();i++)
            {
                QJsonObject mi;
                //读临时功率
                sql.r_mi_temporary_power(mis[i].toString(),data);
                mi.insert("mi_cid",mis[i].toString());
                mi.insert("temporary_power",data);

                ret_mis.append(mi);
            }

            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   rev_data.value("ctl_time").toString(),
                                   rev_data.value("server_cmd").toInt(),
                                   rev_data.value("data").toString());

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
            ret_data.insert("mis",ret_mis);
        }
        else if(request.getPath().startsWith("/w_temporary_power"))
        {
            QString total_station = sql.r_total_station(name);
            sql.w_mi_temporary_power(rev_data.value("mi_cid").toString(),
                                           rev_data.value("temporary_power").toString());

            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   rev_data.value("ctl_time").toString(),
                                   rev_data.value("server_cmd").toInt(),
                                   rev_data.value("mi_cid").toString());

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
            ret_data.insert("mi_cid",rev_data.value("mi_cid").toString());

        }
        else if(request.getPath().startsWith("/w_more_temporary_power"))//
        {
            QString total_station = sql.r_total_station(name);

            QJsonArray mis = rev_data.value("mis").toArray();
            QString mislist_str;
            for(int i=0;i<mis.size();i++)
            {
                sql.w_mi_temporary_power(mis[i].toObject().value("mi_cid").toString(),
                                         mis[i].toObject().value("temporary_power").toString());
                mislist_str.append(mis[i].toObject().value("mi_cid").toString());
            }

            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   rev_data.value("ctl_time").toString(),
                                   rev_data.value("server_cmd").toInt(),
                                   mislist_str);

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
            ret_data.insert("mis",rev_data.value("mis").toArray());

        }
        else if(request.getPath().startsWith("/r_max_power"))
        {
            QString data;
            QString total_station = sql.r_total_station(name);
            sql.r_mi_max_power(rev_data.value("mi_cid").toString(),data);

            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   rev_data.value("ctl_time").toString(),
                                   rev_data.value("server_cmd").toInt(),
                                   rev_data.value("data").toString());

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
            ret_data.insert("mi_cid",rev_data.value("mi_cid").toString());
            ret_data.insert("max_power",data);
        }
        else if(request.getPath().startsWith("/r_more_max_power"))//
        {
            QString data;
            QString total_station = sql.r_total_station(name);
            sql.r_mi_max_power(rev_data.value("mi_cid").toString(),data);
            QJsonArray mis = rev_data.value("mis").toArray();
            QJsonArray ret_mis; //返回的值
            for(int i=0;i<mis.size();i++)
            {
                QJsonObject mi;
                //读临时功率
                sql.r_mi_max_power(mis[i].toString(),data);
                mi.insert("mi_cid",mis[i].toString());
                mi.insert("max_power",data);

                ret_mis.append(mi);
            }



            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   rev_data.value("ctl_time").toString(),
                                   rev_data.value("server_cmd").toInt(),
                                   rev_data.value("data").toString());

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
            ret_data.insert("mis",ret_mis);

        }
        else if(request.getPath().startsWith("/w_more_max_power"))//
        {
            QString total_station = sql.r_total_station(name);

            QJsonArray mis = rev_data.value("mis").toArray();
            QString mislist_str;
            for(int i=0;i<mis.size();i++)
            {
                sql.w_mi_max_power(mis[i].toObject().value("mi_cid").toString(),
                                   mis[i].toObject().value("max_power").toString());
                mislist_str.append(mis[i].toObject().value("mi_cid").toString());
            }

            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   rev_data.value("ctl_time").toString(),
                                   rev_data.value("server_cmd").toInt(),
                                   mislist_str);

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
            ret_data.insert("mis",rev_data.value("mis").toArray());
        }
        else if(request.getPath().startsWith("/w_max_power"))
        {
            QString total_station = sql.r_total_station(name);
            sql.w_mi_max_power(rev_data.value("mi_cid").toString(),
                                           rev_data.value("max_power").toString());

            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   rev_data.value("ctl_time").toString(),
                                   rev_data.value("server_cmd").toInt(),
                                   rev_data.value("mi_cid").toString());

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
            ret_data.insert("mi_cid",rev_data.value("mi_cid").toString());
        }
        else if(request.getPath().startsWith("/r_grid"))
        {
            QString data;
            QString total_station = sql.r_total_station(name);
            sql.r_mi_grid(rev_data.value("mi_cid").toString(),data);

            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   rev_data.value("ctl_time").toString(),
                                   rev_data.value("server_cmd").toInt(),
                                   rev_data.value("data").toString());

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
            ret_data.insert("mi_cid",rev_data.value("mi_cid").toString());
            ret_data.insert("grid",data);

        }
        else if(request.getPath().startsWith("/w_grid"))
        {
            QString total_station = sql.r_total_station(name);
            sql.w_mi_grid(rev_data.value("mi_cid").toString(),
                                           rev_data.value("grid").toString());

            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   rev_data.value("ctl_time").toString(),
                                   rev_data.value("server_cmd").toInt(),
                                   rev_data.value("mi_cid").toString());

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
            ret_data.insert("mi_cid",rev_data.value("mi_cid").toString());
        }
        else if(request.getPath().startsWith("/r_certification"))
        {
            QString data;
            QString total_station = sql.r_total_station(name);
            sql.r_mi_certification(rev_data.value("mi_cid").toString(),data);

            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   rev_data.value("ctl_time").toString(),
                                   rev_data.value("server_cmd").toInt(),
                                   rev_data.value("data").toString());

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
            ret_data.insert("mi_cid",rev_data.value("mi_cid").toString());
            ret_data.insert("certification",data);

        }
        else if(request.getPath().startsWith("/w_certification"))
        {
            QString total_station = sql.r_total_station(name);
            sql.w_mi_certification(rev_data.value("mi_cid").toString(),
                                           rev_data.value("certification").toString());

            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   rev_data.value("ctl_time").toString(),
                                   rev_data.value("server_cmd").toInt(),
                                   rev_data.value("mi_cid").toString());

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
            ret_data.insert("mi_cid",rev_data.value("mi_cid").toString());

        }
        else if(request.getPath().startsWith("/w_emu_func"))
        {
            QString total_station = sql.r_total_station(name);
            sql.w_emu_func_code(rev_data.value("emu_cid").toString(),
                                rev_data.value("func").toString());

            sql.update_dev_control(total_station,rev_data.value("station").toString(),
                                   rev_data.value("emu_cid").toString(),
                                   QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),
                                   rev_data.value("server_cmd").toInt(),
                                   rev_data.value("data").toString());
        }
        else if(request.getPath().startsWith("/r_select_back"))
        {
            QString maxop_data,cop_data,grid_data,cer_data,
                    r_maxop_data,r_cop_data,r_grid_data,r_cer_data;
            QJsonObject obj;

            obj = rev_data.value("r_maxop").toObject();
            if(obj.value("r_maxop_ctl_time").toString() != "" &&
               sql.r_data_send_flag(rev_data.value("emu_cid").toString(),obj.value("r_maxop_ctl_time").toString()))
            {
                QJsonArray mis_arr = obj.value("mis").toArray();
                QJsonObject mi_obj;
                QJsonArray mis_ret_arr;
                QJsonObject r_mxopret_obj;
                for(int i=0;i<mis_arr.size();i++)
                {
                    r_maxop_data = "";
                    sql.r_mi_max_power(mis_arr[i].toString(),r_maxop_data);

                    mi_obj.insert("mi_cid",mis_arr[i].toString());
                    mi_obj.insert("v_max_power",r_maxop_data);

                    mis_ret_arr.append(mi_obj);
                }

                r_mxopret_obj.insert("r_maxop_time",obj.value("r_maxop_ctl_time").toString());
                r_mxopret_obj.insert("mis_maxop",mis_ret_arr);

                ret_data.insert("r_maxop",r_mxopret_obj);
            }


            obj = rev_data.value("r_cop").toObject();
            if(obj.value("r_cop_ctl_time").toString() != "" &&
               sql.r_data_send_flag(rev_data.value("emu_cid").toString(),obj.value("r_cop_ctl_time").toString()))
            {
                QJsonArray mis_arr = obj.value("mis").toArray();
                QJsonObject mi_obj;
                QJsonArray mis_ret_arr;
                QJsonObject r_copret_obj;
                for(int i=0;i<mis_arr.size();i++)
                {
                    r_cop_data = "";
                    sql.r_mi_temporary_power(mis_arr[i].toString(),r_cop_data);

                    mi_obj.insert("mi_cid",mis_arr[i].toString());
                    mi_obj.insert("v_cop",r_cop_data);

                    mis_ret_arr.append(mi_obj);
                }
                r_copret_obj.insert("r_cop_time",obj.value("r_cop_ctl_time").toString());
                r_copret_obj.insert("mis_cop",mis_ret_arr);

                ret_data.insert("r_cop",r_copret_obj);
            }

            obj = rev_data.value("r_grid").toObject();
            if(obj.value("r_grid_ctl_time").toString() != "" &&
                    sql.r_data_send_flag(rev_data.value("emu_cid").toString(),obj.value("r_grid_ctl_time").toString()))
            {
                QJsonArray mis_arr = obj.value("mis").toArray();
                QJsonObject mi_obj;
                QJsonArray mis_ret_arr;
                QJsonObject r_gridret_obj;
                for(int i=0;i<mis_arr.size();i++)
                {
                    r_grid_data = "";
                    sql.r_mi_grid(mis_arr[i].toString(),r_grid_data);

                    mi_obj.insert("mi_cid",mis_arr[i].toString());
                    mi_obj.insert("v_grid",r_grid_data);

                    mis_ret_arr.append(mi_obj);
                }
                r_gridret_obj.insert("r_grid_time",obj.value("r_grid_ctl_time").toString());
                r_gridret_obj.insert("mis_grid",mis_ret_arr);

                ret_data.insert("r_grid",r_gridret_obj);
            }

            obj = rev_data.value("r_cer").toObject();
            if(obj.value("r_cer_ctl_time").toString() != "" &&
                    sql.r_data_send_flag(rev_data.value("emu_cid").toString(),obj.value("r_cer_ctl_time").toString()))
            {
                QJsonArray mis_arr = obj.value("mis").toArray();
                QJsonObject mi_obj;
                QJsonArray mis_ret_arr;
                QJsonObject r_cerret_obj;
                for(int i=0;i<mis_arr.size();i++)
                {
                    r_cer_data = "";
                    sql.r_mi_certification(mis_arr[i].toString(),r_cer_data);

                    mi_obj.insert("mi_cid",mis_arr[i].toString());
                    mi_obj.insert("v_cer",r_cer_data);

                    mis_ret_arr.append(mi_obj);
                }
                r_cerret_obj.insert("r_cer_time",obj.value("r_cer_ctl_time").toString());
                r_cerret_obj.insert("mis_cer",mis_ret_arr);
                ret_data.insert("r_cer",r_cerret_obj);
            }

            obj = rev_data.value("w_maxop").toObject();
            if(obj.value("maxop_ctl_time").toString() != "" &&
               sql.r_data_send_flag(rev_data.value("emu_cid").toString(),obj.value("maxop_ctl_time").toString()))
            {

                QJsonArray mis_arr = obj.value("mis").toArray();
                QJsonObject mi_obj;
                QJsonArray mis_ret_arr;
                QJsonObject r_mxopret_obj;
                for(int i=0;i<mis_arr.size();i++)
                {
                    maxop_data = "";
                    sql.r_mi_max_power(mis_arr[i].toString(),maxop_data);

                    mi_obj.insert("mi_cid",mis_arr[i].toString());
                    mi_obj.insert("v_max_power",maxop_data);

                    mis_ret_arr.append(mi_obj);
                }

                r_mxopret_obj.insert("w_maxop_time",obj.value("maxop_ctl_time").toString());
                r_mxopret_obj.insert("mis_maxop",mis_ret_arr);

                ret_data.insert("w_maxop",r_mxopret_obj);

            }

            obj = rev_data.value("w_cop").toObject();
            if(obj.value("cop_ctl_time").toString() != "" &&
                    sql.r_data_send_flag(rev_data.value("emu_cid").toString(),obj.value("cop_ctl_time").toString()))
            {
                QJsonArray mis_arr = obj.value("mis").toArray();
                QJsonObject mi_obj;
                QJsonArray mis_ret_arr;
                QJsonObject r_copret_obj;
                for(int i=0;i<mis_arr.size();i++)
                {
                    cop_data = "";
                    sql.r_mi_temporary_power(mis_arr[i].toString(),cop_data);

                    mi_obj.insert("mi_cid",mis_arr[i].toString());
                    mi_obj.insert("v_cop",cop_data);

                    mis_ret_arr.append(mi_obj);
                }
                r_copret_obj.insert("w_cop_time",obj.value("cop_ctl_time").toString());
                r_copret_obj.insert("mis_cop",mis_ret_arr);

                ret_data.insert("w_cop",r_copret_obj);
            }

            obj = rev_data.value("w_grid").toObject();
            if(obj.value("grid_ctl_time").toString() != "" &&
                    sql.r_data_send_flag(rev_data.value("emu_cid").toString(),obj.value("grid_ctl_time").toString()))
            {
                QJsonArray mis_arr = obj.value("mis").toArray();
                QJsonObject mi_obj;
                QJsonArray mis_ret_arr;
                QJsonObject r_gridret_obj;
                for(int i=0;i<mis_arr.size();i++)
                {
                    grid_data = "";
                    sql.r_mi_grid(mis_arr[i].toString(),grid_data);

                    mi_obj.insert("mi_cid",mis_arr[i].toString());
                    mi_obj.insert("v_grid",grid_data);

                    mis_ret_arr.append(mi_obj);
                }
                r_gridret_obj.insert("w_grid_time",obj.value("grid_ctl_time").toString());
                r_gridret_obj.insert("mis_grid",mis_ret_arr);
                ret_data.insert("w_grid",r_gridret_obj);

            }
            obj = rev_data.value("w_cer").toObject();
            if(obj.value("cer_ctl_time").toString() != "" &&
                    sql.r_data_send_flag(rev_data.value("emu_cid").toString(),obj.value("cer_ctl_time").toString()))
            {
                QJsonArray mis_arr = obj.value("mis").toArray();
                QJsonObject mi_obj;
                QJsonArray mis_ret_arr;
                QJsonObject r_cerret_obj;
                for(int i=0;i<mis_arr.size();i++)
                {
                    cer_data = "";
                    sql.r_mi_certification(mis_arr[i].toString(),cer_data);

                    mi_obj.insert("mi_cid",mis_arr[i].toString());
                    mi_obj.insert("mis_cer",cer_data);

                    mis_ret_arr.append(mi_obj);
                }
                r_cerret_obj.insert("w_cer_time",obj.value("cer_ctl_time").toString());
                r_cerret_obj.insert("mis_cer",mis_ret_arr);
                ret_data.insert("w_cer",r_cerret_obj);
            }

            ret_data.insert("station",rev_data.value("station").toString());
            ret_data.insert("emu_cid",rev_data.value("emu_cid").toString());
        }
        else if(request.getPath().startsWith("/r_emu_status"))
        {
            sql.r_emu_status(rev_data,ret_data);
        }
        else if(request.getPath().startsWith("/r_one_emu_status"))//
        {
            sql.r_emu_status(rev_data,ret_data);
        }
//        else if(request.getPath().startsWith("/upload_ota_file"))//
//        {
//            QString newfileName = g_ota_beta_dir_path + QDir::separator() + QString(request.getParameter("file_name"));
//            QFile file(newfileName);
//            if (file.open(QIODevice::WriteOnly))
//            {
//                file.write(request.getBody());
//                // 关闭文件
//                file.close();
//            }

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

    sql.r_user_property(name,user_obj);

    user_data_obj = user_obj.value("datas").toObject();

    if(password == user_data_obj.value(ag_user_table::c_field_password).toString())
    {
        return true;
    }
    return false;
}
