#include "data_cache.h"
#include <QSqlQuery>
#include "common.h"

data_cache::data_cache(QObject *parent)
    : QObject{parent}
{
    m_mi_data_cache = new QVector<mi_pv_data>();
}

data_cache::~data_cache()
{
    delete m_mi_data_cache;

}

void data_cache::write_mi_data_cache(mi_pv_data data)
{
    m_mi_data_cache->append(data);
}

void data_cache::write_mi_data_cache(const QVector<mi_pv_data> &data)
{
    for(int i=0;i<data.size();i++)
    {
        write_mi_data_cache(data.at(i));
    }
}

//QVector<mi_pv_data> data_cache::read_mi_data_cache(QString cache_name)
//{

//}


