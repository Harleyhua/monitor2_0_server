#ifndef BASIC_EMU_FACTORY_H
#define BASIC_EMU_FACTORY_H
#include <QObject>

#include "abstract_emu.h"

class basic_emu_factory:public QObject
{
    Q_OBJECT
public:
    basic_emu_factory(QObject *parent = nullptr): QObject{parent}{};
    ~basic_emu_factory(){};

    abstract_emu *create_emu(QString name,emu_type type);

};

#endif // BASIC_EMU_FACTORY_H
