#ifndef DBUSER_H
#define DBUSER_H

#include <dbwork.h>

class DBUser : public DBWork
{
    Q_OBJECT
public:
    explicit DBUser(DBTask *dbt, QObject *parent = nullptr);
    dbUser Check(const QString text, QString &error_text);
    dbUser CheckOnDelete(const QString text, QString &error_text, QString yourName);
    QCompleter *GetCompleter();
private:
    QCompleter *completer = nullptr;
};

#endif // DBUSER_H
