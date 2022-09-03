#include "VultusServiceServer.h"

VultusServiceServer::VultusServiceServer()
{
    if(listen(QHostAddress("192.168.0.182"), 2000)){
        qDebug() << "Start";

        VultusDatabaseManager::connectToDatabase();
        VultusDatabaseManager::authToDatabase("admin", "admin");
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
    m_socket_list.insert(m_socket, insecure);

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyReadMessage()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(rmvToOnlineClient()));
    connect(m_socket, SIGNAL(disconnected()), m_socket, SLOT(deleteLater()));

    qDebug() << _socket_discriptor;
}

void VultusServiceServer::sendToClient(uint _command, QJsonArray _msg)
{
    m_data.clear();
    QDataStream out(&m_data, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0) << QVariant(_command) << QVariant(_msg);
    out.device()->seek(0);
    out << quint16(m_data.size() - sizeof(quint16));

    m_socket->write(m_data);
}

void VultusServiceServer::addToOnlineClient(QString _login, QTcpSocket *_sender)
{
    m_online.insert(_sender, _login);
    m_socket_list[_sender] = securely;
}

void VultusServiceServer::rmvToOnlineClient()
{
    m_socket = (QTcpSocket*)sender();
    m_online.remove(m_socket);
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

        addToOnlineClient("gomanov", m_socket);
        if(m_socket_list.value(m_socket) == securely){
            QVariant reply_command;
            in >> reply_command;
            m_handler->processCommand(reply_command.toJsonArray(), m_socket);
        } else {
            sendToClient(0, JsonMassage::error_msg("insecure"));
        }
    }
}

