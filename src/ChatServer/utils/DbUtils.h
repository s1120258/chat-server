#ifndef DBUTILS_H
#define DBUTILS_H

#include <QString>

class QSqlQuery;

namespace DbUtils {
    QString loadQueryFromFile(const QString& queryName, const QString& filePath);
    bool checkTable(QSqlQuery& query, const QString& checkTableQuery);
}

#endif // DBUTILS_H
