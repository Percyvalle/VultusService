#include "VultusDatabaseManager.h"

VultusDatabaseManager::VultusDatabaseManager()
{

}

VultusDatabaseManager::~VultusDatabaseManager()
{

}

void VultusDatabaseManager::connectToDatabase()
{
    m_manager.setHostName("192.168.253.1");
    m_manager.setPort(5432);
    m_manager.setDatabaseName("vultus");
    m_manager.setUserName("percy");
    m_manager.setPassword("gkpf2ci");

    if(!m_manager.open()){
        qDebug() << "Ошибка БД: " << m_manager.lastError().text();
    } else {
        qDebug() << "Успешно!" << "Соединение с БД установлено!";
    }
}

QJsonArray VultusDatabaseManager::getUsers()
{
    QJsonObject profile_object;
    QJsonArray profile_array;

    m_query->exec("SELECT * FROM profile_Table");
    while(m_query->next()){
        profile_object["id"] = m_query->value("id").toJsonValue();
        profile_object["full_name"] = m_query->value("full_name").toJsonValue();
        profile_object["position"] = m_query->value("position").toJsonValue();
        profile_object["subdivision"] = m_query->value("subdivision").toJsonValue();
        profile_object["phone"] = m_query->value("phone").toJsonValue();
        profile_object["work_phone"] = m_query->value("work_phone").toJsonValue();
        profile_object["work_place"] = m_query->value("work_place").toJsonValue();
        profile_object["birthday"] = m_query->value("birthday").toJsonValue();
        profile_object["description"] = m_query->value("description").toJsonValue();
        profile_object["status_text"] = m_query->value("status_text").toJsonValue();
        profile_object["status_name"] = m_query->value("status_name").toJsonValue();
        profile_object["super_busy"] = m_query->value("super_busy").toJsonValue();

        profile_array.append(profile_object);
    }

    return profile_array;
}

QJsonArray VultusDatabaseManager::getAuth(const QString _login, const QString _password)
{
    QJsonObject profile_main_object;
    QJsonArray profile_main_array;

    m_query->exec("SELECT * FROM profile_table INNER JOIN auth_profile_table ON profile_table.id=auth_profile_table.id_profile");
    while(m_query->next()){
        if(m_query->value("login") == _login && m_query->value("password") == _password){
            profile_main_object["id"] = m_query->value("id").toJsonValue();
            profile_main_object["full_name"] = m_query->value("full_name").toJsonValue();
            profile_main_object["position"] = m_query->value("position").toJsonValue();
            profile_main_object["subdivision"] = m_query->value("subdivision").toJsonValue();
            profile_main_object["phone"] = m_query->value("phone").toJsonValue();
            profile_main_object["work_phone"] = m_query->value("work_phone").toJsonValue();
            profile_main_object["work_place"] = m_query->value("work_place").toJsonValue();
            profile_main_object["birthday"] = m_query->value("birthday").toJsonValue();
            profile_main_object["description"] = m_query->value("description").toJsonValue();
            profile_main_object["status_text"] = m_query->value("status_text").toJsonValue();
            profile_main_object["status_name"] = m_query->value("status_name").toJsonValue();
            profile_main_object["super_busy"] = m_query->value("super_busy").toJsonValue();
        }
    }

    profile_main_array.append(profile_main_object);

    return profile_main_array;
}

QSqlDatabase VultusDatabaseManager::m_manager = QSqlDatabase::addDatabase("QPSQL");
QSqlQuery *VultusDatabaseManager::m_query = new QSqlQuery;
