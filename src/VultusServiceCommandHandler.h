#ifndef VULTUSSERVICECOMMANDHANDLER_H
#define VULTUSSERVICECOMMANDHANDLER_H

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QTcpSocket>
#include <QObject>

class VultusServiceCommandHandler : public QObject
{
    Q_OBJECT
public:
    explicit VultusServiceCommandHandler(QObject *parent = nullptr);

    void processCommand(QJsonArray _command, QTcpSocket* sender);

signals:

};

#endif // VULTUSSERVICECOMMANDHANDLER_H
