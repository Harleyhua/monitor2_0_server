#include "userservice.h"
#include "ag_user_table.h"
#include "mysql.h"
userservice::userservice(QSqlDatabase &DataBase,QObject *parent)
    : QObject{parent}
{
    mDataBase = DataBase;
}

bool userservice::checkAccountValid(QString account, QString password)
{
    QJsonObject userJsObj;
    QString tmpPassword;
    ag_user_table userTable;
    QJsonObject user_js;

    if(password == "")
        return false;

    user_js.insert(ag_user_table::c_field_account,account);
    userTable.read_account(mDataBase,user_js,userJsObj);

    tmpPassword = userJsObj.value("datas").toObject().value(ag_user_table::c_field_password).toString();

    if(password != tmpPassword)
    {
        return false;
    }
    return true;
}

void userservice::readUserInfo(const QJsonObject &s, QJsonObject &rtData)
{
    ag_user_table userTable;
    userTable.read_account(mDataBase,s,rtData);
}

//查找实时数据函数
QString userservice::readUserTotalStation(QString account)
{
    ag_user_table userTable;
    QString totalStation;

    userTable.read_total_stations(mDataBase,account,totalStation);

    return totalStation;
}

bool userservice::writeUserAlgorithm(QString account, const QJsonObject &s)
{
    ag_user_table userTable;

    return userTable.update_algorithm(mDataBase,account,s);
}




