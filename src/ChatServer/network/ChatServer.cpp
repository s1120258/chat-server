#include "ChatServer.h"
#include "utils/DbUtils.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

namespace {
    const QString ROOM_QUERY_FILE = "queries/room_queries.sql";
    const QString USER_ROOM_QUERY_FILE = "queries/user_room_queries.sql";
    const QString MESSAGE_QUERY_FILE = "queries/message_queries.sql";
};

ChatServer::ChatServer(QSqlDatabase& db, QObject* parent) : QTcpServer(parent), m_db(db), userAuth(new UserAuth(db)) {}

void ChatServer::startServer(quint16 port) {
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "Chat server started on port" << port;
    }
    else {
        qDebug() << "Failed to start chat server:" << this->errorString();
    }
}

void ChatServer::incomingConnection(qintptr socketDescriptor) {
    ChatClientHandler* clientHandler = new ChatClientHandler(socketDescriptor, this, userAuth, this);
    clients.append(clientHandler);
    connect(clientHandler, &ChatClientHandler::disconnected, this, [this, clientHandler]() {
        clients.removeAll(clientHandler);
        clientHandler->deleteLater();
        });
}

void ChatServer::createRoomsTable()
{
    QSqlQuery query(m_db);

    QString checkTableQuery = DbUtils::loadQueryFromFile("CHECK_ROOMS_TABLE", ROOM_QUERY_FILE);
    if (checkTableQuery.isEmpty() || !DbUtils::checkTable(query, checkTableQuery)) {
        return;
    }

    QString createTableQuery = DbUtils::loadQueryFromFile("CREATE_ROOMS_TABLE", ROOM_QUERY_FILE);
    if (createTableQuery.isEmpty() || !query.exec(createTableQuery)) {
        qDebug() << "Failed to create table:" << query.lastError().text();
        return;
    }

    qDebug() << "Rooms table created successfully!";
}

void ChatServer::createUserRoomsTable()
{
    QSqlQuery query(m_db);

    QString checkTableQuery = DbUtils::loadQueryFromFile("CHECK_USER_ROOMS_TABLE", USER_ROOM_QUERY_FILE);
    if (checkTableQuery.isEmpty() || !DbUtils::checkTable(query, checkTableQuery)) {
        return;
    }

    QString createTableQuery = DbUtils::loadQueryFromFile("CREATE_USER_ROOMS_TABLE", USER_ROOM_QUERY_FILE);
    if (createTableQuery.isEmpty() || !query.exec(createTableQuery)) {
        qDebug() << "Failed to create user_rooms table:" << query.lastError().text();
        return;
    }

    qDebug() << "User rooms table created successfully!";
}

void ChatServer::createMessagesTable()
{
    QSqlQuery query(m_db);

    QString checkTableQuery = DbUtils::loadQueryFromFile("CHECK_MESSAGES_TABLE", MESSAGE_QUERY_FILE);
    if (checkTableQuery.isEmpty() || !DbUtils::checkTable(query, checkTableQuery)) {
        return;
    }

    QString createTableQuery = DbUtils::loadQueryFromFile("CREATE_MESSAGES_TABLE", MESSAGE_QUERY_FILE);
    if (createTableQuery.isEmpty() || !query.exec(createTableQuery)) {
        qDebug() << "Failed to create messages table:" << query.lastError().text();
        return;
    }

    qDebug() << "Messages table created successfully!";
}

QVector<QVariantMap> ChatServer::fetchJoinedRooms(int userId)
{
    QSqlQuery query(m_db);

    QString queryStr = DbUtils::loadQueryFromFile("FETCH_JOINED_ROOMS", USER_ROOM_QUERY_FILE);
    if (queryStr.isEmpty()) {
        return {};
    }

    query.prepare(queryStr);
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "Failed to fetch joined rooms:" << query.lastError().text();
        return {};
    }

    QVector<QVariantMap> rooms;
    while (query.next()) {
        QVariantMap room;
        room["room_id"] = query.value("room_id");
        room["room_name"] = query.value("room_name");
        rooms.append(room);
    }
    return rooms;
}

bool ChatServer::createRoom(const QString& roomName, int userId)
{
    QSqlQuery query(m_db);

    QString queryStr = DbUtils::loadQueryFromFile("PREPARE_ROOM", ROOM_QUERY_FILE);
    if (queryStr.isEmpty()) {
        return false;
    }

    query.prepare(queryStr);
    query.bindValue(":name", roomName);

    if (!query.exec()) {
        qDebug() << "Database query error:" << query.lastError().text();
        return false;
    }

    int roomId = query.lastInsertId().toInt();
    if (!joinRoom(userId, roomId)) {
        qDebug() << "Failed to join room after creation";
        return false;
    }

    qDebug() << "Room " << roomName << " created successfully!";

    return true;
}

bool ChatServer::joinRoom(int userId, int roomId)
{
    QSqlQuery query(m_db);

    QString queryStr = DbUtils::loadQueryFromFile("JOIN_ROOM", USER_ROOM_QUERY_FILE);
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

    qDebug() << "User " << userId << " joined room " << roomId << " successfully!";

    return true;
}

bool ChatServer::leaveRoom(int userId, int roomId)
{
    QSqlQuery query(m_db);

    QString queryStr = DbUtils::loadQueryFromFile("LEAVE_ROOM", USER_ROOM_QUERY_FILE);
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

    qDebug() << "User " << userId << " left from room " << roomId << " successfully!";

    return true;
}

QVector<QVariantMap> ChatServer::fetchMessages(int roomId)
{
    QSqlQuery query(m_db);

    QString queryStr = DbUtils::loadQueryFromFile("FETCH_MESSAGES", MESSAGE_QUERY_FILE);
	if (queryStr.isEmpty()) {
		return {};
    }

    query.prepare(queryStr);
    query.bindValue(":room_id", roomId);

    if (!query.exec()) {
        qDebug() << "Failed to fetch messages:" << query.lastError().text();
        return {};
    }

    QVector<QVariantMap> messages;
    while (query.next()) {
        QVariantMap message;
        message["message"] = query.value("message");
        message["created_at"] = query.value("created_at");
        message["username"] = query.value("username");
        messages.append(message);
    }
    return messages;
}
