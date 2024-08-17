#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

#include <QHash>
#include "common.h"
#include "mysql.h"
#include "sqlconnectpool.h"
#include <QSqlDatabase>
using namespace stefanfrings;
/**
  账户管理
*/
class usercontroller : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(usercontroller)
public:
    usercontroller(QSqlDatabase &DataBase){
        mDataBase = DataBase;
    };
    ~usercontroller(){
    };
    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

private:
    QSqlDatabase mDataBase; //数据库
    HttpRequest *mRequest = nullptr;
    HttpResponse *mResponse = nullptr;

    QString mName;
    QString Per;
    QJsonObject mJsBody;

    QHash<QString, void (usercontroller::*)()> mMethodMapper=
    {
        {"/r_user_property",&usercontroller::getUserProperty},
        {"/r_user_act",&usercontroller::getUserOperationRecord},
        {"/update_alg",&usercontroller::setUserAlgorithm}
    };
    //获取账户属性
    void getUserProperty();
    //读取用户操作记录
    void getUserOperationRecord();
    //设置用户的老化算法配置
    void setUserAlgorithm();
};

#endif // USERCONTROLLER_H
