#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>

#include <sw/redis++/redis++.h>
#include <iostream>

void connectToPostgreSQL() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

    db.setHostName("localhost");  // Use your PostgreSQL host
    db.setDatabaseName("chatdb"); // Your database name
    db.setUserName("chatuser");   // Your username
    db.setPassword("chatpassword"); // Your password
    db.setPort(5432); // Default PostgreSQL port

    if (!db.open()) {
        qDebug() << "Failed to connect to PostgreSQL:" << db.lastError().text();
    }
    else {
        qDebug() << "Connected to PostgreSQL!";
    }
}

void connectToRedis() {
    try {
        auto redis = sw::redis::Redis("tcp://127.0.0.1:6379");
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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //connectToPostgreSQL();
    connectToRedis();

    return a.exec();
}
