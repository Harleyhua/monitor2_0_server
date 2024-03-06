#ifndef LOGSERVICE_H
#define LOGSERVICE_H

#include <QObject>
#include <QSqlDatabase>

class logservice : public QObject
{
    Q_OBJECT
public:
    explicit logservice(QSqlDatabase &DataBase,QObject *parent = nullptr);

    void readUserAction(const QJsonObject &s,QJsonObject &rtData);
    void writeUserAction(const QJsonObject &s);
    void writeUserAction(QString user,QString totalStation, int act,
                         const QString &des,QString time);
private:
    QSqlDatabase mDataBase;
signals:

};

#endif // LOGSERVICE_H
