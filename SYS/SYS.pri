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
    $$PWD/controller/agingcontroller.h \
    $$PWD/controller/devctlcontroller.h \
    $$PWD/controller/devdatacontroller.h \
    $$PWD/controller/logcontroller.h \
    $$PWD/controller/ordercontroller.h \
    $$PWD/controller/otacontroller.h \
    $$PWD/controller/powerstationcontroller.h \
    $$PWD/controller/usercontroller.h \
    $$PWD/midlayer/bridge.h \
    $$PWD/midlayer/requestmapper.h \
    $$PWD/server/aging_server.h \
    $$PWD/server/client_server.h \
    $$PWD/server/emu_server.h \
    $$PWD/service/agingservice.h \
    $$PWD/service/devdataservice.h \
    $$PWD/service/devmgservice.h \
    $$PWD/service/logservice.h \
    $$PWD/service/userservice.h

SOURCES += \
    $$PWD/controller/agingcontroller.cpp \
    $$PWD/controller/devctlcontroller.cpp \
    $$PWD/controller/devdatacontroller.cpp \
    $$PWD/controller/logcontroller.cpp \
    $$PWD/controller/ordercontroller.cpp \
    $$PWD/controller/otacontroller.cpp \
    $$PWD/controller/powerstationcontroller.cpp \
    $$PWD/controller/usercontroller.cpp \
    $$PWD/midlayer/bridge.cpp \
    $$PWD/midlayer/requestmapper.cpp \
    $$PWD/server/aging_server.cpp \
    $$PWD/server/client_server.cpp \
    $$PWD/server/emu_server.cpp \
    $$PWD/service/agingservice.cpp \
    $$PWD/service/devdataservice.cpp \
    $$PWD/service/devmgservice.cpp \
    $$PWD/service/logservice.cpp \
    $$PWD/service/userservice.cpp

DISTFILES += \
    $$PWD/server/client_server.ini


