QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(./CMU/CMU.pri)
include(./DATA/DATA.pri)
include(./OPEN_LIB/OPEN_LIB.pri)
include(./OPEN_LIB/QtWebApp/httpserver/httpserver.pri)
include(./OPEN_LIB/QtWebApp/logging/logging.pri)
include(./COMMON/COMMON.pri)
include(./DEVICE/DEVICE.pri)
include(./SYS/SYS.pri)
include(./ALG/ALG.pri)
#INCLUDEPATH += $$INCLUDE_PATHS


SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui
RC_ICONS = myserver.ico
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../build-monitor2_0_server-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/log_doc/httpserver_log.log \
    ../build-monitor2_0_server-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/sys_config/http_server.ini

#程序版本
VERSION = 0.2.4.4

#程序名称
QMAKE_TARGET_PRODUCT = "monitor2_0_server"
