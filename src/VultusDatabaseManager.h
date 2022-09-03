#ifndef VULTUSDATABASEMANAGER_H
#define VULTUSDATABASEMANAGER_H

#include <QtSql>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

class VultusDatabaseManager
{
public:
    VultusDatabaseManager();
    ~VultusDatabaseManager();

    static void connectToDatabase();

    static QJsonArray getUsers();
    static QJsonArray authToDatabase(QString _login, QString _password);

private:
    static QSqlDatabase m_manager;
    static QSqlQuery *m_query;
};

#endif // VULTUSDATABASEMANAGER_H
