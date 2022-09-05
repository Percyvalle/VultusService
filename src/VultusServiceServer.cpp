#include "VultusServiceServer.h"

VultusServiceServer::VultusServiceServer()
{
    if(listen(QHostAddress("192.168.0.182"), 2000)){
        qDebug() << "Start";

        VultusDatabaseManager::connectToDatabase();

        connect(m_handler, SIGNAL(authSendResponse(QTcpSocket*, QJsonArray)), this, SLOT(addToOnlineClient(QTcpSocket*, QJsonArray)));
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

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyReadMessage()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(rmvToOnlineClient()));
    connect(m_socket, SIGNAL(disconnected()), m_socket, SLOT(deleteLater()));

    qDebug() << _socket_discriptor;
}

void VultusServiceServer::sendToClient(QJsonArray _msg)
{
    m_data.clear();
    QDataStream out(&m_data, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0) << QVariant(_msg);
    out.device()->seek(0);
    out << quint16(m_data.size() - sizeof(quint16));

    m_socket->write(m_data);
}

void VultusServiceServer::addToOnlineClient(QTcpSocket *_sender, QJsonArray _reply)
{
    m_socket = _sender;
    m_security_list.insert(_sender, safely);
    sendToClient(_reply);
}

void VultusServiceServer::rmvToOnlineClient()
{
    m_socket = (QTcpSocket*)sender();
    m_security_list.remove(m_socket);
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
        if(m_security_list[m_socket] == safely){
            qDebug() << "Safe";
        } else {
            m_handler->authCommand(request, m_socket);
        }

    }
}

