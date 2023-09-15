#ifndef POWER_SQL_H
#define POWER_SQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>

#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QMap>

#include "ag_mi_report_table.h"
#include "ag_device_control_table.h"
#define POWER_DATA_TABLE_MAX_MI   5000
#define RACK_DATA_TABLE_MAX_MI    5000


enum sql_type {MYSQL};


typedef struct
{
    QString hostname;
    uint16_t port;
    QString username;
    QString password;
    QString database_name;
}mysql_login_stc;  //数据库登录参数


class mysql : public QObject
{
    Q_OBJECT
public:
    explicit mysql(QString db_name,QObject *parent = nullptr);
    ~mysql();
    //各个表格初始化
    bool table_init();
    //底层设备 访问的数据库接口
    void w_emu_action(QJsonObject &s_data);
    void w_login(QJsonObject &s_data);
    void w_emu_property(QJsonObject &s_data);
    void w_handshake(QJsonObject &s_data);
    void w_mi_property(QJsonObject &s_data);
    void r_mi_property(QJsonObject &s_data,QJsonObject &rt_data);
    void w_power(QJsonObject &s_data,uint16_t date);
    void r_emu_mapping(QJsonObject &s_data,QJsonObject &rt_data);
    void w_emu_status(QJsonObject &s_data);
    //温度采集器
    void w_room_temp(QJsonObject &s_data);
    //room-1 老化架数据
    void w_rack_data(QJsonObject &s_data);

    void w_rk_data(QJsonObject &s_data);
    //客户端访问业务
    void r_user_property(QString name, QJsonObject &rt_data);
    void r_user_property(QJsonObject &s_data,QJsonObject &rt_data);
    void r_emu_action(QJsonObject &s_data,QJsonObject &rt_data);
    void r_emu_status(QJsonObject &s_data,QJsonObject &rt_data);
    void r_mi_rack_data(QJsonObject &s_data,QJsonObject &rt_data);
    void r_mapping(QString account,QJsonObject &rt_data,QStringList &mis_list);
    void r_mi_current_power(QStringList mi_list,QJsonObject &rt_data);
    void r_mi_power(QJsonObject &s_data,QJsonObject &rt_data,bool is_one = false);
    void r_temp(QJsonObject &s_data,QJsonObject &rt_data);
    void r_temp(QString room_name,QString start_time,QString stop_time,QHash<QString, uint16_t> &temp);
    void r_workorder(QJsonObject &s_data,QJsonObject &rt_data);
    void w_workorder(QJsonObject &s_data);
    QString r_total_station(QString account);
    void w_user_act(QString user,QString total_sta,int act,QString desc);
    void w_mi_report(QString mi,QString start_time,QString stop_time,QString alg,QString report);
    bool update_alg(QString account,QJsonObject alg_data);
    void r_batch_list(QJsonObject &s_data,QJsonObject &rt_data);
    void r_user_act(QJsonObject &s_data,QJsonObject &rt_data);
    void r_emu_property(QString emu_cid,QJsonObject &emu_pro_obj);
    bool update_dev_control(QString total_station,QString station,QString emu,quint8 server_cmd,QString send_data);


    //远程控制相关
    void w_mi_temporary_power(QString name,QString data);
    bool r_mi_temporary_power(QString name,QString &data);

    void w_mi_max_power(QString name,QString data);
    bool r_mi_max_power(QString name,QString &data);

    void w_mi_grid(QString name,QString data);
    bool r_mi_grid(QString name,QString &data);

    void w_mi_certification(QString name,QString data);
    bool r_mi_certification(QString name,QString &data);

    void w_emu_func_code(QString name,QString data);
    bool r_emu_func_code(QString name,QString &data);
    //老化相关
    //根据微逆编号获取最后一次老化的参数
    void r_mi_last_aging_param(QString mi,QString &room, QString &start_time, QString &stop_time,QString &pos_desc);
    //找到完整的老化时间段  根据  房间名和批次
    void r_aging_batch_by_room_start_time(QString mi,QString &room,QString start_time,QString &stop_time,QString &pos_desc);

    bool r_user_judge_params(QString user,QJsonObject &rt_data);
    //根据编号和开始时间和结束时间  获取老化数据
    void r_aging_data_by_mi_date(QString mi_cid,QString st_date,QString sp_date,QJsonObject &rt_data);
    bool r_mi_report(QString mi,QString start_time,QString stop_time,QString alg,QString &report);

    //读取老化时长
    void r_aging_time(QString mi_cid,QString start_time,int &aging_time);


    //根据编号获取老化数据  相当与最后一次老化数据
    //void r_aging_data_by_mis(QString mi,QString &room,QString &pos_desc,QJsonObject &rt_data);




    //层级关系操作方法
    void add_station(QString user,QJsonObject &s_data);
    void add_station(QString user,QString station);
    void add_emu_cid(QString user,QJsonObject &s_data);
    void add_plcmi_cid(QString user,QJsonObject &s_data);
    void add_wifimi_cid(QString user,QJsonObject &s_data);
    void add_wifimi_cid(QString user,QString station, QString emu_cid,QString mi_cid,QString mi_desc);
    void del_station(QString user,QJsonObject &s_data);
    void del_station(QString user,QString station);
    //void del_station();
    void del_emu(QString user,QString station,QString emu);
    void del_emu(QString user,QJsonObject &s_data);
    void del_mi(QString user,QJsonObject &s_data);


    //bool w_device_ctl(QString user,QJsonObject &s_data);
    void r_device_ctl_first_cmd_nosend(QString emu_cid,dev_ctl_strc &ctl_data);
    void r_device_ctl_last_data_nosend(QString emu_cid,dev_ctl_strc &ctl_data);
    void update_device_cmd_send_flag(dev_ctl_strc ctl_data);
    void update_device_data_send_flag(dev_ctl_strc ctl_data);
private:
    QString db_name; //用什么链接名称
    QSqlDatabase m_db; //数据库
    //切换当前的数据库
    bool set_current_database(QString database_name);
    bool create_database(QString database_name);   //创建数据库
    bool delete_database(QString database_name);   //删除数据库
};

#endif // POWER_SQL_H
