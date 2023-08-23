#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer m_timer;
public slots:
    void onm_write_inf(QString text);
    void onm_timer_callback();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
