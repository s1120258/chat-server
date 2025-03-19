#include "ChatServer.h"
#include "utils/DbUtils.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

namespace {
    const QString ROOM_QUERY_FILE = "queries/room_queries.sql";
    const QString MESSAGE_QUERY_FILE = "queries/message_queries.sql";
};

ChatServer::ChatServer(QObject* parent) : QTcpServer(parent) {}

void ChatServer::startServer(quint16 port) {
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "Chat server started on port" << port;
    }
    else {
        qDebug() << "Failed to start chat server:" << this->errorString();
    }
}

void ChatServer::createRoomsTable(QSqlDatabase& db)
{
    QSqlQuery query(db);

    QString checkTableQuery = DbUtils::loadQueryFromFile("CHECK_ROOM_TABLE", ROOM_QUERY_FILE);
    if (checkTableQuery.isEmpty() || !DbUtils::checkTable(query, checkTableQuery)) {
        return;
    }

    QString createTableQuery = DbUtils::loadQueryFromFile("CREATE_ROOM_TABLE", ROOM_QUERY_FILE);
    if (createTableQuery.isEmpty() || !query.exec(createTableQuery)) {
        qDebug() << "Failed to create rooms table:" << query.lastError().text();
        return;
    }

    qDebug() << "Rooms table created successfully!";
}

void ChatServer::createMessagesTable(QSqlDatabase& db)
{
    QSqlQuery query(db);

    QString checkTableQuery = DbUtils::loadQueryFromFile("CHECK_MESSAGE_TABLE", MESSAGE_QUERY_FILE);
    if (checkTableQuery.isEmpty() || !DbUtils::checkTable(query, checkTableQuery)) {
        return;
    }

    QString createTableQuery = DbUtils::loadQueryFromFile("CREATE_MESSAGE_TABLE", MESSAGE_QUERY_FILE);
    if (createTableQuery.isEmpty() || !query.exec(createTableQuery)) {
        qDebug() << "Failed to create messages table:" << query.lastError().text();
        return;
    }

    qDebug() << "Messages table created successfully!";
}

void ChatServer::incomingConnection(qintptr socketDescriptor) {
    ChatClientHandler* clientHandler = new ChatClientHandler(socketDescriptor, this);
    clients.append(clientHandler);
    connect(clientHandler, &ChatClientHandler::disconnected, this, [this, clientHandler]() {
        clients.removeAll(clientHandler);
        clientHandler->deleteLater();
    });
}