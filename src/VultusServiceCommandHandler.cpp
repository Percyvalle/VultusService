#include "VultusServiceCommandHandler.h"


VultusServiceCommandHandler::VultusServiceCommandHandler(QObject *parent)
    : QObject{parent}
{

}

void VultusServiceCommandHandler::processCommand(QJsonArray _command, QTcpSocket *_sender)
{
    QJsonObject command_object = _command[0].toObject();
    QString command = command_object["COMMAND"].toString();
}

void VultusServiceCommandHandler::authCommand(QJsonArray _command, QTcpSocket *_sender)
{
    QJsonObject command_object = _command[0].toObject();
    QString command = command_object["COMMAND"].toString();
    QString login = command_object["LOGIN"].toString();
    QString passwd = command_object["PASSWORD"].toString();
    if(command == "authToServer"){
        QJsonArray response =  VultusDatabaseManager::authToDatabase(login, passwd);
        if(response.size() == 1){
            emit authSendResponse(_sender, response);
            return;
        } else {
            emit authSendResponse(_sender, JsonMassage::error_msg("Ошибка аутентификации"));
            return;
        }
    }
}
