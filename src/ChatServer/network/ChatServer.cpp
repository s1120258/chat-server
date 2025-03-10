#include "ChatServer.h"
#include <QDebug>

ChatServer::ChatServer(QObject* parent) : QTcpServer(parent) {}
