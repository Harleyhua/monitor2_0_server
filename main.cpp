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

#define VERSION "软件版本2.50"

QString g_ota_beta_dir_path = "";     //ota测试文件目录
QString g_ota_official_dir_path = ""; //ota正式文件目录

QString g_current_sys_path = "";     //配置文件路径头部
QString g_http_server_ini_path = ""; //http 服务器的配置文件地址



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));

    qRegisterMetaType<abstract_emu*>("abstract_emu*");

    MainWindow w;
    mysql *tmp_mysql = nullptr;
    QDir tmpdir;
    //显示服务器程序软件版本号
    w.setWindowTitle(VERSION);

    g_current_sys_path = a.applicationDirPath();

    tmpdir.setPath(QDir(g_current_sys_path).absolutePath());
    tmpdir.cdUp();

    //http服务器的配置文件
    g_http_server_ini_path = QDir(g_current_sys_path).filePath("sys_config/http_server.ini");
    g_ota_beta_dir_path = tmpdir.filePath("otaFile_beta");
    g_ota_official_dir_path = tmpdir.filePath("otaFile_official");


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
    tmp_mysql = new mysql("main_sql");
    tmp_mysql->table_init();
    delete tmp_mysql;
    QLOG_INFO() << "启动服务成功";
    //启动服务
    bridge::ins()->start();

    QLOG_INFO() << "准备打开界面";
    w.show();
    QLOG_INFO() << "界面打开成功";
    return a.exec();
}
