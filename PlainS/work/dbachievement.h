#ifndef DBACHIEVEMENT_H
#define DBACHIEVEMENT_H

#include <dbwork.h>

class DBAchievement : public DBWork
{
    Q_OBJECT
public:
    explicit DBAchievement(DBTask *dbt, QObject *parent = nullptr);

    QString Create(dbAchieve achieve, int g_id);
};

#endif // DBACHIEVEMENT_H
