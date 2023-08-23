#ifndef MYSQL_TABLE_H
#define MYSQL_TABLE_H

#include <QObject>
#include <QSqlDatabase>
class mysql_table : public QObject
{
    Q_OBJECT
public:
    explicit mysql_table(QObject *parent = nullptr);

    static bool create_table(QSqlDatabase &m_database,QString &table_name,QString &field);
    static bool delete_table(QSqlDatabase &m_database,QString &table_name);  //
signals:

};

#endif // MYSQL_TABLE_H
