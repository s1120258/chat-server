#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>

class ChatClient : public QObject {
    Q_OBJECT
public:
    explicit ChatClient(QObject* parent = nullptr);
    void connectToServer(const QHostAddress& address, quint16 port);
    void sendMessage(const QString& message);

signals:
    void messageReceived(const QString& message);

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket socket;
};

#endif // CHATCLIENT_H
