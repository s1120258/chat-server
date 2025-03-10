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

private:
    sw::redis::Redis redis;
};

#endif // REDISMANAGER_H