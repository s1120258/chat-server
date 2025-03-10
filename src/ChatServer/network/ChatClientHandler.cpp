#include "ChatClientHandler.h"
#include <QDebug>

ChatClientHandler::ChatClientHandler(qintptr socketDescriptor, QObject* parent)
    : QObject(parent), socket(new QTcpSocket(this)) {
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClientHandler::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClientHandler::onDisconnected);
}

void ChatClientHandler::onReadyRead() {
    QByteArray data = socket->readAll();
    qDebug() << "Received data:" << data;
    // Handle incoming data
}

void ChatClientHandler::onDisconnected() {
    qDebug() << "Client disconnected";
    emit disconnected();
}