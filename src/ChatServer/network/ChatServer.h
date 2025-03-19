#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "ChatClientHandler.h"
#include <QTcpServer>
#include <QList>

class QSqlDatabase;

class ChatServer : public QTcpServer {
    Q_OBJECT

public:
    ChatServer(QSqlDatabase& db, QObject* parent = nullptr);

    void startServer(quint16 port);

	void createRoomsTable();
	void createMessagesTable();

    bool createRoom(const QString& roomName);
    bool joinRoom(int userId, int roomId);
    bool leaveRoom(int userId, int roomId);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QSqlDatabase& m_db;

    QList<ChatClientHandler*> clients;
};

#endif // CHATSERVER_H