#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QList>
#include "ChatClientHandler.h"

class ChatServer : public QTcpServer {
    Q_OBJECT

public:
    ChatServer(QObject* parent = nullptr);
};

#endif // CHATSERVER_H