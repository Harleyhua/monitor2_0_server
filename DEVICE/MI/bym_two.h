#ifndef BYM_TWO_H
#define BYM_TWO_H
#include <QObject>

#include "abstract_bym.h"
class bym_two :public abstract_bym
{
    Q_OBJECT
public:
    explicit bym_two(QString name,QObject *parent = nullptr)
    :abstract_bym(name,bym_type::bym_one_type,parent)
    {

    };
    ~bym_two()
    {

    };

    void analysis_mim(quint8 pv_id,QString mim_err) override;
    void analysis_mis(quint8 pv_id,QString mis_err) override;

};

#endif // BYM_TWO_H
