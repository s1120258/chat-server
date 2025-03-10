#include "ChatServer.h"
#include <QDebug>

ChatServer::ChatServer(QObject* parent) : QTcpServer(parent) {}

void ChatServer::startServer(quint16 port) {
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "Chat server started on port" << port;
    }
    else {
        qDebug() << "Failed to start chat server:" << this->errorString();
    }
}

void ChatServer::incomingConnection(qintptr socketDescriptor) {
    ChatClientHandler* clientHandler = new ChatClientHandler(socketDescriptor, this);
    clients.append(clientHandler);
    connect(clientHandler, &ChatClientHandler::disconnected, this, [this, clientHandler]() {
        clients.removeAll(clientHandler);
        clientHandler->deleteLater();
    });
}