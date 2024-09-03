#ifndef DBITEM_H
#define DBITEM_H

#include <dbwork.h>

class DBItem : public DBWork
{
    Q_OBJECT
public:
    explicit DBItem(DBTask *dbt, QObject *parent = nullptr);

    QString Create(dbItem item, int g_id);
};

#endif // DBITEM_H
