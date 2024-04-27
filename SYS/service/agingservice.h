#ifndef AGINGSERVICE_H
#define AGINGSERVICE_H

#include <QObject>
#include <QSqlDatabase>


class agingservice : public QObject
{
    Q_OBJECT
public:
    explicit agingservice(QSqlDatabase &DataBase,QObject *parent = nullptr);

    void readAgingAlgorithm(QString user,QJsonObject &retData);

    void readRoomTemperature(const QJsonObject &s,QJsonObject &retData);
    void writeRoomTemperature(const QJsonObject &s);
    void writeNewRoomTemperature(QString room,bool runstatus,int curtemp,int settemp,QString curtime);


    void readBatchList(const QJsonObject &s,QJsonObject &retData);

    void writeRackData(const QJsonObject &s);
    void readRackData(const QJsonObject &s,QJsonObject &retData);

    void readOrderData(const QJsonObject &s,QJsonObject &retData);
    void writeOrderData(const QJsonObject &s);

    void generateMiAgingReport(int mode,QString Mi,QString startTime,const QJsonObject &algorithm,QJsonObject &retReport);

    void readMiAgingData(QString mi, QString startTime, QString stopTime,QJsonObject &rt_data);
    void writeMiAgingReport(QString mi,QString startTime,QString stopTime,QString alg,QString report);
    bool readMiAgingReport(QString mi,QString startTime,QString stopTime,QString alg,QString &report);


private:
    QSqlDatabase mDataBase;
signals:

};

#endif // AGINGSERVICE_H
