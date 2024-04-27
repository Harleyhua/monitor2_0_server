#include "basic_bym_factory.h"

#include "bym_one.h"
#include "bym_two.h"
#include "bym_four.h"

abstract_bym *basic_bym_factory::create_emu(QString name, bym_type type)
{
    abstract_bym *ptr = nullptr;
    switch (type) {
    case bym_type::bym_one_type:
        ptr = new bym_one(name);
        break;
    case bym_type::bym_two_type:
        ptr = new bym_two(name);
        break;
    case bym_type::bym_four_type:
        ptr = new bym_four(name);
        break;
    default:
        break;
    }

    return ptr;
}
