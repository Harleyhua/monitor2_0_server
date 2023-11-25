#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <QObject>
#include <QMutex>



//#include "common.h"

//数据表
#include "mysql.h"



class user_service : public QObject
{
    Q_OBJECT
public:
    explicit user_service(QString name,QObject *parent = nullptr);

    QString getTotalStation(QString user_name);

    bool getUserProperty(const QJsonObject &param,QJsonObject &ret);
    bool getUserOperationRecord(QString userName,const QJsonObject &param,QJsonObject &ret);
    bool setUserAlgorithm(QString userName,const QJsonObject &param,QJsonObject &ret);

    bool setEmuProperty(QString userName,QJsonObject &param);

    bool setMiTemporaryPower(QString userName,const QJsonObject &param,QJsonObject &ret);
    bool getMiTemporaryPower(QString userName,const QJsonObject &param,QJsonObject &ret);
    bool setMiMaxPower(QString userName,const QJsonObject &param,QJsonObject &ret);
    bool getMiMaxPower(QString userName,const QJsonObject &param,QJsonObject &ret);
    bool setMiGridParam(QString userName,const QJsonObject &param,QJsonObject &ret);
    bool getMiGridParam(QString userName,const QJsonObject &param,QJsonObject &ret);
    bool setMiCertification(QString userName,const QJsonObject &param,QJsonObject &ret);
    bool getMiCertification(QString userName,const QJsonObject &param,QJsonObject &ret);

    bool getControlResultBack(const QJsonObject &param,QJsonObject &ret);

    bool getEmuAction(const QJsonObject &param,QJsonObject &ret);
    bool getEmuStatus(const QJsonObject &param,QJsonObject &ret);

    bool getRackData(const QJsonObject &param,QJsonObject &ret);

    bool getMIPower(const QJsonObject &param,bool is_one,QJsonObject &ret);
    bool getRoomTemp(const QJsonObject &param,QJsonObject &ret);
    bool getWorkOrder(const QJsonObject &param,QJsonObject &ret);
    bool setWorkOrder(const QJsonObject &param);

    bool addStation(QString userName,const QJsonObject &param);
    bool delStation(QString userName,const QJsonObject &param);

    bool addEmu(QString userName,const QJsonObject &param);
    bool delEmu(QString userName,const QJsonObject &param);

    bool addPlcMi(QString userName,const QJsonObject &param);
    bool addWiFiMi(QString userName,const QJsonObject &param);
    bool delMi(QString userName,const QJsonObject &param);

    bool getUserDeviceMapperAndLastDat(QString userName,QJsonObject &ret);
    bool getMiAgingReport(const QJsonObject &param,QJsonObject &ret);
    bool getMiAgingBatch(const QJsonObject &param,QJsonObject &ret);
private:
    mysql mSql;



signals:

};

#endif // USER_SERVICE_H
