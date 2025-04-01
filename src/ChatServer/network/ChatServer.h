#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "ChatClientHandler.h"
#include <QTcpServer>
#include <QList>
#include <QVector>
#include <QVariantMap>

class QSqlDatabase;
class UserAuth;
class RedisManager;

class ChatServer : public QTcpServer {
    Q_OBJECT

public:
    ChatServer(QSqlDatabase& db, QObject* parent = nullptr);

	QVector<QVariantMap> fetchJoinedRooms(int userId);
    QVector<QVariantMap> fetchUsersInRoom(int roomId);

    bool createRoom(const QString& roomName, int userId);
    bool joinRoom(int userId, int roomId);
    bool joinRoom(const QString& userName, int roomId);
    bool leaveRoom(int userId, int roomId);

    QVector<QVariantMap> fetchMessages(int roomId);
	bool sendMessage(int userId, int roomId, const QString& message);
    void onMessageReceived(const QString& channel, const QString& message);
    void publishMessage(const QString& channel, const QString& message);

    bool registerUser(const QString& username, const QString& password);
    bool authenticateUser(const QString& username, const QString& password);

    int getUserId(const QString& username) const;
    QString getUserName(int userId) const;

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    void startServer(quint16 port);

    void createRoomsTable();
    void createUserRoomsTable();
    void createMessagesTable();

    QSqlDatabase& m_db;
    QList<ChatClientHandler*> clients;
    UserAuth* userAuth;
    RedisManager* redisManager;
};

#endif // CHATSERVER_H