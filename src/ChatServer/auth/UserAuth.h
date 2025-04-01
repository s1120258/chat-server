#ifndef USERAUTH_H
#define USERAUTH_H

#include <QString>

class QSqlDatabase;

class UserAuth {
public:
    UserAuth(QSqlDatabase& db);

	void createUsersTable();
    void deleteUsersTable();

    bool registerUser(const QString& username, const QString& password);
    bool authenticateUser(const QString& username, const QString& password);

    int getUserId(const QString& username) const;
    QString getUserName(int userId) const;

private:
    QSqlDatabase& m_db;
};

#endif // USERAUTH_H