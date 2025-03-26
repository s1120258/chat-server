#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>

class ChatClient : public QObject {
    Q_OBJECT
public:
    explicit ChatClient(QObject* parent = nullptr);
    Q_INVOKABLE void connectToServer(const QHostAddress& address, quint16 port);

    Q_INVOKABLE void sendMessage(const QString& message);
    Q_INVOKABLE void login(const QString& username, const QString& password);

signals:
    void messageReceived(const QString& message);
    void loginResult(bool success, const QString& errorMessage);

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket socket;
};

#endif // CHATCLIENT_H
