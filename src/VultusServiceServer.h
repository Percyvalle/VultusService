#ifndef VULTUSSERVICESERVER_H
#define VULTUSSERVICESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

#include "VultusDatabaseManager.h"

class VultusServiceServer : public QTcpServer
{
    Q_OBJECT
public:
    VultusServiceServer();
    ~VultusServiceServer();

private slots:
    void incomingConnection(qintptr _socket_discriptor);
    void readyReadMessage();
    void sendToClient(quint8 _command, QVariant _reply);

private:
    VultusDatabaseManager *m_manager = new VultusDatabaseManager();
    QTcpSocket *m_socket;
    QByteArray m_data;
    quint16 m_block_size = 0;
    QVector<QString> m_status_list;
    QMap<qintptr, QTcpSocket*> m_socket_list;
};

#endif // VULTUSSERVICESERVER_H
