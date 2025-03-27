#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>

class ChatClient : public QObject {
    Q_OBJECT
public:
    explicit ChatClient(QObject* parent = nullptr);
    Q_INVOKABLE void connectToServer(const QHostAddress& address, quint16 port);

    Q_INVOKABLE void sendMessage(const QString& message);
    Q_INVOKABLE void login(const QString& username, const QString& password);
    Q_INVOKABLE void fetchJoinedRooms();
    Q_INVOKABLE void createRoom(const QString& roomName);
    Q_INVOKABLE void inviteUserToRoom(const QString& username, const QString& roomName);

signals:
    void messageReceived(const QString& message);
    void loginResult(bool success, const QString& errorMessage);
    void joinedRoomsReceived(const QStringList& rooms);
    void roomCreated(bool success, const QString& roomName);
    void roomJoined(bool success, const QString& roomName);
    void invitationReceived(const QString& roomName, const QString& invitedBy);

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket socket;
};

#endif // CHATCLIENT_H
