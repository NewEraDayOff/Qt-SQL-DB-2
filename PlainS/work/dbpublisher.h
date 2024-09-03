#ifndef DBPUBLISHER_H
#define DBPUBLISHER_H

#include <dbwork.h>
#include <QCompleter>

class DBPublisher : public DBWork
{
    Q_OBJECT
public:
    explicit DBPublisher(DBTask *dbt, QObject *parent = nullptr);
    /*! \brief Проверка издателя на существование*/
    dbPublish Check(const QString text, QString &error_text);
    dbPublish CheckExist(const QString text, QString &error_text);
    QString Check(const QString text);
    /*! \brief Попытка создать издателя*/
    QString Create(const QString text);
    QCompleter *GetCompleter();
private:
    QCompleter *completer = nullptr;
};

#endif // DBPUBLISHER_H
