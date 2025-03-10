#ifndef USERAUTH_H
#define USERAUTH_H

#include <QtSql/QSqlDatabase>
#include <QString>

class UserAuth {
public:
    UserAuth(QSqlDatabase& db);

	bool createUserTable();
    bool registerUser(const QString& username, const QString& password);
    bool authenticateUser(const QString& username, const QString& password);
    bool deleteUserTable();

private:
    QSqlDatabase& m_db;
};

#endif // USERAUTH_H