#ifndef BASIC_BYM_FACTORY_H
#define BASIC_BYM_FACTORY_H

#include <QObject>

#include "abstract_bym.h"

class basic_bym_factory :public QObject
{
    Q_OBJECT
public:
    basic_bym_factory(QObject *parent = nullptr): QObject{parent}{};
    ~basic_bym_factory(){};

    abstract_bym *create_emu(QString name,bym_type type);
};

#endif // BASIC_BYM_FACTORY_H
