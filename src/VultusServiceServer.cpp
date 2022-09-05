#include "VultusServiceServer.h"

VultusServiceServer::VultusServiceServer()
{
    if(listen(QHostAddress("192.168.0.182"), 2000)){
        qDebug() << "Start";

        VultusDatabaseManager::connectToDatabase();

        connect(m_handler, &VultusServiceCommandHandler::authSendResponse, this, &VultusServiceServer::addToOnlineClient);
        connect(m_handler, &VultusServiceCommandHandler::getUsersResponse, this, &VultusServiceServer::sendToClient);
        connect(m_handler, &VultusServiceCommandHandler::getIsOnlineUsers, this, &VultusServiceServer::sendIsOnlineUsers);
    } else {
        qDebug() << "Error";
    }
}

VultusServiceServer::~VultusServiceServer()
{
    delete m_socket;
}


void VultusServiceServer::incomingConnection(qintptr _socket_discriptor)
{
    m_socket = new QTcpSocket;
    m_socket->setSocketDescriptor(_socket_discriptor);
    m_socket_list.append(m_socket);

    m_security_list.insert(m_socket, notsafe);

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

    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0) << QVariant(_msg);
    out.device()->seek(0);
    out << quint16(m_data.size() - sizeof(quint16));

    m_socket->write(m_data);
}

void VultusServiceServer::addToOnlineClient(QJsonArray _reply, QTcpSocket *_sender)
{
    m_socket = _sender;
    m_security_list[_sender] = safely;
    m_online_list.insert(_sender, _reply);

    sendToClient(_reply, _sender);
}

void VultusServiceServer::rmvToOnlineClient()
{
    m_socket = (QTcpSocket*)sender();
    m_security_list.remove(m_socket);
    m_online_list.remove(m_socket);
}

void VultusServiceServer::sendIsOnlineUsers(QTcpSocket *_sender)
{
    QJsonArray online_json_array;
    for(const QJsonValue &i : m_online_list.first()){
        online_json_array << i.toObject();
    }
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
        if(m_security_list[m_socket] == notsafe){
            m_handler->authCommand(request, m_socket);
        } else {
            m_handler->processCommand(request, m_socket);
        }

    }
}

