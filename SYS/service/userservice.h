#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QObject>
#include <QJsonObject>
#include <QSqlDatabase>


class userservice : public QObject
{
    Q_OBJECT
public:
    explicit userservice(QSqlDatabase &DataBase,QObject *parent = nullptr);

    bool checkAccountValid(QString account,QString password);   //账户验证
    void readUserInfo(const QJsonObject &s,QJsonObject &rtData);
    QString readUserTotalStation(QString account);

    void readPerssion(QString account,QString permission,const QJsonObject &s);

    bool writeUserAlgorithm(QString account,const QJsonObject &s);    //写用户老化算法
    bool writeUserAlgorithm_new(QString account,const QJsonObject &s);
private:
    QSqlDatabase mDataBase;
signals:

};

#endif // USERSERVICE_H
