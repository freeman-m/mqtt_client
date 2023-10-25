//*******************************************************************************************
//
//  mqttclient.cpp
//  CREATOR: mj.he 136287213@qq.com
//  DATE: 2023/10/24
//*******************************************************************************************

#include "mainwindow.h"
#include "ui_mainwindow.h"

// 配置文件保存与加载
void MainWindow::save_windows_parm()
{
    qDebug("%s", __func__);
    settings->setValue("ip", ui->lineEdit_ip->text().trimmed());
    settings->setValue("port", ui->lineEdit_port->text().trimmed());
    settings->setValue("clientid", ui->lineEdit_clientid->text().trimmed());
    settings->setValue("username", ui->lineEdit_username->text().trimmed());
    settings->setValue("password", ui->lineEdit_password->text().trimmed());
    settings->setValue("pub_topic", ui->lineEdit_pub_topic->text().trimmed());
    settings->setValue("pub_msg", ui->lineEdit_pub_message->text().trimmed());
    settings->setValue("sub_topic", ui->lineEdit_sub_topic->text().trimmed());
    settings->setValue("/jiedian/sub_topic", ui->lineEdit_sub_topic->text().trimmed());
}

void MainWindow::read_windows_parm()
{
    qDebug("%s", __func__);
    settings = new QSettings("setting.ini",QSettings::IniFormat);
    qDebug() << QCoreApplication::applicationDirPath();
    ui->lineEdit_ip->setText(settings->value("ip").toString());
    ui->lineEdit_port->setText(settings->value("port").toString());
    ui->lineEdit_clientid->setText(settings->value("clientid").toString());
    ui->lineEdit_username->setText(settings->value("username").toString());
    ui->lineEdit_password->setText(settings->value("password").toString());
    ui->lineEdit_pub_topic->setText(settings->value("pub_topic").toString());
    ui->lineEdit_pub_message->setText(settings->value("pub_msg").toString());
    ui->lineEdit_sub_topic->setText(settings->value("sub_topic").toString());
}

void MainWindow::closeEvent ( QCloseEvent * e )
{
    //  窗口关闭前先发送断连信号
    QString sTopic = ui->lineEdit_sub_topic->text().trimmed();
    emit sig_btn_unSubscribe(sTopic);

    emit sig_btn_disconnect();
    qDebug("close 111");
    save_windows_parm();
    e->accept();            //不会将事件传递给组件的父组件，即直接关闭程序
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(sig_btn_connect()),
            &mqttClient::getInstance(), SLOT(slot_connect()));
    connect(this, SIGNAL(sig_btn_disconnect()),
            &mqttClient::getInstance(), SLOT(slot_disconnect()));
    connect(this, SIGNAL(sig_btn_publish(QString, QString)),
            &mqttClient::getInstance(), SLOT(slot_publish(QString, QString)));
    connect(this, SIGNAL(sig_btn_subscribe(QString)),
            &mqttClient::getInstance(), SLOT(slot_subscribe(QString)));
    connect(this, SIGNAL(sig_btn_unSubscribe(QString)),
            &mqttClient::getInstance(), SLOT(slot_unSubscribe(QString)));

    connect(&mqttClient::getInstance(), SIGNAL(sig_result_connect()),
            this, SLOT(slot_result_connect()));
    connect(&mqttClient::getInstance(), SIGNAL(sig_result_disconnect()),
            this, SLOT(slot_result_disconnect()));
    connect(&mqttClient::getInstance(), SIGNAL(sig_result_dataReceived(QMQTT::Message)),
            this, SLOT(slot_result_dataReceived(QMQTT::Message)));

    read_windows_parm();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test_func()
{
    qDebug("111");

    mqttClient::getInstance().test_func();
}

void MainWindow::on_button_connect_clicked()
{
    save_windows_parm();

    test_func();
    QString sIp = ui->lineEdit_ip->text().trimmed();
    quint16 nPort = (quint16)ui->lineEdit_port->text().toInt();
    QString sClientId = ui->lineEdit_clientid->text().trimmed();
    QString sUser = ui->lineEdit_username->text().trimmed();
    QString sPwd = ui->lineEdit_password->text().trimmed();
    mqttClient::getInstance().init(sIp, nPort, sClientId, sUser, sPwd);

    emit sig_btn_connect();
}

void MainWindow::on_button_disconnect_clicked()
{
    emit sig_btn_disconnect();
}

void MainWindow::on_button_publish_clicked()
{
    QString sTopic = ui->lineEdit_pub_topic->text().trimmed();
    QString sPayload = ui->lineEdit_pub_message->text().trimmed();

    emit sig_btn_publish(sTopic, sPayload);
}

void MainWindow::on_button_subscribe_clicked()
{
    QString sTopic = ui->lineEdit_sub_topic->text().trimmed();
    emit sig_btn_subscribe(sTopic);
}

void MainWindow::on_button_unsubscribe_clicked()
{
    QString sTopic = ui->lineEdit_sub_topic->text().trimmed();
    emit sig_btn_unSubscribe(sTopic);
}


void MainWindow::slot_result_connect()
{
    qDebug() << "result:Connected";
    ui->label_status->setText("Connected");
}

void MainWindow::slot_result_disconnect()
{
    qDebug() << "result:Unconnected";
    ui->label_status->setText("Unconnected");
}

void MainWindow::slot_result_dataReceived(QMQTT::Message message)
{
    QString sMes;

    QDateTime timeCurrent = QDateTime::currentDateTime();
    QString timeStr = timeCurrent.toString("[yyyy-MM-dd hh:mm:ss.zzz]") + ":\n";
    sMes += timeStr;

    sMes += "id: " + QString::number(message.id()) + " " + "qos: " +  QString::number(message.qos()) + " " + QString(message.topic()) + "\n" + message.payload() + "\n";

    ui->textEdit_log->append(sMes);
}

