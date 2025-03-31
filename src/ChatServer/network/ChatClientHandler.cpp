#include "ChatClientHandler.h"
#include "ChatServer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

ChatClientHandler::ChatClientHandler(qintptr socketDescriptor, ChatServer* chatServer, QObject* parent)
    : QObject(parent), socket(new QTcpSocket(this)), chatServer(chatServer) {
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClientHandler::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClientHandler::onDisconnected);
}

void ChatClientHandler::onReadyRead() {
    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();

    QString type = json["type"].toString();
    if (type == "login") {
        QString username = json["username"].toString();
        QString password = json["password"].toString();
        handleLogin(username, password);
    }
    else if (type == "fetchJoinedRooms") {
        handleFetchJoinedRooms();
    }
    else if (type == "fetchUsersInRoom") {
        int roomId = json["roomId"].toInt();
        handleFetchUsersInRoom(roomId);
    }
    else if (type == "createRoom") {
        QString roomName = json["roomName"].toString();
        handleCreateRoom(roomName);
    }
    else if (type == "inviteUser") {
        QString username = json["username"].toString();
        int roomId = json["roomId"].toInt();
        handleInviteUserToRoom(username, roomId);
    }
    else if (type == "message") {
        QString message = json["content"].toString();
        handleMessage(message);
    }
    else {
        qDebug() << "Received unknown data:" << data;
    }
}

void ChatClientHandler::onDisconnected() {
    qDebug() << "Client disconnected";
    emit disconnected();
}

void ChatClientHandler::handleLogin(const QString& username, const QString& password) {
    bool success = chatServer->authenticateUser(username, password);
    QJsonObject json;
    json["type"] = "loginResult";
    json["success"] = success;
    if (success) {
        userId = chatServer->getUserId(username);
    }
    else {
        json["errorMessage"] = "Invalid username or password";
    }
    socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact));
}

void ChatClientHandler::handleFetchJoinedRooms() {
    QVector<QVariantMap> rooms = chatServer->fetchJoinedRooms(userId);
    QJsonObject json;
    json["type"] = "joinedRooms";
    QJsonArray roomsArray;

    for (const auto& room : rooms) {
        if (!room.contains("room_id") || !room.contains("room_name")) {
            qDebug() << "Room data is missing 'room_id' or 'room_name':" << room;
            return;
        }
        QJsonObject roomJson;
        roomJson["room_id"] = room["room_id"].toInt();
        roomJson["room_name"] = room["room_name"].toString();
        roomsArray.append(roomJson);
    }

    json["rooms"] = roomsArray;
    socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact));
}

void ChatClientHandler::handleFetchUsersInRoom(int roomId) {
    QVector<QVariantMap> users = chatServer->fetchUsersInRoom(roomId);
    QJsonObject json;
    json["type"] = "usersInRoom";
    QJsonArray usersArray;

    for (const auto& user : users) {
        if (!user.contains("user_id") || !user.contains("username")) {
            qDebug() << "Room data is missing 'user_id' or 'username':" << user;
            return;
        }
        QJsonObject userJson;
        userJson["user_id"] = user["user_id"].toInt();
        userJson["username"] = user["username"].toString();
        usersArray.append(userJson);
    }

    json["users"] = usersArray;
    socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact));
}

void ChatClientHandler::handleCreateRoom(const QString& roomName) {
    bool success = chatServer->createRoom(roomName, userId);
    QJsonObject json;
    json["type"] = "roomCreated";
    json["success"] = success;
    json["room_name"] = roomName;
    socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact));
}

void ChatClientHandler::handleInviteUserToRoom(const QString& username, int roomId) {
    bool success = chatServer->joinRoom(username, roomId);
    QJsonObject json;
    json["type"] = "userInvited";
    json["success"] = success;
    json["username"] = username;
    json["room_id"] = roomId;
    socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact));
}

void ChatClientHandler::handleMessage(const QString& message) {
    qDebug() << "Message received:" << message;
    // Broadcast message to other clients or handle it as needed
}
