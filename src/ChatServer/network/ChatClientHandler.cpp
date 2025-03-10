#include "ChatClientHandler.h"
#include <QDebug>

ChatClientHandler::ChatClientHandler(qintptr socketDescriptor, QObject* parent)
    : QObject(parent) {
}
