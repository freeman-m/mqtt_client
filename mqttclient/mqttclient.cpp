//*******************************************************************************************
//
//  mqttclient.cpp
//  CREATOR: mj.he 136287213@qq.com
//  DATE: 2023/10/24
//*******************************************************************************************

#include "mqttclient.h"

mqttClient::mqttClient()
{
    qDebug("%s111", __func__);
    m_pClient = nullptr;
}

mqttClient &mqttClient::getInstance()
{
    static mqttClient s_obj;    // 创建1个实例，并以引用的形式返回
    return s_obj;
}

void mqttClient::test_func()
{
    qDebug("abc");
}

void mqttClient::init(QString sIp, quint16 nPort, QString sClientId, QString sUser, QString sPwd)
{
    if(m_pClient)
    {
        delete m_pClient;
        m_pClient = nullptr;
    }
    m_pClient = new QMQTT::Client(QHostAddress(sIp), nPort);
    if(m_pClient)
    {
        qDebug() << "m_pClient Create Success.";
        m_pClient->setClientId(sClientId);
        m_pClient->setUsername(sUser);
        m_pClient->setPassword(sPwd.toLatin1());
        m_pClient->setKeepAlive(60);
        m_pClient->setHostName(sIp);

        connect(m_pClient, &QMQTT::Client::connected, this, &mqttClient::sig_result_connect);
        connect(m_pClient, &QMQTT::Client::disconnected, this, &mqttClient::sig_result_disconnect);
        connect(m_pClient, SIGNAL(received(QMQTT::Message)), this, SLOT(on_dataReceived(QMQTT::Message)));
    }
}

void mqttClient::slot_connect()
{
    qDebug() << "connect 000";
    if(m_pClient)
    {
        if(!m_pClient->isConnectedToHost())
        {
            qDebug() << "connect 111";
            m_pClient->connectToHost();
        }
    }
}

void mqttClient::slot_disconnect()
{
    qDebug() << "disconnect";

    if (m_pClient == nullptr)
        return;

    if(m_pClient->isConnectedToHost())
    {
        m_pClient->disconnectFromHost();
    }
}

void mqttClient::slot_publish(QString sTopic, QString sPayload)
{
    if (m_pClient == nullptr)
        return;

    if(sTopic.isEmpty() || sPayload.isEmpty())
    {
        qDebug() << "sTopic or sPayload is empty";
        return;
    }
    QMQTT::Message message(136, sTopic, sPayload.toUtf8());
    m_pClient->publish(message);
}

void mqttClient::slot_subscribe(QString sTopic)
{
    if (m_pClient == nullptr)
        return;

    if(sTopic.isEmpty())
    {
        qDebug() << "sTopic is empty";
        return;
    }
    m_pClient->subscribe(sTopic);
}

void mqttClient::slot_unSubscribe(QString sTopic)
{
    if (m_pClient == nullptr)
        return;

    if(sTopic.isEmpty())
    {
        qDebug() << "sTopic is empty";
        return;
    }

    m_pClient->unsubscribe(sTopic);
}

void mqttClient::on_dataReceived(QMQTT::Message message)
{
    if (m_pClient == nullptr)
        return;

    qDebug() << "on_dataReceived";
    emit sig_result_dataReceived(message);
}
