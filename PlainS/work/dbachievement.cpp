#include "dbachievement.h"

DBAchievement::DBAchievement(DBTask *dbt, QObject *parent) : DBWork(dbt, parent) //конструктор
{

}

QString DBAchievement::Create(dbAchieve achieve, int g_id) //создание достижения
{
    int id = -1;
    QString error_text = this->CheckID(T_ACHIEVEMENT, SEQ_ACHIEVEMENT, id);
    if (error_text.isEmpty())
    {
        const QString str_id(QString::number(id));
        const QString strgame_id(QString::number(g_id));
        const QString request = DBRequest::Insert(T_ACHIEVEMENT,
                                                  {str_id,
                                                   strgame_id,
                                                   "'" + achieve.name + "'",
                                                   "'" + achieve.description + "'"});
        error_text = dbtask->Insert(request);
    }
    return error_text;
}
