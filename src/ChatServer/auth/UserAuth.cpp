#include "UserAuth.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace {
    // Hash password using SHA-256
    QString hashPassword(const QString& password) {
        return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
    }

    QString loadQueryFromFile(const QString& queryName) {
        QFile file("queries/user_queries.sql");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open queries file:" << file.errorString();
            return QString();
        }

        QTextStream in(&file);
        QString query;
        bool found = false;
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith(queryName)) {
                found = true;
                continue;
            }
            if (found) {
                if (line.trimmed().isEmpty()) break;
                query += line + "\n";
            }
        }
        return query.trimmed();
    }

    bool checkUserTable(QSqlQuery& query, const QString& checkTableQuery)
    {
        if (!query.exec(checkTableQuery)) {
            qDebug() << "Failed to check table existence:" << query.lastError().text();
            return false;
        }

        if (query.next() && query.value(0).toBool()) {
            qDebug() << "Users table already exists.";
            return false;
        }

        return true;
    }
};

UserAuth::UserAuth(QSqlDatabase& db) : m_db(db) {}

bool UserAuth::createUserTable() {
    QSqlQuery query(m_db);

    QString checkTableQuery = loadQueryFromFile("CHECK_USER_TABLE");
    if (checkTableQuery.isEmpty() || !checkUserTable(query, checkTableQuery)) {
        return false;
    }

    QString createTableQuery = loadQueryFromFile("CREATE_USER_TABLE");
    if (createTableQuery.isEmpty() || !query.exec(createTableQuery)) {
        qDebug() << "Failed to create table:" << query.lastError().text();
        return false;
    }

    qDebug() << "Users table created successfully.";
    return true;
}

bool UserAuth::registerUser(const QString& username, const QString& password)
{
    QSqlQuery query(m_db);
    QString registerUserQuery = loadQueryFromFile("REGISTER_USER");
    if (registerUserQuery.isEmpty()) {
        return false;
    }

    query.prepare(registerUserQuery);
    query.bindValue(":username", username);
    query.bindValue(":password_hash", hashPassword(password));

    if (!query.exec()) {
        qDebug() << "Database query error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool UserAuth::authenticateUser(const QString& username, const QString& password) {
    QSqlQuery query(m_db);
    QString authenticateUserQuery = loadQueryFromFile("AUTHENTICATE_USER");
    if (authenticateUserQuery.isEmpty()) {
        return false;
    }

    query.prepare(authenticateUserQuery);
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Database query error:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QString storedHash = query.value(0).toString();
        QString inputHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
        return storedHash == inputHash;
    }

    return false;
}

bool UserAuth::deleteUserTable() {
    QSqlQuery query(m_db);
    QString deleteTableQuery = loadQueryFromFile("DELETE_USER_TABLE");
    if (deleteTableQuery.isEmpty() || !query.exec(deleteTableQuery)) {
        qDebug() << "Failed to delete table:" << query.lastError().text();
        return false;
    }

    qDebug() << "Users table deleted successfully.";
    return true;
}
