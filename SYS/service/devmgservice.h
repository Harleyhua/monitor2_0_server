#ifndef DEVMGSERVICE_H
#define DEVMGSERVICE_H

#include <QObject>
#include <QSqlDatabase>


class devmgservice : public QObject
{
    Q_OBJECT
public:
    explicit devmgservice(QSqlDatabase &DataBase,QObject *parent = nullptr);

    void addDevCtl(QString totalStation,QString station,QString emu,
                   QString ctlTime, quint8 serverCmd, QString sendData);//设备控制

    void getOneControlResuletBack(QString field,const QJsonObject &s,QJsonObject &retData);


    void DevMapping(QString totalStation,QStringList &retMis,QJsonObject &retData);

    void addStation(QString totalStation,const QJsonObject &s);
    void delStation(QString totalStation,const QJsonObject &s);

    void addStation(QString totalStation,QString station);
    void delStation(QString totalStation,QString station);

    void addOneEmu(QString station, QString emuCid, QString desc);
    void addEmu(QString totalStation,const QJsonObject &s);
    void delEmu(QString totalStation,const QJsonObject &s);
    void delEmu(QString totalStation,QString station,QString emu);

    void addOnePlcMi(QString totalStation,QString station, QString emu, QString mi, QString miDesc);
    void addPlcMi(QString totalStation,const QJsonObject &s);
    void addOneWiFiMi(QString totalStation,QString station, QString emu, QString mi, QString miDesc);
    void addWiFiMi(QString totalStation,const QJsonObject &s);
    void delMi(QString totalStation,const QJsonObject &s);
    void delMiFromEmu(QString station,QString totalstation,QString emu);

private:
    QSqlDatabase mDataBase;
signals:

};

#endif // DEVMGSERVICE_H
