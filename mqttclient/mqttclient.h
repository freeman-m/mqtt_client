#ifndef _D_MQTT_CLIENT_H_
#define _D_MQTT_CLIENT_H_

#include <QObject>
#include "qmqtt.h"


class mqttClient : public QObject
{
    Q_OBJECT
public:
    mqttClient();

    static mqttClient & getInstance();   // 以引用方式返回函数值，类似于C语言里的指针*

    void test_func();

    void init(QString sIp, quint16 nPort, QString sClientId, QString sUser, QString sPwd);

private:
    QMQTT::Client *m_pClient;

signals:
    void sig_result_connect();
    void sig_result_disconnect();
    void sig_result_dataReceived(QMQTT::Message message);

public slots:
    void slot_connect();
    void slot_disconnect();
    void slot_publish(QString sTopic, QString sPayload);
    void slot_subscribe(QString sTopic);
    void slot_unSubscribe(QString sTopic);

    void on_dataReceived(QMQTT::Message message);
};

#endif
