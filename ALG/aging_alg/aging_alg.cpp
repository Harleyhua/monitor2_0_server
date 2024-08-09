#include "aging_alg.h"

#include <QJsonArray>
#include <QSet>
#include <QDateTime>

/*
    老化算法
*/

//#include "abstract_bym.h"
aging_alg::aging_alg()
{

}

void aging_alg::aging_report(QJsonObject s_data, QJsonObject judge_param, QHash<QString, uint16_t> &temp, QJsonObject &r_data,int age_time)
{
    int pv_size = s_data.value("all_pv").toString().toInt();
    int pv_nominal_power = s_data.value("pv_nominal_power").toInt(1000000);
    int total_nominal_power = s_data.value("total_nominal_power").toInt(1000000);
    //起始点测算偏移point数量 默认3个
    int start_point_offset = judge_param.value("v_start_point_offset").toInt(3);
    int drop_thr = judge_param.value("drop_value").toInt(70);
    int drop_points_thr = judge_param.value("drop_points_max").toInt(5);
    int ignore_points_thr = judge_param.value("ignore_points_max").toInt(10);
    //int nominal_pw_thr = judge_param.value("nominal_pw_thr").toInt(100000);

    //阈值
    int agingPoints = judge_param.value("v_aging_points").toInt();
    int agingTime = judge_param.value("v_aging_time").toInt();
    //int age_Time = age_time;


    QJsonArray pv_ret;
    QSet<uint> rm_idx_set;
    int mi_point_size = 0;

    //精度0.01
#if CAL_TEMP40_SWITCH
    int tmp_total_40power_max = DEFAULT_VALUE_MAX;
    int tmp_total_40power_min = DEFAULT_VALUE_MIN;
    int tmp_total_40temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP45_SWITCH
    int tmp_total_45power_max = DEFAULT_VALUE_MAX;
    int tmp_total_45power_min = DEFAULT_VALUE_MIN;
    int tmp_total_45temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP50_SWITCH
    int tmp_total_50power_max = DEFAULT_VALUE_MAX;
    int tmp_total_50power_min = DEFAULT_VALUE_MIN;
    int tmp_total_50temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP55_SWITCH
    int tmp_total_55power_max = DEFAULT_VALUE_MAX;
    int tmp_total_55power_min = DEFAULT_VALUE_MIN;
    int tmp_total_55temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP60_SWITCH
    int tmp_total_60power_max = DEFAULT_VALUE_MAX;
    int tmp_total_60power_min = DEFAULT_VALUE_MIN;
    int tmp_total_60temp_max = DEFAULT_VALUE_MAX;
#endif
    int tmp_total_power_max = DEFAULT_VALUE_MAX;
    int tmp_total_power_min = DEFAULT_VALUE_MIN;
    int tmp_total_temp_max = DEFAULT_VALUE_MAX;

    int total_start_power = 0;

    int b_ret = 0; //ok-0 again-1 err-2
    aging_err_cause cur_err_cause = pass_data;
    QString total_ret_str;    //整机 输出判断的文本结果

    QString total_stop_time = "2030-01-30 17:05:33";

    int all_data_size = 0;
    for(int i=0;i<pv_size;i++)
    {
        QJsonArray pv_datas = s_data.value("datas").toArray()[i].toObject().value("pv_datas").toArray();
        QString pv_ret_str;  //pv侧的老化结果文本
        //精度0.01
#if CAL_TEMP40_SWITCH
        int tmp_40power_max = DEFAULT_VALUE_MAX;
        int tmp_40power_min = DEFAULT_VALUE_MIN;
        int tmp_40temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP45_SWITCH
        int tmp_45power_max = DEFAULT_VALUE_MAX;
        int tmp_45power_min = DEFAULT_VALUE_MIN;
        int tmp_45temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP50_SWITCH
        int tmp_50power_max = DEFAULT_VALUE_MAX;
        int tmp_50power_min = DEFAULT_VALUE_MIN;
        int tmp_50temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP55_SWITCH
        int tmp_55power_max = DEFAULT_VALUE_MAX;
        int tmp_55power_min = DEFAULT_VALUE_MIN;
        int tmp_55temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP60_SWITCH
        int tmp_60power_max = DEFAULT_VALUE_MAX;
        int tmp_60power_min = DEFAULT_VALUE_MIN;
        int tmp_60temp_max = DEFAULT_VALUE_MAX;
#endif
        int tmp_power_max = DEFAULT_VALUE_MAX;
        int tmp_power_min = DEFAULT_VALUE_MIN;
        int tmp_temp_max = DEFAULT_VALUE_MAX;

        uint16_t mim_code = 0; //累计故障码
        uint16_t mis_code = 0;

        //原边故障定义(14)
        uint16_t pv_ov_point = 0; //光伏板过压次数
        uint16_t pv_un_point = 0; //光伏板欠压次数
        uint16_t dcov_point = 0; //变压器过压次数
        uint16_t dcoa_point = 0; //变压器过流次数
        uint16_t mi_island_point = 0; //逆变器孤岛
        uint16_t low_res_point = 0; //绝缘阻抗低
        uint16_t pv_ov_point2 = 0; //光伏板过压2
        uint16_t pv_un_point2 = 0; //光伏板欠压2
        uint16_t dc_ov_point2 = 0; //开关管过压2
        uint16_t dc_oc_point2 = 0; //开关管过流2
        uint16_t sync_err_point = 0; //原副边同步异常
        uint16_t safe_err_point = 0; //安全链信号异常
        uint16_t grid_point = 0; //电网电压频率异常
        uint16_t remote_point = 0; //遥控关机使能

        //副边故障定义(16)
        uint16_t grid_ov_point = 0; //电网过压
        uint16_t grid_un_point = 0; //电网欠压
        uint16_t grid_ovfre_point = 0; //电网过频
        uint16_t grid_unfre_point = 0; //电网欠频
        uint16_t lock_err_point = 0; //锁相异常
        uint16_t grid_surge_point = 0; //电网电压冲击
        uint16_t SEC_MCU_point = 0; //副边 MCU 电压过低
        uint16_t mispv_un_point = 0; //PV 电压过低
        uint16_t H_oc_point = 0; //H 桥过流
        uint16_t H_ov_point = 0; //H 桥过压
        uint16_t grid_dis_point = 0; //电网失真大
        uint16_t reserved = 0; //保留
        uint16_t PRI2_point = 0; //原边2与副边通信异常
        uint16_t PRI1_point = 0; //原边1与副边通信异常
        uint16_t grid_fre_point = 0; //电网频率未达恢复区间
        uint16_t grid_vo_point = 0; //电网电压未达恢复区间

        int ignore_points = 0; //发生异常时后续忽略的点数
        int ignore_times = 0;  //异常忽略的次数

        int drop_times = 0;   //跌落次数
        int start_power = 0;  //启动电压  是否满功率运行
        QString start_time = "";  //起始时间
        QString stop_time = "";
        //没有故障 有异常跌落情况
        int old_power_value = 0;

        if(pv_datas.size() >start_point_offset)
        {
            start_power = pv_datas[pv_datas.size()-start_point_offset].toObject().value("power").toString().toDouble() *100;
            start_time = pv_datas[pv_datas.size()-start_point_offset].toObject().value("sys_time").toString();
            stop_time = pv_datas[0].toObject().value("sys_time").toString();
        }
        total_stop_time = stop_time;

        mi_point_size = pv_datas.size();
        //累计整机启动功率
        total_start_power += start_power;

        old_power_value = start_power;  //避免第一次的判断
        //不解析前两个点  和后5个点
        for(int j=pv_datas.size() - start_point_offset;j>=start_point_offset;j--)
        {
            all_data_size += pv_datas.size();
            QJsonObject data = pv_datas[j].toObject();
            int cur_power = data.value("power").toString().toDouble() *100;      //当前point  功率
            int cur_temp = data.value("temperature").toString().toDouble() *100; //当前point  温度
            //当前故障码
            QString mim_err = data.value("mim_err").toString("");
            QString mis_err = data.value("mis_err").toString("");
            //某时刻的温度
            QDateTime tmp_dt = QDateTime::fromString(data.value("sys_time").toString(),"yyyy-MM-dd hh:mm:ss");
            uint16_t v_temp = temp.value(tmp_dt.toString("yyyy-MM-dd hh:")+QString("%1").arg((tmp_dt.toString("mm").toUInt()/5)*5,2,10,QLatin1Char('0')) + ":00",100);
            //是否计算某环境温度下标志
            bool cal_temp_flag = true;

            if(mim_err != "0000")
            {
               bool ret = false;
               uint16_t new_code = QString(mim_err).toInt(&ret,16);
               //光伏板过压
               if(new_code & 0x8000)
               {
                   pv_ov_point++;
               }
               //光伏板欠压
               if(new_code & 0x4000)
               {
                   pv_un_point++;
               }
               //变压器过压
               if(new_code & 0x2000)
               {
                   dcov_point++;
               }
               //变压器过流
               if(new_code & 0x1000)
               {
                   dcoa_point++;
               }
               // //开关管过温
               // if(new_code & 0x0800)
               // {
               //     dcov_point++;
               // }
               //逆变器孤岛
               if(new_code & 0x0400)
               {
                   mi_island_point++;
               }
               //绝缘阻抗低
               if(new_code & 0x0200)
               {
                   low_res_point++;
               }
               // //开关管过温2
               // if(new_code & 0x0100)
               // {
               //     dcoa_point++;
               // }
               //光伏板过压2
               if(new_code & 0x0080)
               {
                   pv_ov_point2++;
               }
               //光伏板欠压2
               if(new_code & 0x0040)
               {
                   pv_un_point2++;
               }
               //开关管过压2
               if(new_code & 0x0020)
               {
                   dc_ov_point2++;
               }
               //开关管过流2
               if(new_code & 0x0010)
               {
                   dc_oc_point2++;
               }
               //原副边同步异常
               if(new_code & 0x0008)
               {
                   sync_err_point++;
               }
               //安全链信号异常
               if(new_code & 0x0004)
               {
                   safe_err_point++;
               }
               //电网电压频率异常
               if(new_code & 0x0002)
               {
                   grid_point++;
               }
               //遥控关机使能
               if(new_code & 0x0001)
               {
                   remote_point++;
               }
               mim_code |= new_code;
            }

            //副边故障
            if(mis_err != "0000")
            {
                bool ret = false;
                uint16_t new_code = QString(mis_err).toInt(&ret,16);
                //电网过压
                if(new_code & 0x8000)
                {
                    grid_ov_point++;
                }
                //电网欠压
                if(new_code & 0x4000)
                {
                    grid_un_point++;
                }
                //电网过频
                if(new_code & 0x2000)
                {
                    grid_ovfre_point++;
                }
                //电网欠频
                if(new_code & 0x1000)
                {
                    grid_unfre_point++;
                }
                //锁相异常
                if(new_code & 0x0800)
                {
                    lock_err_point++;
                }
                //电网电压冲击
                if(new_code & 0x0400)
                {
                    grid_surge_point++;
                }
                //副边 MCU 电压过低
                if(new_code & 0x0200)
                {
                    SEC_MCU_point++;
                }
                //PV 电压过低
                if(new_code & 0x0100)
                {
                    mispv_un_point++;
                }
                //H 桥过流
                if(new_code & 0x0080)
                {
                    H_oc_point++;
                }
                //H 桥过压
                if(new_code & 0x0040)
                {
                    H_ov_point++;
                }
                //电网失真大
                if(new_code & 0x0020)
                {
                    grid_dis_point++;
                }
                //保留
                if(new_code & 0x0010)
                {
                    reserved++;
                }
                //原边2与副边通信异常
                if(new_code & 0x0008)
                {
                    PRI2_point++;
                }
                //原边1与副边通信异常
                if(new_code & 0x0004)
                {
                    PRI1_point++;
                }
                //电网频率未达恢复区间
                if(new_code & 0x0002)
                {
                    grid_fre_point++;
                }
                //电网电压未达恢复区间
                if(new_code & 0x0001)
                {
                    grid_vo_point++;
                }
                mis_code |= new_code;
            }

            // 温度最大值   此数据不能被忽略跳过
            if(cur_temp > tmp_temp_max)
                tmp_temp_max = cur_temp;


            //如果该点为忽略的点  不再统计这个点 跳到下个点
            if(ignore_points > 0)
            {
                --ignore_points;
                //记录被忽略的点  index
                rm_idx_set.insert(j);
                //功率还是正常刷新
                old_power_value = cur_power;
                //总计忽略次数
                ignore_times ++;
                continue;
            }

            bool ok_16;
            uint16_t ignore_mim_code = 0x0000;
            if(pv_size == 1)
            {
                ignore_mim_code = 0x0808;
            }
            else if(pv_size == 2 || pv_size == 4)
            {
                if(i == 0 || i== 2)
                {
                    //初始部分 0b00000001 11110000
                    //公共叠加 0b00000000 00001000
                    //自身叠加 0b00001000 00000000
                    ignore_mim_code = 0x09f8;
                }
                else if(i == 1 || i == 3)
                {
                    //初始部分 0b11111000 00000000
                    //公共叠加 0b00000000 00001000
                    //自身叠加 0b00000001 00000000
                    ignore_mim_code = 0xf908;
                }
            }

            //原边部分错误 去除判断
            if((mim_err.toUInt(&ok_16,16)) & (~ignore_mim_code))
            {
                //有故障 不计算温度
                cal_temp_flag = false;
                ignore_points = ERR_IGNORE_POINTS;  //设置后续忽略的点数

                rm_idx_set.insert(j);
                old_power_value = cur_power;
                //总计忽略次数
                ignore_times ++;
                continue;
            }
            //当前大于滤波点
            if(j <= pv_datas.size() - start_point_offset)
            {
                //跌幅超过% 认为波动异常  不统计功率  比较时cur_power都已经放大100倍
                if((cur_power * 100) < (old_power_value * drop_thr))
                {
                    drop_times ++;
                    cal_temp_flag = false;
                    ignore_points = ERR_IGNORE_POINTS;
                    rm_idx_set.insert(j);
                    old_power_value = cur_power;
                    //总计忽略次数
                    ignore_times ++;
                    continue;
                }
            }

            //特定环境温度下数据处理
            if(cal_temp_flag)
            {
                // 计算 功率最大,最小值  温度
                if(cur_power > tmp_power_max)
                    tmp_power_max = cur_power;
                if(cur_power < tmp_power_min)
                    tmp_power_min = cur_power;


#if CAL_TEMP40_SWITCH
                //房间温度 40℃
                if(v_temp >= 380 && v_temp<440)
                {
                    // 计算 功率最大,最小值
                    if(cur_power > tmp_40power_max)
                        tmp_40power_max = cur_power;
                    if(cur_power < tmp_40power_min)
                        tmp_40power_min = cur_power;
                    // 温度最大值
                    if(cur_temp > tmp_40temp_max)
                        tmp_40temp_max = cur_temp;
                }
#endif
#if CAL_TEMP45_SWITCH
                //房间温度 45℃
                if(v_temp >= 440 && v_temp < 490)
                {
                    // 计算功率最大,最小值
                    if(cur_power > tmp_45power_max)
                        tmp_45power_max = cur_power;
                    if(cur_power < tmp_45power_min)
                        tmp_45power_min = cur_power;
                    // 温度最大值
                    if(cur_temp > tmp_45temp_max)
                        tmp_45temp_max = cur_temp;
                }
#endif

#if CAL_TEMP50_SWITCH
                //房间温度 50℃
                if(v_temp >= 475 && v_temp < 540)
                {
                    // 计算功率最大,最小值
                    if(cur_power > tmp_50power_max)
                        tmp_50power_max = cur_power;
                    if(cur_power < tmp_50power_min)
                        tmp_50power_min = cur_power;
                    // 温度最大值
                    if(cur_temp > tmp_50temp_max)
                        tmp_50temp_max = cur_temp;
                }
#endif
#if CAL_TEMP55_SWITCH
                        //房间温度 55℃
                        if(temp >= 540 && temp < 590)
                        {
                            // 计算功率最大,最小值
                            if(cur_power > tmp_55power_max)
                                tmp_55power_max = cur_power;
                            if(cur_power < tmp_55power_min)
                                tmp_55power_min = cur_power;
                            // 温度最大值
                            if(cur_temp > tmp_55temp_max)
                                tmp_55temp_max = cur_temp;
                        }
#endif
#if CAL_TEMP60_SWITCH
                        //房间温度 60℃
                        if(temp >= 590)
                        {
                            // 计算功率最大,最小值
                            if(cur_power > tmp_60power_max)
                                tmp_60power_max = cur_power;
                            if(cur_power < tmp_60power_min)
                                tmp_60power_min = cur_power;
                            // 温度最大值
                            if(cur_temp > tmp_60temp_max)
                                tmp_60temp_max = cur_temp;
                        }
#endif
            }
            old_power_value = cur_power;
            //整机数据处理s
            if(i == pv_size -1)
            {
                //如果下标没被移除
                if(!rm_idx_set.contains(j))
                {

                    if(pv_size == 2)
                    {
                        cur_power = (s_data.value("datas").toArray()[0].toObject().value("pv_datas").toArray()[j].toObject().value("power").toString().toDouble() +
                                     s_data.value("datas").toArray()[1].toObject().value("pv_datas").toArray()[j].toObject().value("power").toString().toDouble())*100;
                        cur_temp = (s_data.value("datas").toArray()[0].toObject().value("pv_datas").toArray()[j].toObject().value("temperature").toString().toDouble() +
                                     s_data.value("datas").toArray()[1].toObject().value("pv_datas").toArray()[j].toObject().value("temperature").toString().toDouble())*50;    //*100/2
                    }
                    else if(pv_size == 4)
                    {
                        cur_power = (s_data.value("datas").toArray()[0].toObject().value("pv_datas").toArray()[j].toObject().value("power").toString().toDouble() +
                                     s_data.value("datas").toArray()[1].toObject().value("pv_datas").toArray()[j].toObject().value("power").toString().toDouble() +
                                     s_data.value("datas").toArray()[2].toObject().value("pv_datas").toArray()[j].toObject().value("power").toString().toDouble() +
                                     s_data.value("datas").toArray()[3].toObject().value("pv_datas").toArray()[j].toObject().value("power").toString().toDouble())*100;
                        cur_temp = (s_data.value("datas").toArray()[0].toObject().value("pv_datas").toArray()[j].toObject().value("temperature").toString().toDouble() +
                                     s_data.value("datas").toArray()[1].toObject().value("pv_datas").toArray()[j].toObject().value("temperature").toString().toDouble() +
                                     s_data.value("datas").toArray()[2].toObject().value("pv_datas").toArray()[j].toObject().value("temperature").toString().toDouble() +
                                     s_data.value("datas").toArray()[3].toObject().value("pv_datas").toArray()[j].toObject().value("temperature").toString().toDouble())*25;    //*100/4
                    }

                    // 计算 整机功率最大,最小值  温度
                    if(cur_power > tmp_total_power_max)
                        tmp_total_power_max = cur_power;
                    if(cur_power < tmp_total_power_min)
                        tmp_total_power_min = cur_power;
                    // 温度最大值
                    if(cur_temp > tmp_total_temp_max)
                        tmp_total_temp_max = cur_temp;

                    #if CAL_TEMP40_SWITCH
                    //房间温度 40℃
                    if(v_temp >= 390 && v_temp<440)
                    {
                        // 计算 功率最大,最小值
                        if(cur_power > tmp_total_40power_max)
                            tmp_total_40power_max = cur_power;
                        if(cur_power < tmp_total_40power_min)
                            tmp_total_40power_min = cur_power;
                        // 温度最大值
                        if(cur_temp > tmp_total_40temp_max)
                            tmp_total_40temp_max = cur_temp;
                    }
                    #endif
                    #if CAL_TEMP45_SWITCH
                    //房间温度 45℃
                    if(v_temp >= 440 && v_temp < 500)
                    {
                        // 计算功率最大,最小值
                        if(cur_power > tmp_total_45power_max)
                            tmp_total_45power_max = cur_power;
                        if(cur_power < tmp_total_45power_min)
                            tmp_total_45power_min = cur_power;
                        // 温度最大值
                        if(cur_temp > tmp_total_45temp_max)
                            tmp_total_45temp_max = cur_temp;
                    }
                    #endif

                    #if CAL_TEMP50_SWITCH
                    //房间温度 50℃
                    if(v_temp >= 490 && v_temp < 540)
                    {
                        // 计算功率最大,最小值
                        if(cur_power > tmp_total_50power_max)
                            tmp_total_50power_max = cur_power;
                        if(cur_power < tmp_total_50power_min)
                            tmp_total_50power_min = cur_power;
                        // 温度最大值
                        if(cur_temp > tmp_total_50temp_max)
                            tmp_total_50temp_max = cur_temp;
                    }
                    #endif
                    #if CAL_TEMP55_SWITCH
                            //房间温度 55℃
                            if(temp >= 540 && temp < 590)
                            {
                                // 计算功率最大,最小值
                                if(cur_power > tmp_total_55power_max)
                                    tmp_total_55power_max = cur_power;
                                if(cur_power < tmp_total_55power_min)
                                    tmp_total_55power_min = cur_power;
                                // 温度最大值
                                if(cur_temp > tmp_total_55temp_max)
                                    tmp_total_55temp_max = cur_temp;
                            }
                    #endif
                    #if CAL_TEMP60_SWITCH
                            //房间温度 60℃
                            if(temp >= 590)
                            {
                                // 计算功率最大,最小值
                                if(cur_power > tmp_total_60power_max)
                                    tmp_total_60power_max = cur_power;
                                if(cur_power < tmp_total_60power_min)
                                    tmp_total_60power_min = cur_power;
                                // 温度最大值
                                if(cur_temp > tmp_total_60temp_max)
                                    tmp_total_60temp_max = cur_temp;
                            }
                    #endif
                }
            }
        }//j  pv_data

        //老化数据处理
        aging_err_cause tmp_ret = pass_data;
        QJsonObject pv_judge_param = judge_param.value("pv_params").toArray()[0].toObject();
        if(i==0 || i==1)
        {
            if(~(uint)judge_param.value("mim1_err").toInt() & mim_code)
            {
                if(pv_ov_point  > PVOV_ERR_COUNT_MAX  ||pv_un_point  > PVUN_ERR_COUNT_MAX ||
                dcov_point > DCOV_ERR_COUNT_MAX ||dcoa_point > DCOA_ERR_COUNT_MAX||
                mi_island_point > MIIS_ERR_COUNT_MAX ||low_res_point  > LOW_RES_COUNT_MAX ||
                pv_ov_point2 > PVOV2_ERR_COUNT_MAX ||pv_un_point2 > PVUN2_ERR_COUNT_MAX ||
                dc_ov_point2 > DC_OV2_ERR_COUNT_MAX ||dc_oc_point2 > DC_UN2_ERR_COUNT_MAX ||
                sync_err_point > SYNC_ERR_COUNT_MAX ||safe_err_point > SAFE_ERR_COUNT_MAX ||
                grid_point > GRID_ERR_COUNT_MAX ||remote_point > REMOTE_ERR_COUNT_MAX )
                {
                    tmp_ret = errcode_data;
                }
            }
        }
        if(i==2 || i==3)
        {
            if(~(uint)judge_param.value("mim2_err").toInt() & mim_code)
            {
                if(pv_ov_point  > PVOV_ERR_COUNT_MAX  ||pv_un_point  > PVUN_ERR_COUNT_MAX ||
                    dcov_point > DCOV_ERR_COUNT_MAX ||dcoa_point > DCOA_ERR_COUNT_MAX||
                    mi_island_point > MIIS_ERR_COUNT_MAX ||low_res_point  > LOW_RES_COUNT_MAX ||
                    pv_ov_point2 > PVOV2_ERR_COUNT_MAX ||pv_un_point2 > PVUN2_ERR_COUNT_MAX ||
                    dc_ov_point2 > DC_OV2_ERR_COUNT_MAX ||dc_oc_point2 > DC_UN2_ERR_COUNT_MAX ||
                    sync_err_point > SYNC_ERR_COUNT_MAX ||safe_err_point > SAFE_ERR_COUNT_MAX ||
                    grid_point > GRID_ERR_COUNT_MAX ||remote_point > REMOTE_ERR_COUNT_MAX )
                {
                    tmp_ret = errcode_data;
                }
            }
        }
        //副边故障打开，判断每一个故障
        if(~(uint)judge_param.value("mis_err").toInt() & mis_code)
        {
            if(grid_ov_point > GRID_OV_COUNT_MAX || grid_un_point > GRID_UN_COUNT_MAX ||
            grid_ovfre_point  > GRID_OVF_COUNT_MAX  || grid_unfre_point  > GRID_UNF_COUNT_MAX ||
            lock_err_point > LOCK_ERR_COUNT_MAX || grid_surge_point > GRID_SUR_COUNT_MAX ||
            SEC_MCU_point > MCU_UN_COUNT_MAX  || mispv_un_point > PV_UN_COUNT_MAX ||
            H_oc_point > HOC_ERR_COUNT_MAX || H_ov_point > HOV_ERR_COUNT_MAX ||
            grid_dis_point > GRID_DIS_ERR_COUNT_MAX || reserved > RESERVER_ERR_COUNT_MAX ||
            PRI2_point > PRI2CM_ERR_COUNT_MAX || PRI1_point > PRI1CM_ERR_COUNT_MAX ||
            grid_fre_point > GF_ERR_COUNT_MAX || grid_vo_point > GV_ERR_COUNT_MAX )
            {
                tmp_ret = errcode_data;
            }

        }

#if CAL_TEMP40_SWITCH
        if(pv_judge_param.value("v_pv_rm40_pwmin").toInt() != -1)
        {
            if(tmp_40power_min == DEFAULT_VALUE_MIN  ||
               tmp_40power_min < (int)(pv_judge_param.value("v_pv_rm40_pwmin").toInt() * pv_nominal_power))
            {
                tmp_ret = lowpower_data;
            }
        }
#endif
#if CAL_TEMP45_SWITCH
        if(pv_judge_param.value("v_pv_rm45_pwmin").toInt() != -1)
        {
            if(tmp_45power_min == DEFAULT_VALUE_MIN  ||
               tmp_45power_min < (int)(pv_judge_param.value("v_pv_rm45_pwmin").toInt() * pv_nominal_power))
            {
                tmp_ret = lowpower_data;
            }
        }
#endif
#if CAL_TEMP50_SWITCH
        if(pv_judge_param.value("v_pv_rm50_pwmin").toInt() != -1)
        {
            if(tmp_50power_min == DEFAULT_VALUE_MIN  ||
               tmp_50power_min < (int)(pv_judge_param.value("v_pv_rm50_pwmin").toInt() * pv_nominal_power))
            {
                tmp_ret = lowpower_data;
            }
        }
#endif
#if CAL_TEMP55_SWITCH
        if(pv_judge_param.value("v_pv_rm55_pwmin").toInt() != -1)
        {
            if(tmp_55power_min == DEFAULT_VALUE_MIN  ||
               tmp_55power_min < (int)(pv_judge_param.value("v_pv_rm55_pwmin").toInt() * nominal_power))
            {
                tmp_ret = lowpower_data;
            }
        }

#endif
#if CAL_TEMP60_SWITCH
        if(pv_judge_param.value("v_pv_rm60_pwmin").toInt() != -1)
        {
            if(tmp_60power_min == DEFAULT_VALUE_MIN  ||
               tmp_60power_min < (int)(pv_judge_param.value("v_pv_rm60_pwmin").toInt() * nominal_power))
            {
                tmp_ret = lowpower_data;
            }
        }

#endif
#if CAL_TEMP40_SWITCH
        if(pv_judge_param.value("v_pv_rm40_tmax").toInt() != -1)
        {
            if(tmp_temp_max == DEFAULT_VALUE_MAX ||
               tmp_temp_max > (int)(pv_judge_param.value("v_pv_rm40_tmax").toInt() * 100))
            {
                tmp_ret = uptemp_data;
            }
        }
#endif
#if CAL_TEMP45_SWITCH
        if(pv_judge_param.value("v_pv_rm45_tmax").toInt() != -1)
        {
            if(tmp_temp_max == DEFAULT_VALUE_MAX ||
               tmp_temp_max > (int)(pv_judge_param.value("v_pv_rm45_tmax").toInt() * 100))
            {
                tmp_ret = uptemp_data;
            }
        }
#endif
#if CAL_TEMP50_SWITCH
        if(pv_judge_param.value("v_pv_rm50_tmax").toInt() != -1)
        {
            if(tmp_temp_max == DEFAULT_VALUE_MAX ||
               tmp_temp_max > (int)(pv_judge_param.value("v_pv_rm50_tmax").toInt() * 100))
            {
                tmp_ret = uptemp_data;
            }
        }
#endif
#if CAL_TEMP55_SWITCH
        if(pv_judge_param.value("v_pv_rm55_tmax").toInt() != -1)
        {
            if(tmp_temp_max == DEFAULT_VALUE_MAX ||
               tmp_temp_max > (int)(pv_judge_param.value("v_pv_rm55_tmax").toInt() * 100))
            {
                tmp_ret = uptemp_data;
            }
        }
#endif
#if CAL_TEMP60_SWITCH
        if(pv_judge_param.value("v_pv_rm60_tmax").toInt() != -1)
        {
            if(tmp_temp_max == DEFAULT_VALUE_MAX ||
               tmp_temp_max > (int)(pv_judge_param.value("v_pv_rm60_tmax").toInt() * 100))
            {
                tmp_ret = uptemp_data;
            }
        }
#endif
        if(ignore_times > ignore_points_thr)
        {
            tmp_ret = ignore_max_data;
        }
        if(drop_times > drop_points_thr)
        {
            tmp_ret = drop_max_data;
        }

        if(pv_size == 4)
        {
            //pv 最高功率没超过标称功率
            if(tmp_power_max < 95 * pv_nominal_power)
            {
                tmp_ret = nonominal_power;
            }
        }
        else
        {
            //pv 最高功率没超过标称功率
            if(tmp_power_max < 100 * pv_nominal_power)
            {
                tmp_ret = nonominal_power;
            }
        }

        if(tmp_power_max == DEFAULT_VALUE_MAX && tmp_power_min == DEFAULT_VALUE_MIN)
        {
            tmp_power_max = 0;
            tmp_power_min = 0;
#if CAL_TEMP40_SWITCH
        tmp_40power_max = 0;
        tmp_40power_min = 0;
        tmp_40temp_max = 0;
#endif
#if CAL_TEMP45_SWITCH
        tmp_45power_max = 0;
        tmp_45power_min = 0;
        tmp_45temp_max = 0;
#endif
#if CAL_TEMP50_SWITCH
        tmp_50power_max = 0;
        tmp_50power_min = 0;
        tmp_50temp_max = 0;
#endif
#if CAL_TEMP55_SWITCH
        tmp_55power_max = 0;
        tmp_55power_min = 0;
        tmp_55temp_max = 0;
#endif
#if CAL_TEMP60_SWITCH
        tmp_60power_max = 0;
        tmp_60power_min = 0;
        tmp_60temp_max = 0;
#endif

            tmp_ret = nopower_data;
        }

        int a1 = pv_datas.size(); //实际值
        int aging_points_now = age_time /2 ; //标准值

        // 计算实际点数占阈值的百分比 (实际值/标准值*100)
        double percen_points = (double)a1 / aging_points_now * 100.0;
        if(judge_param.value("v_aging_points").toInt() != -1)
        {
            if(percen_points < agingPoints)
            {
                tmp_ret = missing_point_data;
            }
        }

        // if(judge_param.value("v_aging_points").toInt() != -1)
        // {
        //     if(pv_datas.size() < judge_param.value("v_aging_points").toInt())
        //     {
        //         tmp_ret = missing_point_data;
        //     }
        // }

        start_time = s_data.value("start_time").toString();
        if(judge_param.value("v_aging_time").toInt() != -1)
        {
            QDateTime start_time_dt  =  QDateTime::fromString(start_time,"yyyy-MM-dd hh:mm:ss");
            QDateTime stop_time_dt = QDateTime::fromString(stop_time,"yyyy-MM-dd hh:mm:ss");

            qint64 min = start_time_dt.secsTo(stop_time_dt);
            int d = min / 60; //老化的分钟数

            // 实际老化时长/规定老化的时间 *100.0
            double percent_time = (double)d / age_time * 100.0;

            if(percent_time < agingTime)
            {
                tmp_ret = missing_time_data;
            }
        }

        // if(judge_param.value("v_aging_time").toInt() != -1)
        // {
        //     QDateTime start_time_dt  =  QDateTime::fromString(start_time,"yyyy-MM-dd hh:mm:ss");
        //     QDateTime stop_time_dt = QDateTime::fromString(stop_time,"yyyy-MM-dd hh:mm:ss");

        //     //int c = start_time_dt.secsTo(stop_time_dt)/60;

        //     if((start_time_dt.secsTo(stop_time_dt)/60) < judge_param.value("v_aging_time").toInt())
        //     {
        //         tmp_ret = missing_time_data;
        //     }
        // }

        if(pv_datas.size() == 0)
        {
            tmp_power_max = DEFAULT_VALUE_MAX;
            tmp_power_min = DEFAULT_VALUE_MIN;

#if CAL_TEMP40_SWITCH
        tmp_40power_max = DEFAULT_VALUE_MAX;
        tmp_40power_min = DEFAULT_VALUE_MIN;
        tmp_40temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP45_SWITCH
        tmp_45power_max = DEFAULT_VALUE_MAX;
        tmp_45power_min = DEFAULT_VALUE_MIN;
        tmp_45temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP50_SWITCH
        tmp_50power_max = DEFAULT_VALUE_MAX;
        tmp_50power_min = DEFAULT_VALUE_MIN;
        tmp_50temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP55_SWITCH
        tmp_55power_max = DEFAULT_VALUE_MAX;
        tmp_55power_min = DEFAULT_VALUE_MIN;
        tmp_55temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP60_SWITCH
        tmp_60power_max = DEFAULT_VALUE_MAX;
        tmp_60power_min = DEFAULT_VALUE_MIN;
        tmp_60temp_max = DEFAULT_VALUE_MAX;
#endif

            tmp_ret = empty_data;
        }

        //当前错误严重等级(实际倒序,越大越小) 小于 实际错误等级
        if(cur_err_cause >= tmp_ret )
        {
            cur_err_cause = tmp_ret;
        }
        //生成pv老化结论文本解析
        pv_ret_str = get_aging_err_cause_str(tmp_ret);

        //生成pv_jsonobj
        QJsonObject pv_obj;
        pv_obj.insert("pv_id",i+1);
        pv_obj.insert("run_points",pv_datas.size());
        pv_obj.insert("start_power",start_power);
        pv_obj.insert("dcov_points",dcov_point);
        pv_obj.insert("dcoa_points",dcoa_point);
        pv_obj.insert("pw_max",tmp_power_max);
        pv_obj.insert("pw_min",tmp_power_min);
        pv_obj.insert("tmax",tmp_temp_max);
#if CAL_TEMP40_SWITCH
        pv_obj.insert("rm40_pwmax",tmp_40power_max);
        pv_obj.insert("rm40_pwmin",tmp_40power_min);
        pv_obj.insert("rm40_tmax",tmp_40temp_max);
#endif
#if CAL_TEMP45_SWITCH
        pv_obj.insert("rm45_pwmax",tmp_45power_max);
        pv_obj.insert("rm45_pwmin",tmp_45power_min);
        pv_obj.insert("rm45_tmax",tmp_45temp_max);
#endif
#if CAL_TEMP50_SWITCH
        pv_obj.insert("rm50_pwmax",tmp_50power_max);
        pv_obj.insert("rm50_pwmin",tmp_50power_min);
        pv_obj.insert("rm50_tmax",tmp_50temp_max);
#endif
#if CAL_TEMP55_SWITCH
        pv_obj.insert("rm55_pwmax",tmp_55power_max);
        pv_obj.insert("rm55_pwmin",tmp_55power_min);
        pv_obj.insert("rm55_tmax",tmp_55temp_max);
#endif
#if CAL_TEMP60_SWITCH
        pv_obj.insert("rm60_pwmax",tmp_60power_max);
        pv_obj.insert("rm60_pwmin",tmp_60power_min);
        pv_obj.insert("rm60_tmax",tmp_60temp_max);
#endif
        //跳过温度统计的点数
        pv_obj.insert("ignore_times",ignore_times);
        //跌落次数
        pv_obj.insert("drop_times",drop_times);
        pv_obj.insert("mim_err",mim_code);
        pv_obj.insert("mis_err",mis_code);
        pv_obj.insert("pv_ret",pv_ret_str);
        pv_ret.append(pv_obj);
    }// i

    //整机只判断部分情况
    aging_err_cause tmp_total_casue = pass_data;
#if CAL_TEMP40_SWITCH
    if(tmp_total_40power_min < (int)(judge_param.value("v_rm40_pwmin").toInt() * total_nominal_power))
        tmp_total_casue = lowpower_data;
#endif
#if CAL_TEMP45_SWITCH
    if(tmp_total_45power_min < (int)(judge_param.value("v_rm45_pwmin").toInt() * total_nominal_power))
        tmp_total_casue = lowpower_data;
#endif
#if CAL_TEMP50_SWITCH
    if(tmp_total_50power_min < (int)(judge_param.value("v_rm50_pwmin").toInt() * total_nominal_power))
        tmp_total_casue = lowpower_data;
#endif
#if CAL_TEMP55_SWITCH
    if(tmp_total_55power_min < (int)(judge_param.value("v_rm55_pwmin").toInt() * nominal_power))
        tmp_total_casue = lowpower_data;
#endif
#if CAL_TEMP60_SWITCH
    if(tmp_total_60power_min < (int)(judge_param.value("v_rm60_pwmin").toInt() * nominal_power))
        tmp_total_casue = lowpower_data;
#endif
    if(pv_size == 4)
    {
        //整机功率过低
        if(tmp_total_power_max < total_nominal_power *99)
            tmp_total_casue = nonominal_power;
    }
    else
    {
        //整机功率过低
        if(tmp_total_power_max < total_nominal_power *100)
            tmp_total_casue = nonominal_power;
    }

    if(tmp_total_power_max == DEFAULT_VALUE_MAX && tmp_total_power_min == DEFAULT_VALUE_MIN)
    {
        tmp_total_power_max = 0;
        tmp_total_power_min = 0;
#if CAL_TEMP40_SWITCH
    tmp_total_40power_max = 0;
    tmp_total_40power_min = 0;
    tmp_total_40temp_max = 0;
#endif
#if CAL_TEMP45_SWITCH
    tmp_total_45power_max = 0;
    tmp_total_45power_min = 0;
    tmp_total_45temp_max = 0;
#endif
#if CAL_TEMP50_SWITCH
    tmp_total_50power_max = 0;
    tmp_total_50power_min = 0;
    tmp_total_50temp_max = 0;
#endif
#if CAL_TEMP55_SWITCH
    tmp_total_55power_max = 0;
    tmp_total_55power_min =0 ;
    tmp_total_55temp_max = 0;
#endif
#if CAL_TEMP60_SWITCH
    tmp_total_60power_max = 0;
    tmp_total_60power_min = 0;
    tmp_total_60temp_max = 0;
#endif
        tmp_total_casue = nopower_data;
    }

    if(all_data_size == 0)
    {
        tmp_total_power_max = DEFAULT_VALUE_MAX;
        tmp_total_power_min = DEFAULT_VALUE_MIN;
#if CAL_TEMP40_SWITCH
    tmp_total_40power_max = DEFAULT_VALUE_MAX;
    tmp_total_40power_min = DEFAULT_VALUE_MIN;
    tmp_total_40temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP45_SWITCH
    tmp_total_45power_max = DEFAULT_VALUE_MAX;
    tmp_total_45power_min = DEFAULT_VALUE_MIN;
    tmp_total_45temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP50_SWITCH
    tmp_total_50power_max = DEFAULT_VALUE_MAX;
    tmp_total_50power_min = DEFAULT_VALUE_MIN;
    tmp_total_50temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP55_SWITCH
    tmp_total_55power_max = DEFAULT_VALUE_MAX;
    tmp_total_55power_min = DEFAULT_VALUE_MIN ;
    tmp_total_55temp_max = DEFAULT_VALUE_MAX;
#endif
#if CAL_TEMP60_SWITCH
    tmp_total_60power_max = DEFAULT_VALUE_MAX;
    tmp_total_60power_min = DEFAULT_VALUE_MIN;
    tmp_total_60temp_max = DEFAULT_VALUE_MAX;
#endif


        tmp_total_casue = empty_data;

    }

    if(cur_err_cause > tmp_total_casue )
    {
        cur_err_cause = tmp_total_casue;
    }

    total_ret_str = get_aging_err_cause_str(tmp_total_casue);
    if(cur_err_cause == pass_data)
    {
        b_ret = 0;
    }
    else if(cur_err_cause < pass_data && cur_err_cause >= nonominal_power)
    {
        b_ret = 1;
    }
    else if(cur_err_cause < nonominal_power)
    {
        b_ret = 2;
    }



    QJsonObject total_obj;

    total_obj.insert("start_power",total_start_power);
    total_obj.insert("pw_max",tmp_total_power_max);
    total_obj.insert("pw_min",tmp_total_power_min);
    total_obj.insert("tmax",tmp_total_temp_max);
    total_obj.insert("total_ret",total_ret_str);
#if CAL_TEMP40_SWITCH
    total_obj.insert("rm40_pwmax",tmp_total_40power_max);
    total_obj.insert("rm40_pwmin",tmp_total_40power_min);
    total_obj.insert("rm40_tmax",tmp_total_40temp_max);
#endif
#if CAL_TEMP45_SWITCH
    total_obj.insert("rm45_pwmax",tmp_total_45power_max);
    total_obj.insert("rm45_pwmin",tmp_total_45power_min);
    total_obj.insert("rm45_tmax",tmp_total_45temp_max);
#endif
#if CAL_TEMP50_SWITCH
    total_obj.insert("rm50_pwmax",tmp_total_50power_max);
    total_obj.insert("rm50_pwmin",tmp_total_50power_min);
    total_obj.insert("rm50_tmax",tmp_total_50temp_max);
#endif
#if CAL_TEMP55_SWITCH
    total_obj.insert("rm55_pwmax",tmp_total_55power_max);
    total_obj.insert("rm55_pwmin",tmp_total_55power_min);
    total_obj.insert("rm55_tmax",tmp_total_55temp_max);
#endif
#if CAL_TEMP60_SWITCH
    total_obj.insert("rm60_pwmax",tmp_total_60power_max);
    total_obj.insert("rm60_pwmin",tmp_total_60power_min);
    total_obj.insert("rm60_tmax",tmp_total_60temp_max);
#endif

    r_data.insert("pv_data",pv_ret);
    r_data.insert("total_data",total_obj);

    r_data.insert("mi_cid",s_data.value("mi_cid").toString());
    r_data.insert("all_pv",s_data.value("all_pv").toString());
    r_data.insert("start_time",s_data.value("start_time").toString());
    //r_data.insert("stop_time",s_data.value("stop_time").toString());
    r_data.insert("stop_time",total_stop_time);

    r_data.insert("pv_nominal_power",pv_nominal_power);
    r_data.insert("total_nominal_power",total_nominal_power);
    r_data.insert("aging_points",mi_point_size);

    //版本号  已经取到值
    r_data.insert("mim_version",s_data.value("mim_version").toString());
    r_data.insert("mis_version",s_data.value("mis_version").toString());

    r_data.insert("b_ret",b_ret);

}

QString aging_alg::get_aging_err_cause_str(aging_err_cause err_cause)
{
    QString str = "";
    switch (err_cause) {
    case errcode_data:
        str.append("错误代码异常");
        break;
    case uptemp_data:
        str.append("温度过高");
        break;
    case lowpower_data:
        str.append("老化功率低");
        break;
    case ignore_max_data:
        str.append("异常忽略点数过多");
        break;
    case drop_max_data:
        str.append("异常跌落次数过多");
        break;
    case nonominal_power:
        str.append("最高功率没达到标称功率");
        break;
    case nopower_data:
        str.append("无功率输出");
        break;
    case missing_point_data:
        str.append("老化数据量不足");
        break;
    case missing_time_data:
        str.append("老化时长不够");
        break;
    case empty_data:
        str.append("没有数据上传");
        break;
    case pass_data:
        str.append("通过");
        break;
    default:
        break;
    }
    return str;

}
