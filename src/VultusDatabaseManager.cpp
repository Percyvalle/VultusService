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

QJsonArray VultusDatabaseManager::getUsers()
{
    QJsonObject profile_object;
    QJsonArray profile_array;

    m_query->exec("SELECT * FROM table_profile");
    while(m_query->next()){
        profile_object["id"] = m_query->value("id").toJsonValue();
        profile_object["first_name"] = m_query->value("first_name").toJsonValue();
        profile_object["middle_name"] = m_query->value("middle_name").toJsonValue();
        profile_object["last_name"] = m_query->value("last_name").toJsonValue();
        profile_object["position"] = m_query->value("position").toJsonValue();
        profile_object["subdivision"] = m_query->value("subdivision").toJsonValue();
        profile_object["phone"] = m_query->value("phone").toJsonValue();
        profile_object["workphone"] = m_query->value("workphone").toJsonValue();
        profile_object["workplace"] = m_query->value("workplace").toJsonValue();
        profile_object["birthday"] = m_query->value("birthday").toJsonValue();
        profile_object["description"] = m_query->value("description").toJsonValue();
        profile_object["status_text"] = m_query->value("status_text").toJsonValue();
        profile_object["status"] = m_query->value("status").toJsonValue();

        profile_array.append(profile_object);
    }

    return profile_array;
}

QJsonArray VultusDatabaseManager::authToDatabase(QString _login, QString _password)
{
    QJsonObject profile_main_object;
    QJsonArray profile_main_array;

    m_query->exec("SELECT * FROM table_profile INNER JOIN table_auth ON table_profile.id=table_auth.id_profile");
    while(m_query->next()){
        if(m_query->value("login") == _login && m_query->value("password") == _password){
            profile_main_object["id"] = m_query->value("id").toJsonValue();
            profile_main_object["first_name"] = m_query->value("first_name").toJsonValue();
            profile_main_object["middle_name"] = m_query->value("middle_name").toJsonValue();
            profile_main_object["last_name"] = m_query->value("last_name").toJsonValue();
            profile_main_object["position"] = m_query->value("position").toJsonValue();
            profile_main_object["subdivision"] = m_query->value("subdivision").toJsonValue();
            profile_main_object["phone"] = m_query->value("phone").toJsonValue();
            profile_main_object["workphone"] = m_query->value("workphone").toJsonValue();
            profile_main_object["workplace"] = m_query->value("workplace").toJsonValue();
            profile_main_object["birthday"] = m_query->value("birthday").toJsonValue();
            profile_main_object["description"] = m_query->value("description").toJsonValue();
            profile_main_object["status_text"] = m_query->value("status_text").toJsonValue();
            profile_main_object["status"] = m_query->value("status").toJsonValue();
        }
    }

    profile_main_array.append(profile_main_object);

    return profile_main_array;
}

QSqlDatabase VultusDatabaseManager::m_manager = QSqlDatabase::addDatabase("QPSQL");
QSqlQuery *VultusDatabaseManager::m_query = new QSqlQuery;
