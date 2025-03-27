#include "ChatClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

ChatClient::ChatClient(QObject* parent) : QObject(parent) {
    connect(&socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
    connect(&socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
}

void ChatClient::connectToServer(const QHostAddress& address, quint16 port) {
    socket.connectToHost(address, port);
}

void ChatClient::sendMessage(const QString& message) {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        QJsonObject json;
        json["type"] = "message";
        json["content"] = message;
        socket.write(QJsonDocument(json).toJson(QJsonDocument::Compact));
    }
}

void ChatClient::login(const QString& username, const QString& password) {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        QJsonObject json;
        json["type"] = "login";
        json["username"] = username;
        json["password"] = password;
        socket.write(QJsonDocument(json).toJson(QJsonDocument::Compact));
    }
}

void ChatClient::fetchJoinedRooms() {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        QJsonObject json;
        json["type"] = "fetchJoinedRooms";
        socket.write(QJsonDocument(json).toJson(QJsonDocument::Compact));
    }
}

void ChatClient::createRoom(const QString& roomName) {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        QJsonObject json;
        json["type"] = "createRoom";
        json["roomName"] = roomName;
        socket.write(QJsonDocument(json).toJson(QJsonDocument::Compact));
    }
}

void ChatClient::inviteUserToRoom(const QString& username, const QString& roomName) {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        QJsonObject json;
        json["type"] = "inviteUser";
        json["username"] = username;
        json["roomName"] = roomName;
        socket.write(QJsonDocument(json).toJson(QJsonDocument::Compact));
    }
}

void ChatClient::onReadyRead() {
    QByteArray data = socket.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();

    if (json["type"] == "message") {
        emit messageReceived(json["content"].toString());
    }
    else if (json["type"] == "loginResult") {
        bool success = json["success"].toBool();
        QString errorMessage = json["errorMessage"].toString();
        emit loginResult(success, errorMessage);
    }
    else if (json["type"] == "joinedRooms") {
        QJsonArray roomsArray = json["rooms"].toArray();
        QStringList rooms;
        for (const QJsonValue& value : roomsArray) {
            rooms.append(value.toString());
        }
        emit joinedRoomsReceived(rooms);
    }
    else if (json["type"] == "roomCreated") {
        bool success = json["success"].toBool();
        QString roomName = json["roomName"].toString();
        emit roomCreated(success, roomName);
    }
    else if (json["type"] == "roomJoined") {
        bool success = json["success"].toBool();
        QString roomName = json["roomName"].toString();
        emit roomJoined(success, roomName);
    }
    else if (json["type"] == "invitation") {
        QString roomName = json["roomName"].toString();
        QString invitedBy = json["invitedBy"].toString();
        emit invitationReceived(roomName, invitedBy);
    }
}

void ChatClient::onDisconnected() {
    qDebug() << "Disconnected from server";
}