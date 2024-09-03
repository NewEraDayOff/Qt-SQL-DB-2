#include "dbwork.h"

DBWork::DBWork(DBTask *dbt, QObject *parent) : QObject(parent) //конструктор
{
    dbtask = dbt;
}

QString DBWork::CheckID(const QString &table, const QString &sequence, int &id) //получение последнего индекса
{
    const QString request(DBRequest::SelectLastID(table, sequence));
    int temp_id = dbtask->Select(request);
    QString error_text = QString();
    if (temp_id == -1)
    {
        error_text = "DBWORK: ID_SELECT_ERROR";
    }
    else
        id = temp_id;

    return error_text;
}

QString DBWork::Delete(const QString &table, int id) //удаление по индексу
{
    QString error_text = QString();

    const QString request(DBRequest::Delete(table, id));

    error_text = dbtask->Insert(request);

    return error_text;
}

QString DBWork::DeleteAll(const QString &table, const QString &condition) //удаление по условию
{
    QString error_text = QString();

    const QString request(DBRequest::DeleteAll(table, condition));

    error_text = dbtask->Insert(request);

    return error_text;
}

QString DBWork::Update(const QString &statement, int id, const QString table) //изменение по индексу
{
    QString error_text = QString();

    const QString request(DBRequest::Update(table, statement, "id=" + QString::number(id)));

    error_text = dbtask->Update(request);

    return error_text;
}

QString DBWork::Update(const QString &statement, const QString table, const QString condition) //изменение по условию
{
    QString error_text = QString();

    const QString request(DBRequest::Update(table, statement, condition));

    error_text = dbtask->Update(request);

    return error_text;
}

int DBWork::GetIDAmount(const QString &table, const QString &condition) //получить количество элементов по индексу
{
    const QString request(DBRequest::SelectWhere(table, "id", condition));

    QList<int> ids = dbtask->SelectID(request);

    return ids.count() > 0 ? ids.count() : -1;
}

int DBWork::GetID(const QString &condition, const QString table) //получить индекс
{
    const QString request(DBRequest::SelectWhere(table, "id", condition));

    return dbtask->Select(request);
}

int DBWork::GetSomeAmount(const QString &table, const QString &some, const QString &condition) //получить количество элементов по условию
{
    const QString request(DBRequest::SelectWhere(table, some, condition));

    QList<int> ids = dbtask->SelectID(request);

    return ids.count() > 0 ? ids.count() : -1;
}
