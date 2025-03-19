#include "DbUtils.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace DbUtils {
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
