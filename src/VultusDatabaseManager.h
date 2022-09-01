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

    void connectToDatabase();
    bool authentication(QString _login, QString _password);

private:
    QSqlDatabase m_manager = QSqlDatabase::addDatabase("QPSQL");
    QSqlQuery *m_query = new QSqlQuery;
};

#endif // VULTUSDATABASEMANAGER_H
