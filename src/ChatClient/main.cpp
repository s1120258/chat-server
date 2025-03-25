#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtWebSockets/QWebSocket>
#include <QtQml/QQmlContext>
#include "client/ChatClient.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ChatClient chatClient;
    engine.rootContext()->setContextProperty("chatClient", &chatClient);

    // Connect to the chat server
    QHostAddress serverAddress = QHostAddress::LocalHost;
    quint16 serverPort = 12345;
    chatClient.connectToServer(serverAddress, serverPort);

    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/chatclient/qml/Login.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
