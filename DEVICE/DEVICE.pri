INCLUDEPATH += \
    $$PWD/EMU \
    $$PWD/MI \
    $$PWD/CLIENT
DEPENDPATH += \
    $$PWD/EMU \
    $$PWD/MI \
    $$PWD/CLIENT

HEADERS += \
    $$PWD/CLIENT/cs_client.h \
    $$PWD/CLIENT/cs_controller.h \
    $$PWD/EMU/abstract_emu.h \
    $$PWD/EMU/basic_emu_factory.h \
    $$PWD/EMU/bry1000.h \
    $$PWD/EMU/bry2000.h \
    $$PWD/MI/abstract_bym.h \
    $$PWD/MI/basic_bym_factory.h \
    $$PWD/MI/bym_four.h \
    $$PWD/MI/bym_one.h \
    $$PWD/MI/bym_two.h

SOURCES += \
    $$PWD/CLIENT/cs_client.cpp \
    $$PWD/CLIENT/cs_controller.cpp \
    $$PWD/EMU/abstract_emu.cpp \
    $$PWD/EMU/basic_emu_factory.cpp \
    $$PWD/EMU/bry1000.cpp \
    $$PWD/EMU/bry2000.cpp \
    $$PWD/MI/abstract_bym.cpp \
    $$PWD/MI/basic_bym_factory.cpp \
    $$PWD/MI/bym_four.cpp \
    $$PWD/MI/bym_one.cpp \
    $$PWD/MI/bym_two.cpp


