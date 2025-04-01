#include "RedisManager.h"
#include <iostream>

RedisManager::RedisManager(QObject* parent)
    : QObject(parent), redis("tcp://127.0.0.1:6379") {
}

void RedisManager::connectToRedis()
{
    try {
        redis.set("test_key", "Hello, Redis!");
        auto val = redis.get("test_key");
        if (val) {
            std::cout << "Redis Value: " << *val << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Redis Error: " << e.what() << std::endl;
    }
}

void RedisManager::publishMessage(const QString& channel, const QString& message)
{
	try {
		redis.publish(channel.toStdString(), message.toStdString());
	}
	catch (const std::exception& e) {
		std::cerr << "Redis Error: " << e.what() << std::endl;
	}
}

void RedisManager::subscribeToChannel(const QString& channel)
{
	try {
		auto subscriber = redis.subscriber();
		subscriber.on_message([this](const std::string& channel, const std::string& message) {
			emit messageReceived(QString::fromStdString(channel), QString::fromStdString(message));
		});
		subscriber.subscribe(channel.toStdString());
	}
	catch (const std::exception& e) {
		std::cerr << "Redis Error: " << e.what() << std::endl;
	}
}

void RedisManager::userOnline(int userId) {
    try {
        redis.sadd("online_users", std::to_string(userId));
    }
    catch (const std::exception& e) {
        std::cerr << "Redis Error: " << e.what() << std::endl;
    }
}

void RedisManager::userOffline(int userId) {
    try {
        redis.srem("online_users", std::to_string(userId));
    }
    catch (const std::exception& e) {
        std::cerr << "Redis Error: " << e.what() << std::endl;
    }
}

bool RedisManager::isUserOnline(int userId) {
    try {
        return redis.sismember("online_users", std::to_string(userId));
    }
    catch (const std::exception& e) {
        std::cerr << "Redis Error: " << e.what() << std::endl;
        return false;
    }
}
