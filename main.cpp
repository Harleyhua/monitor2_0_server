#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

#include <QByteArray>
#include <QThread>
#include <QDir>

//#include "abstract_bym.h"
#include "abstract_emu.h"
#include "mysql.h"

#include "bridge.h"
#include "QsLog.h"
//#include "common.h"

QString g_current_sys_path = "";     //配置文件路径头部
QString g_http_server_ini_path = ""; //http 服务器的配置文件地址



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));

    qRegisterMetaType<abstract_emu*>("abstract_emu*");

    MainWindow w;
    mysql tmp_mysql("main_sql");

    g_current_sys_path = a.applicationDirPath();
    g_http_server_ini_path = QDir(g_current_sys_path).filePath("sys_config/http_server.ini");

    //启用日志系统
    {
    using namespace QsLogging;
    // 1. init the logging mechanism
    QsLogging::Logger& logger = QsLogging::Logger::instance();
    //logger.setLoggingLevel(QsLogging::TraceLevel);
    logger.setLoggingLevel(QsLogging::InfoLevel);
    //const QString sLogPath(QDir(a.applicationDirPath()).filePath("log.txt"));//文件路径
    const QString sLogPath(QDir(a.applicationDirPath()).filePath("./log_doc/device_log.txt"));
    // 2. add two destinations  //文件最大限制100M
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
                                    sLogPath, EnableLogRotation, MaxSizeBytes(0x6400000*5), MaxOldLogCount(10)));
    //DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    DestinationPtr objectDestination(DestinationFactory::MakeFunctorDestination(&w, SLOT(onm_write_inf(QString))));
    //logger.addDestination(objectDestination);
    //DestinationPtr functorDestination(DestinationFactory::MakeFunctorDestination(&logFunction));
    //logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);
    logger.addDestination(objectDestination);
    }

    //初始化数据库表
    tmp_mysql.table_init();
    //启动服务
    bridge::ins()->start();

    w.show();

    return a.exec();
}
