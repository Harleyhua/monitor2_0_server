#include "basic_emu_factory.h"

#include "bry1000.h"
#include "bry2000.h"
abstract_emu *basic_emu_factory::create_emu(QString name, emu_type type)
{
    abstract_emu *ptr = nullptr;

    if(abstract_emu::is_general_cid_valid(name) || abstract_emu::is_wifiemu_cid_valid(name))
    {
        switch (type) {
        case emu_type::bry1000_type:
            ptr = new bry1000(name);
            break;
        case emu_type::bry2000_type:
            ptr = new bry2000(name);
            break;
        default:
            break;
        }
    }

    return ptr;
}
