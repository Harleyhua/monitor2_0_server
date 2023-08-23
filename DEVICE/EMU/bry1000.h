#ifndef BRY1000_H
#define BRY1000_H

#include "abstract_emu.h"

class  emu_protocolb;

class bry1000 :public abstract_emu
{
    Q_OBJECT
public:
    bry1000(QString name,QObject *parent = nullptr)
    :abstract_emu(name,emu_type::bry1000_type,parent)
    {

    };
    ~bry1000(){

    };


    void onm_deal_msg(QByteArray data) override;
private:
    void deal_frame(emu_protocolb &tmp_pcol,QByteArray &frame_data,quint8 frame_cmd);
};

#endif // BRY1000_H
