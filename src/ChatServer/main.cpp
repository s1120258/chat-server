#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>

#include "network/ChatServer.h"
#include "redis/RedisManager.h"
#include "auth/UserAuth.h"
#include "utils/DBUtils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase db;
    DbUtils::connectToPostgreSQL(db);
    ChatServer chatServer(db);

	RedisManager redisManager;
    redisManager.connectToRedis();
	redisManager.subscribeToChannel("chat");

    //UserAuth auth(db);
    //auth.registerUser("u01", "p01");
    //auth.registerUser("u02", "p02");
    //chatServer.createRoom("r01");
    //chatServer.createRoom("r02");
    //chatServer.joinRoom(1, 1);
    //chatServer.joinRoom(2, 1);
    //chatServer.joinRoom(1, 2);

    return a.exec();
}
