#ifndef VULTUSSERVICECOMMANDHANDLER_H
#define VULTUSSERVICECOMMANDHANDLER_H

#include <QTcpSocket>
#include <QObject>


enum command{
    authToServer = 0,
    getOnlineUsers = 1
};

class VultusServiceCommandHandler : public QObject
{
    Q_OBJECT
public:
    VultusServiceCommandHandler();

    void processCommand(uint _command, QTcpSocket *_sender, QDataStream &data);

signals:
    void authenticationIsDone(QString _login, QTcpSocket *_sender);
};

#endif // VULTUSSERVICECOMMANDHANDLER_H
