#include "UserAuth.h"
#include "utils/DbUtils.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace {
	const QString USER_QUERY_FILE = "queries/user_queries.sql";

    // Hash password using SHA-256
    QString hashPassword(const QString& password) {
        return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
    }
};

UserAuth::UserAuth(QSqlDatabase& db) : m_db(db) {}

void UserAuth::createUserTable() {
    QSqlQuery query(m_db);

    QString checkTableQuery = DbUtils::loadQueryFromFile("CHECK_USER_TABLE", USER_QUERY_FILE);
    if (checkTableQuery.isEmpty() || !DbUtils::checkTable(query, checkTableQuery)) {
        return;
    }

    QString createTableQuery = DbUtils::loadQueryFromFile("CREATE_USER_TABLE", USER_QUERY_FILE);
    if (createTableQuery.isEmpty() || !query.exec(createTableQuery)) {
        qDebug() << "Failed to create table:" << query.lastError().text();
        return;
    }

    qDebug() << "Users table created successfully.";
}

void UserAuth::deleteUserTable() {
    QSqlQuery query(m_db);
    QString deleteTableQuery = DbUtils::loadQueryFromFile("DELETE_USER_TABLE", USER_QUERY_FILE);
    if (deleteTableQuery.isEmpty() || !query.exec(deleteTableQuery)) {
        qDebug() << "Failed to delete table:" << query.lastError().text();
        return;
    }

    qDebug() << "Users table deleted successfully.";
}

bool UserAuth::registerUser(const QString& username, const QString& password)
{
    QSqlQuery query(m_db);
    QString registerUserQuery = DbUtils::loadQueryFromFile("REGISTER_USER", USER_QUERY_FILE);
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
    QString authenticateUserQuery = DbUtils::loadQueryFromFile("AUTHENTICATE_USER", USER_QUERY_FILE);
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
