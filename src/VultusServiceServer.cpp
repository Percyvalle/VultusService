#include "VultusServiceServer.h"

VultusServiceServer::VultusServiceServer()
{
    if(listen(QHostAddress::Any, 2000)){
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
    m_socket_list.insert(_socket_discriptor, m_socket);

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyReadMessage()));
    connect(m_socket, SIGNAL(disconnected()), m_socket, SLOT(deleteLater()));

    qDebug() << _socket_discriptor;
}

void VultusServiceServer::sendToClient(quint8 _command, QVariant _reply)
{
    m_data.clear();
    QDataStream out(&m_data, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0) << QVariant(_command) << _reply;
    out.device()->seek(0);
    out << quint16(m_data.size() - sizeof(quint16));

    m_socket->write(m_data);
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

        QVariant command;
        in >> command;
        if (command.value<int>() == 0){
            QVariant login;
            QVariant pass;
            in >> login;
            in >> pass;

            if (m_manager->authentication(login.value<QString>(), pass.value<QString>())){
                sendToClient(1, QVariant("Yes"));
            }
        }
    }
}

