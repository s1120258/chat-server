#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "ChatClientHandler.h"
#include <QTcpServer>
#include <QList>
#include <QVector>
#include <QVariantMap>

class QSqlDatabase;

class ChatServer : public QTcpServer {
    Q_OBJECT

public:
    ChatServer(QSqlDatabase& db, QObject* parent = nullptr);

    void startServer(quint16 port);

	void createRoomsTable();
    void createUserRoomsTable();
	void createMessagesTable();

    bool createRoom(const QString& roomName);
    bool joinRoom(int userId, int roomId);
    bool leaveRoom(int userId, int roomId);

    QVector<QVariantMap> fetchMessages(int roomId);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QSqlDatabase& m_db;

    QList<ChatClientHandler*> clients;
};

#endif // CHATSERVER_H