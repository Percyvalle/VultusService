#ifndef VULTUSSERVICESERVER_H
#define VULTUSSERVICESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QThread>

#include "VultusDatabaseManager.h"
#include "VultusServiceCommandHandler.h"

class VultusServiceServer : public QTcpServer
{
    Q_OBJECT
public:
    VultusServiceServer();
    ~VultusServiceServer();

private slots:
    void incomingConnection(qintptr _socket_discriptor);
    void readyReadMessage();
    void sendToClient();
    void addToOnlineClient(QString _login, QTcpSocket* _sender);

private:
    VultusServiceCommandHandler *m_cmd_handler = new VultusServiceCommandHandler;
    VultusDatabaseManager *m_manager = new VultusDatabaseManager;
    QTcpSocket *m_socket;
    QVector<QTcpSocket*> m_socket_list;
    QMap<QTcpSocket*, QString> m_online;
    QByteArray m_data;
    quint16 m_block_size = 0;
};

#endif // VULTUSSERVICESERVER_H
