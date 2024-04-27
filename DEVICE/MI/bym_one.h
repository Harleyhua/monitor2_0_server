#ifndef BYM_ONE_H
#define BYM_ONE_H

#include <QObject>

#include "abstract_bym.h"
class bym_one : public abstract_bym
{
    Q_OBJECT
public:
    explicit bym_one(QString name,QObject *parent = nullptr)
        :abstract_bym(name,bym_type::bym_one_type,parent)
    {

    };
    ~bym_one()
    {

    };

    void analysis_mim(quint8 pv_id,QString mim_err) override;
    void analysis_mis(quint8 pv_id,QString mis_err) override;

signals:

};

#endif // BYM_ONE_H
