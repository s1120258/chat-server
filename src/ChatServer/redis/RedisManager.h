#ifndef REDISMANAGER_H
#define REDISMANAGER_H

#include <sw/redis++/redis++.h>
#include <QObject>
#include <QString>

class RedisManager : public QObject {
    Q_OBJECT

public:
    RedisManager(QObject* parent = nullptr);

	void connectToRedis();
    void publishMessage(const QString& channel, const QString& message);
    void subscribeToChannel(const QString& channel);

    void userOnline(int userId);
    void userOffline(int userId);
    bool isUserOnline(int userId);

signals:
    void messageReceived(const QString& channel, const QString& message);

private:
    sw::redis::Redis redis;
};

#endif // REDISMANAGER_H