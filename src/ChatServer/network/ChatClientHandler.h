#ifndef CHATCLIENTHANDLER_H
#define CHATCLIENTHANDLER_H

#include <QTcpSocket>
#include <QObject>

class UserAuth;

class ChatClientHandler : public QObject {
    Q_OBJECT

public:
    ChatClientHandler(qintptr socketDescriptor, UserAuth* userAuth, QObject* parent = nullptr);

signals:
    void disconnected();

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    void handleLogin(const QString& username, const QString& password);

    QTcpSocket* socket;
    UserAuth* userAuth;
};

#endif // CHATCLIENTHANDLER_H