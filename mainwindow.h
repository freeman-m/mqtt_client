#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSettings>
#include <QCloseEvent>

#include "./mqttclient/mqttclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void test_func();

signals:
    void sig_btn_connect();
    void sig_btn_disconnect();
    void sig_btn_publish(QString sTopic, QString sPayload);
    void sig_btn_subscribe(QString sTopic);
    void sig_btn_unSubscribe(QString sTopic);

private slots:
    void on_button_connect_clicked();

    void on_button_disconnect_clicked();

    void on_button_publish_clicked();

    void on_button_subscribe_clicked();

    void on_button_unsubscribe_clicked();


    void slot_result_connect();
    void slot_result_disconnect();
    void slot_result_dataReceived(QMQTT::Message message);

private:
    Ui::MainWindow *ui;

    // 重写关闭事件
    virtual void closeEvent(QCloseEvent* event) override;

    // 配置文件对象
    QSettings *settings;

    // 配置文件保存与恢复函数
    void save_windows_parm();
    void read_windows_parm();

};

#endif // MAINWINDOW_H
