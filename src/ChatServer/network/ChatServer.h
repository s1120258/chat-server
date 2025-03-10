#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QList>
#include "ChatClientHandler.h"

class ChatServer : public QTcpServer {
    Q_OBJECT

public:
    ChatServer(QObject* parent = nullptr);

    void startServer(quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QList<ChatClientHandler*> clients;
};

#endif // CHATSERVER_H