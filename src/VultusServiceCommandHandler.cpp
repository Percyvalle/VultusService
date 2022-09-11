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
            emit errorResponse(JsonMessage::error_msg("Ошибка получения пользователей"), _sender);
            return;
        }
    }
    else if(command == "getOnlineUsers"){
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
            QJsonArray token = generateToken(response.first().toObject()["id"].toInt(),
                                             response.first().toObject()["last_name"].toString());
            response.push_front(token.first());
            response.push_front(token.last());

            emit authSendResponse(response, _sender);
            return;
        } else {
            emit errorResponse(JsonMessage::error_msg("Ошибка аутентификации"), _sender);
            return;
        }
    }
}

QJsonArray VultusServiceCommandHandler::generateToken(const int &_id, const QString &_last_name)
{
    QJsonArray jwt_common;

    QJsonObject jwt_header_object;
    jwt_header_object["ALGM"] = "NONE";
    jwt_header_object["TYPE"] = "JWP";

    QJsonObject jwt_payload_object;
    jwt_payload_object["ID"] = _id;
    jwt_payload_object["NAME"] = _last_name;
    jwt_payload_object["ADMIN"] = "NONE";

    jwt_common.append(jwt_header_object);
    jwt_common.append(jwt_payload_object);

    return jwt_common;
}
