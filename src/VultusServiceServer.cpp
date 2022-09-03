#include "VultusServiceServer.h"

VultusServiceServer::VultusServiceServer()
{
    if(listen(QHostAddress("192.168.0.182"), 2000)){
        qDebug() << "Start";
        m_manager->connectToDatabase();
    } else {
        qDebug() << "Error";
    }
}

VultusServiceServer::~VultusServiceServer()
{

}


void VultusServiceServer::incomingConnection(qintptr _socket_discriptor)
{
    m_socket = new QTcpSocket;
    m_socket->setSocketDescriptor(_socket_discriptor);
    m_socket_list.append(m_socket);

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyReadMessage()));
    connect(m_socket, SIGNAL(disconnected()), m_socket, SLOT(deleteLater()));
    connect(m_cmd_handler, SIGNAL(authenticationIsDone(QString, QTcpSocket*)), this, SLOT(addToOnlineClient(QString, QTcpSocket*)));

    qDebug() << _socket_discriptor;
}

void VultusServiceServer::sendToClient()
{
    m_data.clear();
    QDataStream out(&m_data, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0);
    out.device()->seek(0);
    out << quint16(m_data.size() - sizeof(quint16));

    m_socket->write(m_data);
}

void VultusServiceServer::addToOnlineClient(QString _login, QTcpSocket *_sender)
{
    m_online.insert(_sender, _login);
    qDebug() << m_online.size();
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

        QVariant reply_command;
        in >> reply_command;
        m_cmd_handler->processCommand(reply_command.value<uint>(), m_socket,in);
    }
}

