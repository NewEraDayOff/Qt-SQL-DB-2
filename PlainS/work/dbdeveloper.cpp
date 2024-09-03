#include "dbdeveloper.h"

DBDeveloper::DBDeveloper(DBTask *dbt, QObject *parent) : DBWork(dbt, parent) //конструктор
{

}

dbDev DBDeveloper::Check(const QString text, QString &error_text) //проверка разработчика
{
    DBCheck::errors e(DBCheck::E_);
    error_text = DBCheck::Check(text, e, MIN_DEV_NAME);
    dbDev dev;
    if (e == DBCheck::E_)
    {
        dev = CheckExist(text, error_text);
    }
    return dev;
}

dbDev DBDeveloper::CheckExist(const QString text, QString &error_text) //проверка разработчика
{
    error_text = QString();
    const QString name(text);
    const QString column("d_name");
    const QString condition(DBRequest::Statement(column, name));

    QString request(DBRequest::SelectWhere(T_DEVELOPER, {"id", column}, condition));
    const dbDev dev = dbtask->SelectDeveloper(request);

    if (dev.name.isEmpty())
        error_text = "Такого разработчика не существует!";
    return dev;
}

QString DBDeveloper::Check(const QString text) //проверка разработчика
{
    DBCheck::errors e(DBCheck::E_);
    QString error_text = DBCheck::Check(text, e, MIN_DEV_NAME);

    if (e == DBCheck::E_)
    {
        const QString name(text);
        const QString column("d_name");
        const QString condition(DBRequest::Statement(column, name));

        QString request(DBRequest::SelectWhere(T_DEVELOPER, {"id", column}, condition));
        dbDev dev = dbtask->SelectDeveloper(request);

        if (!dev.name.isEmpty())
            error_text = "Такой разработчик уже существует";
    }
    return error_text;
}

QString DBDeveloper::Create(const QString text) //создание разработчика
{
    QString error_text = this->Check(text);

    const QString name(text);

    QString request = DBRequest::SelectLastID(T_DEVELOPER, SEQ_DEVELOPER);
    const QString id(QString::number(dbtask->SelectDeveloper(request, {"id"}).id));
    request = DBRequest::Insert(T_DEVELOPER, {id, "'" + name + "'"});
    error_text = dbtask->Insert(request);

    return error_text;
}

QCompleter *DBDeveloper::GetCompleter() //получить комплектовщик
{
    QStringList list_name = QStringList();

    const QString d_column("d_name");
    const QString d_request(DBRequest::Select(T_DEVELOPER, d_column));

    const QList<dbDev> dev = dbtask->SelectDevelopers(d_request, {d_column});

    for (int i = 0; i < dev.count(); i++)
        list_name.append(dev[i].name);

    if (completer != nullptr)
        completer->deleteLater();

    completer = new QCompleter(list_name, this);

    return completer;
}

QString DBDeveloper::Update(const QString &condition, int id, const QString table) //переопределение обновления
{
    return DBWork::Update(condition, id, table);
}
