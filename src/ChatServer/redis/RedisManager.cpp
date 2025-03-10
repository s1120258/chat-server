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
