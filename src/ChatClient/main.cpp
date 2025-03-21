#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/chatclient/qml/Login.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
