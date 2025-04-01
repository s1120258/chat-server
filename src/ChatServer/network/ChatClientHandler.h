#ifndef CHATCLIENTHANDLER_H
#define CHATCLIENTHANDLER_H

#include <QTcpSocket>
#include <QObject>

class ChatServer;

class ChatClientHandler : public QObject {
    Q_OBJECT

public:
    ChatClientHandler(qintptr socketDescriptor, ChatServer* chatServer, QObject* parent = nullptr);

    void sendMessage(const QString& message);

signals:
    void disconnected();

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    void handleLogin(const QString& username, const QString& password);

    void handleFetchJoinedRooms();
    void handleCreateRoom(const QString& roomname);

    void handleFetchUsersInRoom(int roomId);
    void handleInviteUserToRoom(const QString& username, int roomId);

    void handleFetchMessages(int roomId);
    void handleSendMessage(int roomId, const QString& message);

    QTcpSocket* socket;
    ChatServer* chatServer;
    int userId;
};

#endif // CHATCLIENTHANDLER_H