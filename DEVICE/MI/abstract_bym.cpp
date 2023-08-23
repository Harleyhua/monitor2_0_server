#include "abstract_bym.h"



void abstract_bym::set_property(QString nominal, QString mim_ver, QString mis_ver)
{
    m_nominal = nominal;
    m_mim_version = mim_ver;
    m_mis_version = mis_ver;

}

bool abstract_bym::is_cid_valid(QString mi_cid)
{
    if(mi_cid =="")
    {
        return false;
    }

    //16进制数 8位
    QRegExp hexMatcher("^[0-9A-F]{8}$");
    if(hexMatcher.exactMatch(mi_cid) && (mi_cid[0] == '1' || mi_cid[0] == '2' ||
            mi_cid[0] == '4' || mi_cid[0] == '8' ))
        return true;
    return false;
}

bym_type abstract_bym::analysis_type(QString name)
{
    if(is_cid_valid(name))
    {
        if(name.startsWith("1"))
        {
            return bym_one_type;
        }
        else if(name.startsWith("2"))
        {
            return bym_two_type;
        }
        else if(name.startsWith("4"))
        {
            return bym_four_type;
        }
        else if(name.startsWith("8"))
        {
            return bym_eight_type;
        }
    }

    return bym_empty_type;
}


