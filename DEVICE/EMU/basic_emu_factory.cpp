#include "basic_emu_factory.h"

#include "bry1000.h"
#include "bry2000.h"
abstract_emu *basic_emu_factory::create_emu(QString name, emu_type type)
{
    abstract_emu *ptr = nullptr;

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

    return ptr;
}
