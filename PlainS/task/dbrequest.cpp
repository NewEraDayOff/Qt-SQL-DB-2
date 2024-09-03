#include "dbrequest.h"

QString DBRequest::Statement(QString column, int value) //выражение "название столбца" = целочисленное значение
{
    const QString statement("%1 = %2");
    const QString v(QString::number(value));
    return statement.arg(column, v);
}

QString DBRequest::Statement(QString column, QString value) //выражение "название столбца" = строка
{
    const QString statement("%1 = '%2'");

    return statement.arg(column, value);
}

QString DBRequest::ConditionConnector(const QStringList conditions, const QString connector)
{
    //соединитель для множества значений "значение1" И "значение2" И "значение3" И ... И "значениеN"
    const QString connectedConditions(conditions.join(" " + connector + " "));

    return connectedConditions;
}

QString DBRequest::Insert(const QString &table, const QStringList &values) //выражение вставить в таблицу значения(перечисление значений)
{
    const QString request("INSERT INTO %1 VALUES (%2);");

    const QString val = values.join(", ");

    return request.arg(table, val);
}

QString DBRequest::Delete(const QString &table, const int id) //удалить из таблицы по индексу
{
    const QString request("DELETE FROM %1 WHERE id=%2;");

    const QString str_id(QString::number(id));

    return request.arg(table, str_id);
}

QString DBRequest::DeleteAll(const QString &table, const QString &condition) //удалить все по условию
{
    const QString request("DELETE FROM %1 WHERE %2;");

    return request.arg(table, condition);
}

QString DBRequest::Select(const QString &table, const QString &column) //выбрать "название столбца" из таблицы
{
    const QString request("SELECT %1 FROM %2;");

    return request.arg(column, table);
}

QString DBRequest::Select
(const QString &table, const QStringList &columns) //выбрать несколько столбцов из таблицы
{
    const QString request("SELECT %1 FROM %2;");

    const QString col = columns.join(", ");

    return request.arg(col, table);
}

QString DBRequest::SelectWhere
(const QString &table, const QString &column, const QString &conditions) //выбрать столбец при условиях
{
    const QString request("SELECT %1 FROM %2 WHERE %3;");

    return request.arg(column, table, conditions);
}

QString DBRequest::SelectWhere
(const QString &table, const QStringList &columns, const QString &conditions) //выбрать столбцы при условиях
{
    const QString request("SELECT %1 FROM %2 WHERE %3;");

    const QString col(columns.join(", "));

    return request.arg(col, table, conditions);
}

QString DBRequest::SelectWhereLike
(const QString &table, const QString &column, const QString &condition, const QString &like)
{
    //выбрать столбцы где присутствует конкретная строка
    const QString request("SELECT %1 FROM %2 WHERE %3 LIKE '%%4%'");

    return request.arg(column, table, condition, like);
}

QString DBRequest::SelectWhereLikeLimit //выбрать столбец где есть строка в количестве amount
(const QString &table, const QString &column, const QString &condition, const QString &like, const int amount)
{
    const QString request("SELECT %1 FROM %2 WHERE %3 LIKE '%4%' LIMIT %5");
    const QString a(QString::number(amount));

    return request.arg(column, table, condition, like, a);
}

QString DBRequest::SelectWhereLikeLimit //выбрать столбцы где есть строка в количестве amount
(const QString &table, const QStringList &columns, const QString &condition, const QString &like, const int amount)
{
    const QString request("SELECT %1 FROM %2 WHERE %3 LIKE '%%4%' LIMIT %5");
    const QString a(QString::number(amount));
    const QString col(columns.join(","));
    return request.arg(col, table, condition, like, a);
}

QString DBRequest::SelectAll(const QString &table) //выбрать все столбцы
{
    const QString request("SELECT * FROM %1;");

    return request.arg(table);
}

QString DBRequest::SelectAllWhere(const QString &table, const QString &conditions) //выбрать все столбцы при условиях
{
    const QString request("SELECT * FROM %1 WHERE %2;");

    return request.arg(table, conditions);
}

QString DBRequest::SelectByRandomLimit //выбрать столбцы, где строки выбраны случайно в количестве amount
(const QString &table, const QStringList &columns, const int amount)
{
    const QString request("SELECT %1 FROM %2 ORDER BY RANDOM() LIMIT %3;");

    const QString col = columns.join(", ");
    const QString a = QString::number(amount);

    return request.arg(col, table, a);
}

QString DBRequest::SelectLastID(const QString &table, const QString &sequence) //получить последний индекс
{
    const QString request("SELECT setval('%1', (SELECT MAX(id)+1 FROM %2));");

    return request.arg(sequence, table);
}

QString DBRequest::Update //обновить значение в таблице при условиях
(const QString &table, const QString &statements, const QString &conditions)
{
    const QString request("UPDATE %1 SET %2 WHERE %3");

    return request.arg(table, statements, conditions);
}
