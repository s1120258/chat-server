#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>

class ChatClient : public QObject {
    Q_OBJECT
public:
    explicit ChatClient(QObject* parent = nullptr);
    Q_INVOKABLE void connectToServer(const QHostAddress& address, quint16 port);

    Q_INVOKABLE void login(const QString& username, const QString& password);
    Q_INVOKABLE void fetchJoinedRooms();
    Q_INVOKABLE void createRoom(const QString& roomName);
    Q_INVOKABLE void fetchUsersInRoom(int roomId);
    Q_INVOKABLE void inviteUserToRoom(const QString& username, int roomId);
    Q_INVOKABLE void fetchMessages(int roomId);
    Q_INVOKABLE void sendMessage(int roomId, const QString& message);

signals:
    void loginResult(bool success, const QString& errorMessage);
    void joinedRoomsReceived(const QStringList& rooms);
    void roomCreated(bool success, const QString& roomName);
    void usersInRoomReceived(const QStringList& users);
    void userInvited(bool success, const QString& userName);
    void messagesReceived(const QStringList& messages);
    void messageReceived(const QString& message);

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket socket;
};

#endif // CHATCLIENT_H
