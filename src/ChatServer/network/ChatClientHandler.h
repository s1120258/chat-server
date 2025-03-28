#ifndef CHATCLIENTHANDLER_H
#define CHATCLIENTHANDLER_H

#include <QTcpSocket>
#include <QObject>

class ChatServer;
class UserAuth;

class ChatClientHandler : public QObject {
    Q_OBJECT

public:
    ChatClientHandler(qintptr socketDescriptor, ChatServer* chatServer, UserAuth* userAuth, QObject* parent = nullptr);

signals:
    void disconnected();

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    void handleLogin(const QString& username, const QString& password);
    void handleFetchJoinedRooms();
    void handleCreateRoom(const QString& roomname);
    void handleInviteUser(const QString& username, const QString& roomname);
    void handleMessage(const QString& message);

    QTcpSocket* socket;
    ChatServer* chatServer;
    UserAuth* userAuth;
    int userId;
};

#endif // CHATCLIENTHANDLER_H