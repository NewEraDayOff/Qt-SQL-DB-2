#include "dbpublisher.h"

DBPublisher::DBPublisher(DBTask *dbt, QObject *parent) : DBWork(dbt, parent) //конструктор
{

}

dbPublish DBPublisher::Check(const QString text, QString &error_text) //проверка издателя
{
    DBCheck::errors e(DBCheck::E_);
    error_text = DBCheck::Check(text, e, MIN_PUB_NAME);
    dbPublish publisher;

    if (e == DBCheck::E_)
    {
        publisher = CheckExist(text, error_text);
    }
    return publisher;
}

dbPublish DBPublisher::CheckExist(const QString text, QString &error_text) //проверка издателя
{
    const QString name(text);
    const QString column("p_name");
    const QString condition(DBRequest::Statement(column, name));

    QString request(DBRequest::SelectWhere(T_PUBLISHER, {"id", column}, condition));
    dbPublish publisher = dbtask->SelectPublisher(request);

    if (publisher.name.isEmpty())
        error_text = "Такого разработчика не существует";

    return publisher;
}

QString DBPublisher::Check(const QString text) //проверка издателя
{
    DBCheck::errors e(DBCheck::E_);
    QString error_text = DBCheck::Check(text, e, MIN_PUB_NAME);

    if (e == DBCheck::E_)
    {
        const QString name(text);
        const QString column("p_name");
        const QString condition(DBRequest::Statement(column, name));

        QString request(DBRequest::SelectWhere(T_PUBLISHER, {"id", column}, condition));
        const dbPublish publisher = dbtask->SelectPublisher(request);

        if (!publisher.name.isEmpty())
            error_text = "Такой разработчик уже существует";
    }
    return error_text;
}

QString DBPublisher::Create(const QString text) //создание издателя
{
    QString error_text = this->Check(text);
    if (!error_text.isEmpty())
        return error_text;

    const QString name(text);

    int id;
    error_text = this->CheckID(T_PUBLISHER, SEQ_PUBLISHER, id);
    if (!error_text.isEmpty())
        return error_text;
    const QString str_id(QString::number(id));
    const QString request = DBRequest::Insert(T_PUBLISHER, {str_id, "'" + name + "'"});
    error_text = dbtask->Insert(request);
    return error_text;
}

QCompleter *DBPublisher::GetCompleter() //получить комплектовщик
{
    QStringList list_name = QStringList();

    const QString d_column("p_name");
    const QString d_request(DBRequest::Select(T_PUBLISHER, d_column));

    const QList<dbPublish> pub = dbtask->SelectPublishers(d_request, {d_column});

    for (int i = 0; i < pub.count(); i++)
        list_name.append(pub[i].name);

    if (completer != nullptr)
    {
        delete completer;
        completer = nullptr;
    }

    completer = new QCompleter(list_name, this);

    return completer;
}
