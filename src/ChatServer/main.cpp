#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>
#include "auth/UserAuth.h"
#include "network/ChatServer.h"
#include "redis/RedisManager.h"

void connectToPostgreSQL(QSqlDatabase& db) {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("chatdb");
    db.setUserName("chatuser");
    db.setPassword("chatpassword");
    db.setPort(5432); // Default PostgreSQL port

    if (!db.open()) {
        qDebug() << "Failed to connect to PostgreSQL:" << db.lastError().text();
    }
    else {
        qDebug() << "Connected to PostgreSQL!";
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase db;
    connectToPostgreSQL(db);

	UserAuth auth(db);
    //auth.deleteUserTable();
	auth.createUserTable();

    ChatServer chatServer;
    chatServer.startServer(12345);

	RedisManager redisManager;
    redisManager.connectToRedis();
	redisManager.subscribeToChannel("chat");

    return a.exec();
}
