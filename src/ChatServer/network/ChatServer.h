#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "ChatClientHandler.h"
#include <QTcpServer>
#include <QList>

class QSqlDatabase;

class ChatServer : public QTcpServer {
    Q_OBJECT

public:
    ChatServer(QObject* parent = nullptr);

    void startServer(quint16 port);

	void createRoomsTable(QSqlDatabase& db);
	void createMessagesTable(QSqlDatabase& db);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QList<ChatClientHandler*> clients;
};

#endif // CHATSERVER_H