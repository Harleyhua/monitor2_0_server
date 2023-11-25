INCLUDEPATH += \
    $$PWD/midlayer \
    $$PWD/server \
    $$PWD/service \
    $$PWD/controller
DEPENDPATH += \
    $$PWD/midlayer \
    $$PWD/server \
    $$PWD/service \
    $$PWD/controller
HEADERS += \
    $$PWD/controller/user_controller.h \
    $$PWD/midlayer/bridge.h \
    $$PWD/server/aging_server.h \
    $$PWD/server/client_server.h \
    $$PWD/server/emu_server.h \
    $$PWD/service/user_service.h

SOURCES += \
    $$PWD/controller/user_controller.cpp \
    $$PWD/midlayer/bridge.cpp \
    $$PWD/server/aging_server.cpp \
    $$PWD/server/client_server.cpp \
    $$PWD/server/emu_server.cpp \
    $$PWD/service/user_service.cpp

DISTFILES += \
    $$PWD/server/client_server.ini


