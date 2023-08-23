#ifndef DATA_CACHE_H
#define DATA_CACHE_H

#include <QObject>
#include <QQueue>
#include "devices/mi.h"
#include "data/sql/mysql.h"
class data_cache : public QObject
{
    Q_OBJECT
public:
    explicit data_cache(QObject *parent = nullptr);
    ~data_cache();
    QVector<mi_pv_data> * m_mi_data_cache;

    void write_mi_data_cache(mi_pv_data);
    void write_mi_data_cache(const QVector<mi_pv_data> &data);
    QVector<mi_pv_data> read_mi_data_cache(QString cache_name);


signals:

};

#endif // DATA_CACHE_H
