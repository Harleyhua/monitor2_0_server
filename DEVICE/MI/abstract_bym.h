#ifndef ABSTRACT_BYM_H
#define ABSTRACT_BYM_H

#include <QObject>
#include <QJsonObject>
class emu_protocolb;
class emu_socket;

enum bym_type
{
    bym_empty_type = 0,
    bym_one_type = 1,    //1路微逆
    bym_two_type = 2,    //2路微逆
    bym_four_type = 4,   //4路微逆
    bym_six_type = 6,    //6路微逆
    bym_eight_type = 8   //8路微逆
};

class abstract_bym : public QObject
{
    Q_OBJECT
public:
    explicit abstract_bym(QString name,bym_type type,QObject *parent = nullptr)
    :QObject(parent)
    {
        m_type = type;
        m_name = name;
    };
    virtual ~abstract_bym(){};

    virtual QString get_name()
    {
        return m_name;
    };
    virtual void analysis_mim(quint8 pv_id,QString mim_err) = 0;
    virtual void analysis_mis(quint8 pv_id,QString mis_err) = 0;

    void set_property(QString nominal,QString mim_ver,QString mis_ver);

    static bool is_cid_valid(QString cid);
    static bym_type analysis_type(QString name);

private:
    bym_type m_type;

    QString m_name = "";

    QString m_nominal = "";
    QString m_mim_version = "";
    QString m_mis_version = "";

    uint32_t m_offline_sec = 0;
signals:


};


#endif // ABSTRACT_BYM_H
