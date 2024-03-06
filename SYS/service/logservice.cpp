#include "logservice.h"
#include "mysql.h"
logservice::logservice(QSqlDatabase &DataBase, QObject *parent)
    : QObject{parent}
{
    mDataBase = DataBase;
}

void logservice::readUserAction(const QJsonObject &s, QJsonObject &rtData)
{
    ag_user_act_table us_act_tb;
    us_act_tb.read_datas(mDataBase,s,rtData);
}

void logservice::writeUserAction(const QJsonObject &s)
{
    ag_user_act_table userActTable;

    userActTable.write_datas(mDataBase,s);
}

void logservice::writeUserAction(QString user, QString totalStation, int act, const QString &des, QString time)
{
    ag_user_act_table userActTable;
    QJsonObject paramJsObj;
    paramJsObj.insert("user",user);
    paramJsObj.insert("total_station",totalStation);
    paramJsObj.insert("act",act);
    paramJsObj.insert("act_desc",des);
    paramJsObj.insert("act_time",time);
    userActTable.write_datas(mDataBase,paramJsObj);
}
