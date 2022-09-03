#include "VultusServiceCommandHandler.h"

VultusServiceCommandHandler::VultusServiceCommandHandler() : QObject()
{

}

void VultusServiceCommandHandler::processCommand(uint _command, QTcpSocket *_sender, QDataStream &data)
{
    if(_command == authToServer){
        QVariant login;
        data >> login;
        qDebug() << "Аутентификация";
        emit authenticationIsDone(login.value<QString>(), _sender);
    } else if (_command == getOnlineUsers){
        qDebug() << "Список онлайн юзеров";
    }
}
