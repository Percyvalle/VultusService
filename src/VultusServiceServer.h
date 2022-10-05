#ifndef VULTUSSERVICESERVER_H
#define VULTUSSERVICESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QThread>

#include "VultusDatabaseManager.h"
#include "VultusServiceCommandHandler.h"

enum secure{
    safely = 1,
    notsafe = 0
};

class VultusServiceServer : public QTcpServer
{
    Q_OBJECT
public:
    VultusServiceServer();
    ~VultusServiceServer();

private slots:
    void incomingConnection(qintptr _socket_discriptor);

    void readyReadMessage();
    void sendToClient(QJsonArray _msg, QTcpSocket *_socket);

    void rmvToOnlineClient();
    void addToOnlineClient(QJsonArray _reply, QTcpSocket* _sender);
    void sendIsOnlineUsers(QTcpSocket* _sender);

private:
    VultusServiceCommandHandler *m_handler = new VultusServiceCommandHandler;
    QMap<QTcpSocket*, QJsonArray> m_secure_list; // Сюда должен идти JWT токен
    QMap<QTcpSocket*, QJsonArray> m_online_list; // Сюда вся информаци о пользователях в сети
    QTcpSocket *m_socket;
    quint16 m_block_size=0;
    QByteArray m_data;
};

#endif // VULTUSSERVICESERVER_H
