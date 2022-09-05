#ifndef VULTUSSERVICECOMMANDHANDLER_H
#define VULTUSSERVICECOMMANDHANDLER_H

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QTcpSocket>
#include <QObject>

#include "VultusDatabaseManager.h"

struct JsonMassage{
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
    void authSendResponse(QTcpSocket *_sender, QJsonArray _response);
};

#endif // VULTUSSERVICECOMMANDHANDLER_H
