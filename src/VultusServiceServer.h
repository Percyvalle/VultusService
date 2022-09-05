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
    void sendToClient(QJsonArray _msg);

    void addToOnlineClient(QTcpSocket* _sender, QJsonArray _reply);
    void rmvToOnlineClient();

private:
    VultusServiceCommandHandler *m_handler = new VultusServiceCommandHandler;
    QVector<QTcpSocket*> m_socket_list;
    QMap<QTcpSocket*, int> m_security_list;
    quint16 m_block_size = 0;
    QTcpSocket *m_socket;
    QByteArray m_data;
};

#endif // VULTUSSERVICESERVER_H
