#ifndef AGING_ALG_H
#define AGING_ALG_H
#include <QObject>
#include <QJsonObject>

/* 源数据 s_datas
{
    "mi_cid":"10010085",
    "all_pv":"1"
    "nominal_power":600,
    "start_time":"2023-07-13 15:26:49",
    "stop_time":"2023-08-13 15:26:49",
    "datas":[
        {
            "pv_id":1,
            "pv_datas":[
            {
                "emu_cid": "90999911",
                "emu_time": "2023-06-28 17:54:53",
                "energy": "58.95",
                "gridf": "60.01",
                "gridv": "231.03",
                "mi_cid": "10000002",
                "mim_err": "0000",
                "mim_version": "12.1",
                "mis_err": "0000",
                "mis_version": "1.7",
                "nominal_power": 0,
                "power": "603.13",
                "pv": "38.75",
                "pv_id": 1,
                "reissue_data": "NO",
                "sys_time": "2023-07-13 16:24:21",
                "temperature": "79.00"
            }



            ]
        },

        {


        }
    ]
}

/* judge_param 1.0
{
    "version_1.0",
    "v_aging_points":200,
    "v_aging_time":120,
    "v_start_point_offset":2,

    "all_pv":4,
    "v_rm40_pwmin": 91,
    "v_rm40_tmax": 85,
    "v_rm45_pwmin": 90,
    "v_rm45_tmax": 80,
    "v_rm50_pwmin": 75,
    "v_rm50_tmax": 90,
    "v_rm55_pwmin": 80,
    "v_rm55_tmax": 80,
    "v_rm60_pwmin": 0,
    "v_rm60_tmax": 80,
    "pv_params":[
        {
            "v_pv_rm40_pwmin": 91,
            "v_pv_rm40_tmax": 85,

            "v_pv_rm45_pwmin": 90,
            "v_pv_rm45_tmax": 80,

            "v_pv_rm50_pwmin": 70,
            "v_pv_rm50_tmax": 90,

            "v_pv_rm55_pwmin": 80,
            "v_pv_rm55_tmax": 80,

            "v_pv_rm60_pwmin": 0,
            "v_pv_rm60_tmax": 80
        }
    ]
    "mim1_err":65535,
    "mim2_err":65535,
    "mis_err":65479
}



*/

#define CAL_TEMP40_SWITCH 1
#define CAL_TEMP45_SWITCH 0
#define CAL_TEMP50_SWITCH 1
#define CAL_TEMP55_SWITCH 0
#define CAL_TEMP60_SWITCH 0

#define CAL_FILTER_SWITCH 1 //开启滤波
#define FILTER_POINT      3 //滤波计算点
#define FILTER_THRESHOLD  85 //滤波阈值 %
#define ERR_IGNORE_POINTS 1  //发生故障时 后续忽略的点数

//#define ERR_IGNORE_POINTS_MAX
//#define ERR_DROP_POINTS_MAX


#define DEFAULT_VALUE_MIN (400000)
#define DEFAULT_VALUE_MAX (-400000)
#define DCOV_ERR_COUNT_MAX 10
#define DCOA_ERR_COUNT_MAX 10
enum aging_err_cause{
    errcode_data,     //错误代码
    lowpower_data,    //功率低
    uptemp_data,      //温度过高
    ignore_max_data,  //频繁异常 忽略点数过多 故障过多
    drop_max_data,    //功率跌落次数过多
    nonominal_power,  //功率没达到标称功率上
    nopower_data,     //无功率输出
    missing_point_data, //老化采集点数丢失
    missing_time_data,  //老化时间不充分
    empty_data,       //无数据
    pass_data         //数据通过
};


class aging_alg
{
public:
    aging_alg();

    void aging_report(QJsonObject s_data,QJsonObject judge_param,QHash<QString,uint16_t> &temp,QJsonObject &r_data);


    QString get_aging_err_cause_str(aging_err_cause err_cause);
};

#endif // AGING_ALG_H
