#SOURCES += \
CONFIG += c++11

INCLUDEPATH += \
    $$PWD/QsLog/include

DEPENDPATH += \
    $$PWD/QsLog/include


DEFINES += QS_LOG_LINE_NUMBERS    # automatically writes the file and line for each log message
#DEFINES += QS_LOG_DISABLE         # logging code is replaced with a no-op
HEADERS += \
    $$PWD/QsLog/include/QsLog.h \
    $$PWD/QsLog/include/QsLogDest.h \
    $$PWD/QsLog/include/QsLogDisableForThisFile.h \
    $$PWD/QsLog/include/QsLogLevel.h

LIBS += -L$$PWD/QsLog/bin -lQsLog2


