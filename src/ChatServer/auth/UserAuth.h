#ifndef USERAUTH_H
#define USERAUTH_H

#include <QString>

class QSqlDatabase;

class UserAuth {
public:
    UserAuth(QSqlDatabase& db);

	void createUserTable();
    void deleteUserTable();

    bool registerUser(const QString& username, const QString& password);
    bool authenticateUser(const QString& username, const QString& password);

private:
    QSqlDatabase& m_db;
};

#endif // USERAUTH_H