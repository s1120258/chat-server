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

void ChatClient::onReadyRead() {
    QByteArray data = socket.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();

    if (json["type"] == "loginResult") {
        bool success = json["success"].toBool();
        QString errorMessage = json["errorMessage"].toString();
        emit loginResult(success, errorMessage);
    }
    else if (json["type"] == "joinedRooms") {
        QJsonArray roomsArray = json["rooms"].toArray();
        QStringList rooms;
        for (const QJsonValue& value : roomsArray) {
            if (value.isObject()) {
                QJsonObject roomObject = value.toObject();
                QJsonObject roomJson;
                roomJson["room_id"] = QString::number(roomObject["room_id"].toInt());
                roomJson["room_name"] = roomObject["room_name"].toString();
                QString roomJsonString = QString(QJsonDocument(roomJson).toJson(QJsonDocument::Compact));
                rooms.append(roomJsonString);
            }
            else {
                qDebug() << "Room value is not an object";
            }
        }
        emit joinedRoomsReceived(rooms);
    }
    else if (json["type"] == "roomCreated") {
        bool success = json["success"].toBool();
        QString roomName = json["roomName"].toString();
        emit roomCreated(success, roomName);
    }
    else if (json["type"] == "usersInRoom") {
        QJsonArray usersArray = json["users"].toArray();
        QStringList users;
        for (const QJsonValue& value : usersArray) {
            if (value.isObject()) {
                QJsonObject userObject = value.toObject();
                QJsonObject userJson;
                userJson["user_id"] = QString::number(userObject["user_id"].toInt());
                userJson["username"] = userObject["username"].toString();
                QString userJsonString = QString(QJsonDocument(userJson).toJson(QJsonDocument::Compact));
                users.append(userJsonString);
            }
            else {
                qDebug() << "User value is not an object";
            }
        }
        emit usersInRoomReceived(users);
    }
    else if (json["type"] == "userInvited") {
        bool success = json["success"].toBool();
        QString userName = json["username"].toString();
        emit userInvited(success, userName);
    }
    else if (json["type"] == "messages") {
        QJsonArray messagesArray = json["messages"].toArray();
        QStringList messages;
        for (const QJsonValue& value : messagesArray) {
            if (value.isObject()) {
                QJsonObject messageObject = value.toObject();
                QJsonObject messageJson;
                messageJson["username"] = messageObject["username"].toString();
                messageJson["message"] = messageObject["message"].toString();
                QString messageJsonString = QString(QJsonDocument(messageJson).toJson(QJsonDocument::Compact));
                messages.append(messageJsonString);
            }
            else {
                qDebug() << "Message value is not an object";
            }
        }
        emit messagesReceived(messages);
    }
    else if (json["type"] == "messageSent") {
        bool success = json["success"].toBool();
        QString message = json["content"].toString();
        emit messageReceived(json["content"].toString());
    }
    else {
        qDebug() << "Received unknown data:" << data;
    }
}

void ChatClient::onDisconnected() {
    qDebug() << "Disconnected from server";
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

void ChatClient::fetchUsersInRoom(int roomId)
{
	if (socket.state() == QAbstractSocket::ConnectedState) {
		QJsonObject json;
		json["type"] = "fetchUsersInRoom";
		json["roomId"] = roomId;
		socket.write(QJsonDocument(json).toJson(QJsonDocument::Compact));
	}
}

void ChatClient::inviteUserToRoom(const QString& username, int roomId) {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        QJsonObject json;
        json["type"] = "inviteUser";
        json["username"] = username;
        json["roomId"] = roomId;
        socket.write(QJsonDocument(json).toJson(QJsonDocument::Compact));
    }
}

void ChatClient::fetchMessages(int roomId) {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        QJsonObject json;
        json["type"] = "fetchMessages";
        json["roomId"] = roomId;
        socket.write(QJsonDocument(json).toJson(QJsonDocument::Compact));
    }
}

void ChatClient::sendMessage(int roomId, const QString& message) {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        QJsonObject json;
        json["type"] = "sendMessage";
        json["roomId"] = roomId;
        json["content"] = message;
        socket.write(QJsonDocument(json).toJson(QJsonDocument::Compact));
    }
}
