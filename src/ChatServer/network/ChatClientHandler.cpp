#include "ChatClientHandler.h"
#include "ChatServer.h"
#include "auth/UserAuth.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

ChatClientHandler::ChatClientHandler(qintptr socketDescriptor, ChatServer* chatServer, UserAuth* userAuth, QObject* parent)
    : QObject(parent), socket(new QTcpSocket(this)), chatServer(chatServer), userAuth(userAuth) {
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
    else if (type == "createRoom") {
        QString roomName = json["roomName"].toString();
        handleCreateRoom(roomName);
    }
    else if (type == "inviteUser") {
        QString username = json["username"].toString();
        QString roomName = json["roomName"].toString();
        handleInviteUser(username, roomName);
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
    bool success = userAuth->authenticateUser(username, password);
    QJsonObject json;
    json["type"] = "loginResult";
    json["success"] = success;
    if (success) {
        userId = userAuth->getUserId(username);
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

    qDebug() << "Fetched rooms:" << rooms;

    for (const auto& room : rooms) {
        QJsonObject roomJson;
        if (room.contains("room_id") && room.contains("room_name")) {
            roomJson["room_id"] = room["room_id"].toInt();
            roomJson["room_name"] = room["room_name"].toString();
            roomsArray.append(roomJson);
        }
        else {
            qDebug() << "Room data is missing 'room_id' or 'room_name':" << room;
        }
    }

    json["rooms"] = roomsArray;
    qDebug() << "Fetched rooms:" << QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
    socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact));
}


void ChatClientHandler::handleCreateRoom(const QString& roomName) {
    bool success = chatServer->createRoom(roomName, userId);
    QJsonObject json;
    json["type"] = "roomCreated";
    json["success"] = success;
    json["roomName"] = roomName;
    socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact));
}

void ChatClientHandler::handleInviteUser(const QString& username, const QString& roomName) {
    QJsonObject json;
    json["type"] = "invitation";
    json["roomName"] = roomName;
    json["invitedBy"] = username;
    // Send invitation to the specified user
    // Example: find the user and send the invitation
    socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact));
}

void ChatClientHandler::handleMessage(const QString& message) {
    qDebug() << "Message received:" << message;
    // Broadcast message to other clients or handle it as needed
}
