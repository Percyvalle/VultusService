#include "VultusDatabaseManager.h"

VultusDatabaseManager::VultusDatabaseManager()
{

}

void VultusDatabaseManager::connectToDatabase()
{
    m_manager.setHostName("localhost");
    m_manager.setPort(5432);
    m_manager.setDatabaseName("Vultus");
    m_manager.setUserName("percy");
    m_manager.setPassword("gkpf2ci");

    if(!m_manager.open()){
        qDebug() << "Ошибка БД: " << m_manager.lastError().text();
    } else {
        qDebug() << "Успешно!" << "Соединение с БД установлено!";
    }
}

