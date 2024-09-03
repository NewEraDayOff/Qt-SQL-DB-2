#ifndef DBWORK_H
#define DBWORK_H

#include <QObject>
#include <QLineEdit>
#include <QCompleter>

#include "dbcheck.h"
#include "task/dbtask.h"
#include "task/dbrequest.h"

class DBWork : public QObject
{
    Q_OBJECT
public:
    explicit DBWork(DBTask *dbt, QObject *parent = nullptr);

    QString CheckID(const QString &table, const QString &sequence, int &id);
    QString Delete(const QString &table, int id);
    QString DeleteAll(const QString &table, const QString &condition);
    int GetIDAmount(const QString &table, const QString &condition);
    int GetSomeAmount(const QString &table, const QString &some, const QString &condition);

//    virtual QString Create(const QString text) = 0;
//    virtual QString Check(const QString text) = 0;
protected:
    DBTask *dbtask = nullptr;
    /*! \brief SELECT id FROM table WHERE condition*/
    virtual int GetID(const QString &condition, const QString table);
    virtual QString Update(const QString &statement, int id, const QString table);
    virtual QString Update(const QString &statement, const QString table, const QString condition = nullptr);

signals:

};

#endif // DBWORK_H
