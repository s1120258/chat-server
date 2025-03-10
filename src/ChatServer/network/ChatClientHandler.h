#ifndef CHATCLIENTHANDLER_H
#define CHATCLIENTHANDLER_H

#include <QTcpSocket>
#include <QObject>

class ChatClientHandler : public QObject {
    Q_OBJECT

public:
    ChatClientHandler(qintptr socketDescriptor, QObject* parent = nullptr);

signals:
    void disconnected();

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket* socket;
};

#endif // CHATCLIENTHANDLER_H