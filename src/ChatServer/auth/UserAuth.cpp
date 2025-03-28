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

void UserAuth::createUsersTable() {
    QSqlQuery query(m_db);

    QString checkTableQuery = DbUtils::loadQueryFromFile("CHECK_USERS_TABLE", USER_QUERY_FILE);
    if (checkTableQuery.isEmpty() || !DbUtils::checkTable(query, checkTableQuery)) {
        return;
    }

    QString createTableQuery = DbUtils::loadQueryFromFile("CREATE_USERS_TABLE", USER_QUERY_FILE);
    if (createTableQuery.isEmpty() || !query.exec(createTableQuery)) {
        qDebug() << "Failed to create table:" << query.lastError().text();
        return;
    }

    qDebug() << "Users table created successfully.";
}

void UserAuth::deleteUsersTable() {
    QSqlQuery query(m_db);
    QString queryStr = DbUtils::loadQueryFromFile("DELETE_USERS_TABLE", USER_QUERY_FILE);
    if (queryStr.isEmpty() || !query.exec(queryStr)) {
        qDebug() << "Failed to delete table:" << query.lastError().text();
        return;
    }

    qDebug() << "Users table deleted successfully.";
}

bool UserAuth::registerUser(const QString& username, const QString& password)
{
    QSqlQuery query(m_db);
    QString queryStr = DbUtils::loadQueryFromFile("REGISTER_USER", USER_QUERY_FILE);
    if (queryStr.isEmpty()) {
        return false;
    }

    query.prepare(queryStr);
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
    QString queryStr = DbUtils::loadQueryFromFile("AUTHENTICATE_USER", USER_QUERY_FILE);
    if (queryStr.isEmpty()) {
        return false;
    }

    query.prepare(queryStr);
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

int UserAuth::getUserId(const QString& username) {
    QSqlQuery query(m_db);
    QString queryStr = DbUtils::loadQueryFromFile("GET_USER_ID", USER_QUERY_FILE);
    if (queryStr.isEmpty()) {
        return -1;
    }

    query.prepare(queryStr);
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Database query error:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }

    return -1;
}
