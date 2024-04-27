#ifndef DEVDATASERVICE_H
#define DEVDATASERVICE_H

#include <QObject>

#include <QSqlDatabase>


class devdataservice : public QObject
{
    Q_OBJECT
public:
    explicit devdataservice(QSqlDatabase &DataBase,QObject *parent = nullptr);

    void readEmuAction(const QJsonObject &s,QJsonObject &retData);
    void readEmuStatus(const QJsonObject &s,QJsonObject &retData);

    void readMiCurrentPowerData(QStringList Mis,QJsonObject &retData);
    void readMiPowerData(const QJsonObject &s,QJsonObject &retData,bool is_one =false);

    void readMiProperty(QString Mi,QJsonObject &retData);//读取微逆属性

    QString readEmuLastActionTime(QString emu);
    void readEmuLastStatus(QString Emu,QJsonObject &retData);

    void writeEmuProperty(const QJsonObject &s,QJsonObject &retData);
    void writeEmuProperty(QString emu,QString countercurrent);
    void readEmuProperty(QString Emu,QJsonObject &retData);

    void writeMiTemporaryPower(QString Mi,QString &temporaryPower);
    void readMiTemporaryPower(QString Mi,QString &temporaryPower);

    void writeMiMaxPower(QString Mi,QString &maxPower);
    void readMiMaxPower(QString Mi,QString &maxPower);

    void writeMiGrid(QString Mi,QString &grid);
    void readMiGrid(QString Mi,QString &grid);

    void writeMiCertification(QString Mi,QString &certification);
    void readMiCertification(QString Mi,QString &certification);
private:
    QSqlDatabase mDataBase;
signals:

};

#endif // DEVDATASERVICE_H
