#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(&m_timer,&QTimer::timeout,this,&MainWindow::onm_timer_callback);
    m_timer.start(20000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onm_write_inf(QString text)
{
    ui->textBrowser->append(text);
}

void MainWindow::onm_timer_callback()
{
    static int clear_log_count = 0;
    clear_log_count++;

    if(clear_log_count == 12)
    {
        ui->textBrowser->clear();
        clear_log_count =0;
    }

}

