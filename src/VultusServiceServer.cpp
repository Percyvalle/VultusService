#include "VultusServiceServer.h"

VultusServiceServer::VultusServiceServer()
{
    if(listen(QHostAddress("192.168.0.182"), 2000)){
        qDebug() << "Start";

        VultusDatabaseManager::connectToDatabase();

        connect(m_handler, &VultusServiceCommandHandler::authSendResponse, this, &VultusServiceServer::addToOnlineClient);
        connect(m_handler, &VultusServiceCommandHandler::getIsOnlineUsers, this, &VultusServiceServer::sendIsOnlineUsers);
        connect(m_handler, &VultusServiceCommandHandler::getUsersResponse, this, &VultusServiceServer::sendToClient);
        connect(m_handler, &VultusServiceCommandHandler::errorResponse, this, &VultusServiceServer::sendToClient);
    } else {
        qDebug() << "Error";
    }
}

VultusServiceServer::~VultusServiceServer()
{
    delete m_socket;
    delete m_handler;
}


void VultusServiceServer::incomingConnection(qintptr _socket_discriptor)
{
    m_socket = new QTcpSocket;
    m_socket->setSocketDescriptor(_socket_discriptor);

    connect(m_socket, &QIODevice::readyRead, this, &VultusServiceServer::readyReadMessage);
    connect(m_socket, &QAbstractSocket::disconnected, this, &VultusServiceServer::rmvToOnlineClient);
    connect(m_socket, &QAbstractSocket::disconnected, m_socket, &QObject::deleteLater);

    qDebug() << _socket_discriptor;
}

void VultusServiceServer::sendToClient(QJsonArray _msg, QTcpSocket *_socket)
{
    m_socket = _socket;
    m_data.clear();
    QDataStream out(&m_data, QIODevice::WriteOnly);

    qDebug() << _msg;
    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0) << QVariant(_msg);
    out.device()->seek(0);
    out << quint16(m_data.size() - sizeof(quint16));

    m_socket->write(m_data);
}

void VultusServiceServer::addToOnlineClient(QJsonArray _reply, QTcpSocket *_sender)
{
    m_socket = _sender;
    m_online_list.insert(_sender, _reply);

    sendToClient(_reply, _sender);
}

void VultusServiceServer::rmvToOnlineClient()
{
    m_socket = (QTcpSocket*)sender();
    m_online_list.remove(m_socket);
}

void VultusServiceServer::sendIsOnlineUsers(QTcpSocket *_sender)
{

    QJsonArray online_json_array;
    for(QJsonArray &ary : m_online_list){
        if(m_online_list.key(ary) == _sender){
            continue;
        }
        online_json_array.append(ary.first().toObject());
    }

    m_handler->addHeaderResponse(online_json_array, "getIsOnlineUsersResponse");
    sendToClient(online_json_array, _sender);
}

void VultusServiceServer::readyReadMessage()
{
    m_socket = (QTcpSocket*)sender();

    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_5_15);

    if(in.status() == QDataStream::Ok){
        if(m_block_size == 0){
            if(m_socket->bytesAvailable() < 2){
                return;
            }

            in >> m_block_size;
        }
        if(m_socket->bytesAvailable() < m_block_size){
            return;
        } else {
            m_block_size = 0;
        }

        QVariant request_var;
        in >> request_var;

        QJsonArray request = request_var.toJsonArray();
        qDebug() << request;
        m_handler->authCommand(request, m_socket);
        m_handler->processCommand(request, m_socket);
    }
}

