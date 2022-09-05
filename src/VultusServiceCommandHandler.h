#ifndef VULTUSSERVICECOMMANDHANDLER_H
#define VULTUSSERVICECOMMANDHANDLER_H

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QTcpSocket>
#include <QObject>

#include "VultusDatabaseManager.h"

struct JsonMessage{
    static QJsonArray error_msg(const QString _error){
        QJsonObject m_object_jsn;
        QJsonArray m_array_jsn;

        m_object_jsn["ERROR"] = _error;
        m_array_jsn.append(m_object_jsn);
        return m_array_jsn;
    }
};


class VultusServiceCommandHandler : public QObject
{
    Q_OBJECT
public:
    explicit VultusServiceCommandHandler(QObject *parent = nullptr);

    void processCommand(QJsonArray _command, QTcpSocket*_sender);
    void authCommand(QJsonArray _command, QTcpSocket*_sender);

signals:
    void authSendResponse(QJsonArray _response, QTcpSocket *_sender);
    void getUsersResponse(QJsonArray _response, QTcpSocket *_sender);
    void getIsOnlineUsers(QTcpSocket *_sender);
};

#endif // VULTUSSERVICECOMMANDHANDLER_H
