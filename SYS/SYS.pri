INCLUDEPATH += \
    $$PWD/midlayer \
    $$PWD/server
DEPENDPATH += \
    $$PWD/midlayer \
    $$PWD/server

HEADERS += \
    $$PWD/midlayer/bridge.h \
    $$PWD/server/aging_server.h \
    $$PWD/server/client_server.h \
    $$PWD/server/emu_server.h




SOURCES += \
    $$PWD/midlayer/bridge.cpp \
    $$PWD/server/aging_server.cpp \
    $$PWD/server/client_server.cpp \
    $$PWD/server/emu_server.cpp

DISTFILES += \
    $$PWD/server/client_server.ini


