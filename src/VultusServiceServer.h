#ifndef VULTUSSERVICESERVER_H
#define VULTUSSERVICESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QThread>

#include "VultusDatabaseManager.h"
#include "VultusServiceCommandHandler.h"

struct JsonMassage{
    static QJsonArray error_msg(const QString _error){
        QJsonObject m_object_jsn;
        QJsonArray m_array_jsn;

        m_object_jsn["ERROR"] = _error;
        m_array_jsn.append(m_object_jsn);
        return m_array_jsn;
    }
};

enum secure{
    insecure = 0,
    securely = 1
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
    void sendToClient(uint _command, QJsonArray _msg);

    void addToOnlineClient(QString _login, QTcpSocket* _sender);
    void rmvToOnlineClient();

private:
    VultusServiceCommandHandler *m_handler = new VultusServiceCommandHandler;
    QMap<QTcpSocket*, int> m_socket_list;
    QMap<QTcpSocket*, QString> m_online;
    quint16 m_block_size = 0;
    QTcpSocket *m_socket;
    QByteArray m_data;
};

#endif // VULTUSSERVICESERVER_H
