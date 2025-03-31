#ifndef CHATCLIENTHANDLER_H
#define CHATCLIENTHANDLER_H

#include <QTcpSocket>
#include <QObject>

class ChatServer;

class ChatClientHandler : public QObject {
    Q_OBJECT

public:
    ChatClientHandler(qintptr socketDescriptor, ChatServer* chatServer, QObject* parent = nullptr);

signals:
    void disconnected();

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    void handleLogin(const QString& username, const QString& password);

    void handleFetchJoinedRooms();
    void handleFetchUsersInRoom(int roomId);

    void handleCreateRoom(const QString& roomname);
    void handleInviteUserToRoom(const QString& username, int roomId);
    void handleMessage(const QString& message);

    QTcpSocket* socket;
    ChatServer* chatServer;
    int userId;
};

#endif // CHATCLIENTHANDLER_H