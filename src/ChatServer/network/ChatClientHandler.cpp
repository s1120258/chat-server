#include "ChatClientHandler.h"
#include "auth/UserAuth.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

ChatClientHandler::ChatClientHandler(qintptr socketDescriptor, UserAuth* userAuth, QObject* parent)
    : QObject(parent), socket(new QTcpSocket(this)), userAuth(userAuth) {
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClientHandler::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClientHandler::onDisconnected);
}

void ChatClientHandler::onReadyRead() {
    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();

    if (json["type"] == "login") {
        QString username = json["username"].toString();
        QString password = json["password"].toString();
        handleLogin(username, password);
    } else {
        qDebug() << "Received data:" << data;
        // Handle other incoming data
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
    if (!success) {
        json["errorMessage"] = "Invalid username or password";
    }
    socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact));
}


