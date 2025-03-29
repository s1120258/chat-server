#include "DbUtils.h"
#include "env_loader.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace DbUtils {
	void connectToPostgreSQL(QSqlDatabase& db) {
		try {
			EnvLoader::loadEnv(".env");
		}
		catch (const std::exception& e) {
			qDebug() << "Error loading environment variables:" << e.what();
			return;
		}

		db = QSqlDatabase::addDatabase("QPSQL");

		char* dbHost = nullptr;
		char* dbName = nullptr;
		char* dbUser = nullptr;
		char* dbPassword = nullptr;
		char* dbPort = nullptr;

		size_t len;
		_dupenv_s(&dbHost, &len, "DB_HOST");
		_dupenv_s(&dbName, &len, "DB_NAME");
		_dupenv_s(&dbUser, &len, "DB_USER");
		_dupenv_s(&dbPassword, &len, "DB_PASSWORD");
		_dupenv_s(&dbPort, &len, "DB_PORT");

		db.setHostName(dbHost);
		db.setDatabaseName(dbName);
		db.setUserName(dbUser);
		db.setPassword(dbPassword);
		db.setPort(std::atoi(dbPort)); // Default PostgreSQL port

		if (!db.open()) {
			qDebug() << "Failed to connect to PostgreSQL:" << db.lastError().text();
		}
		else {
			qDebug() << "Connected to PostgreSQL!";
		}

		free(dbHost);
		free(dbName);
		free(dbUser);
		free(dbPassword);
		free(dbPort);
	}

	QString loadQueryFromFile(const QString& queryName, const QString& filePath) {
		QFile file(filePath);
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

	bool checkTable(QSqlQuery& query, const QString& checkTableQuery) {
		if (!query.exec(checkTableQuery)) {
			qDebug() << "Failed to check table existence:" << query.lastError().text();
			return false;
		}
		if (query.next() && query.value(0).toBool()) {
			qDebug() << "Table already exists.";
			return false;
		}
		return true;
	}
};
