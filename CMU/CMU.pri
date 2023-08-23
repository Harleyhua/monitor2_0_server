INCLUDEPATH += \
    $$PWD/tcp_socket \
    $$PWD/parser

DEPENDPATH += \
    $$PWD/tcp_socket \
    $$PWD/parser

HEADERS += \
    $$PWD/parser/emu_protocolb.h \
    $$PWD/tcp_socket/aging_socket.h \
    $$PWD/tcp_socket/emu_socket.h

SOURCES += \
    $$PWD/parser/emu_protocolb.cpp \
    $$PWD/tcp_socket/aging_socket.cpp \
    $$PWD/tcp_socket/emu_socket.cpp

DISTFILES += \
    $$PWD/parser/protocolb_json.txt
