#include "VultusServiceCommandHandler.h"


VultusServiceCommandHandler::VultusServiceCommandHandler(QObject *parent)
    : QObject{parent}
{

}

void VultusServiceCommandHandler::processCommand(QJsonArray _command, QTcpSocket *_sender)
{
    QJsonObject command_object = _command.first().toObject();
    QString command = command_object["COMMAND"].toString();
    if(command == "getUsers"){
        QJsonArray response = VultusDatabaseManager::getUsers();
        if(response.size() > 1){
            emit getUsersResponse(response, _sender);
            return;
        } else {
            emit getUsersResponse(JsonMessage::error_msg("Ошибка получения пользователей"), _sender);
            return;
        }
    } else if(command == "getOnlineUsers"){
        emit getIsOnlineUsers(_sender);
    }
}

void VultusServiceCommandHandler::authCommand(QJsonArray _command, QTcpSocket *_sender)
{
    QJsonObject command_object = _command[0].toObject();
    QString command = command_object["COMMAND"].toString();
    if(command == "authToServer"){
        QJsonArray response =  VultusDatabaseManager::getAuth(command_object["LOGIN"].toString(),
                                                                    command_object["PASSWORD"].toString());
        if(response.size() == 1){
            emit authSendResponse(response, _sender);
            return;
        } else {
            emit authSendResponse(JsonMessage::error_msg("Ошибка аутентификации"), _sender);
            return;
        }
    }
}
