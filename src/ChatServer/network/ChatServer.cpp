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

ChatServer::ChatServer(QSqlDatabase& db, QObject* parent) : m_db(db), QTcpServer(parent) {}

void ChatServer::startServer(quint16 port) {
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "Chat server started on port" << port;
    }
    else {
        qDebug() << "Failed to start chat server:" << this->errorString();
    }
}

void ChatServer::createRoomsTable()
{
    QSqlQuery query(m_db);

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

void ChatServer::createMessagesTable()
{
    QSqlQuery query(m_db);

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

bool ChatServer::createRoom(const QString& roomName)
{
    QSqlQuery query(m_db);

    QString queryStr = DbUtils::loadQueryFromFile("CREATE_ROOM", ROOM_QUERY_FILE);
    if (queryStr.isEmpty()) {
        return false;
    }

    query.prepare(queryStr);
    query.bindValue(":name", roomName);

    if (!query.exec()) {
        qDebug() << "Database query error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool ChatServer::joinRoom(int userId, int roomId)
{
    QSqlQuery query(m_db);

    QString queryStr = DbUtils::loadQueryFromFile("JOIN_ROOM", ROOM_QUERY_FILE);
    if (queryStr.isEmpty()) {
        return false;
    }

    query.prepare(queryStr);
    query.bindValue(":user_id", userId);
    query.bindValue(":room_id", roomId);

    if (!query.exec()) {
        qDebug() << "Database query error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool ChatServer::leaveRoom(int userId, int roomId)
{
    QSqlQuery query(m_db);

    QString queryStr = DbUtils::loadQueryFromFile("LEAVE_ROOM", ROOM_QUERY_FILE);
    if (queryStr.isEmpty()) {
        return false;
    }

    query.prepare(queryStr);
    query.bindValue(":user_id", userId);
    query.bindValue(":room_id", roomId);

    if (!query.exec()) {
        qDebug() << "Database query error:" << query.lastError().text();
        return false;
    }

    return true;
}

void ChatServer::incomingConnection(qintptr socketDescriptor) {
    ChatClientHandler* clientHandler = new ChatClientHandler(socketDescriptor, this);
    clients.append(clientHandler);
    connect(clientHandler, &ChatClientHandler::disconnected, this, [this, clientHandler]() {
        clients.removeAll(clientHandler);
        clientHandler->deleteLater();
    });
}