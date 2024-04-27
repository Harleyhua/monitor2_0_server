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

    bool writeUserAlgorithm(QString account,const QJsonObject &s);    //写用户老化算法
private:
    QSqlDatabase mDataBase;
signals:

};

#endif // USERSERVICE_H
