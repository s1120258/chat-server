#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>
#include "network/ChatServer.h"
#include "utils/DBUtils.h"
//#include "auth/UserAuth.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase db;
    DbUtils::connectToPostgreSQL(db);
    ChatServer chatServer(db);

    //UserAuth auth(db);
    //auth.registerUser("u01", "p01");
    //auth.registerUser("u02", "p02");
    //chatServer.createRoom("r01");
    //chatServer.createRoom("r02");
    //chatServer.joinRoom(1, 1);
    //chatServer.joinRoom(2, 1);
    //chatServer.joinRoom(1, 2);
    //chatServer.sendMessage(1, 1, "m01");

    return a.exec();
}
