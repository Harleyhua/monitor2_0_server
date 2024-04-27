#ifndef BRY2000_H
#define BRY2000_H

#include "abstract_emu.h"

class  emu_protocolb;

class bry2000 :public abstract_emu
{
    Q_OBJECT
public:
    bry2000(QString name,QObject *parent = nullptr)
    :abstract_emu(name,emu_type::bry2000_type,parent)
    {

    };
    ~bry2000(){

    };
    void onm_deal_msg(QByteArray data) override;
private:
    void deal_frame(emu_protocolb &tmp_pcol,QByteArray &frame_data,quint8 cmd);
};

#endif // BRY2000_H
