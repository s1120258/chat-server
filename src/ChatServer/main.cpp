#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>

#include "auth/UserAuth.h"
#include "network/ChatServer.h"
#include "redis/RedisManager.h"
#include "utils/DBUtils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase db;
    DbUtils::connectToPostgreSQL(db);

	UserAuth auth(db);
    //auth.deleteUserTable();
	auth.createUserTable();

    ChatServer chatServer(db);
    chatServer.createRoomsTable();
    chatServer.startServer(12345);

	RedisManager redisManager;
    redisManager.connectToRedis();
	redisManager.subscribeToChannel("chat");

    return a.exec();
}
