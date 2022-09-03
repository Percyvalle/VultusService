#include "VultusServiceCommandHandler.h"


VultusServiceCommandHandler::VultusServiceCommandHandler(QObject *parent)
    : QObject{parent}
{

}

void VultusServiceCommandHandler::processCommand(QJsonArray _command, QTcpSocket *sender)
{
    QJsonObject command_object = _command[0].toObject();
    QString command = command_object["COMMAND"].toString();
    if(command == "authToServer"){
        qDebug() << command;
    }
}
