#include "ChatClient.h"
#include <QJsonDocument>
#include <QJsonObject>
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
        socket.write(message.toUtf8());
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
}

void ChatClient::onDisconnected() {
    qDebug() << "Disconnected from server";
}