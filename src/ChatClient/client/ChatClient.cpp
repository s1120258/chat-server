#include "ChatClient.h"
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

void ChatClient::onReadyRead() {
    QByteArray data = socket.readAll();
    qDebug() << "Message received:" << data;
    emit messageReceived(QString::fromUtf8(data));
}

void ChatClient::onDisconnected() {
    qDebug() << "Disconnected from server";
}