#ifndef CHATCLIENTHANDLER_H
#define CHATCLIENTHANDLER_H

#include <QTcpSocket>
#include <QObject>

class ChatClientHandler : public QObject {
    Q_OBJECT

public:
    ChatClientHandler(qintptr socketDescriptor, QObject* parent = nullptr);
};

#endif // CHATCLIENTHANDLER_H