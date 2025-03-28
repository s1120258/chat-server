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
	auth.createUsersTable();

    ChatServer chatServer(db);
    chatServer.createRoomsTable();
    chatServer.createUserRoomsTable();
    //chatServer.createMessagesTable();
    chatServer.startServer(12345);

	RedisManager redisManager;
    redisManager.connectToRedis();
	redisManager.subscribeToChannel("chat");

    //auth.registerUser("u01", "p01");
    //auth.registerUser("u02", "p02");
    //chatServer.createRoom("r01");
    //chatServer.createRoom("r02");
    //chatServer.joinRoom(1, 1);
    //chatServer.joinRoom(2, 1);
    //chatServer.joinRoom(1, 2);

    return a.exec();
}
