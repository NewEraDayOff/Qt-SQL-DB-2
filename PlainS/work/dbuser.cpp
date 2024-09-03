#include "dbuser.h"

DBUser::DBUser(DBTask *dbt, QObject *parent) : DBWork(dbt, parent) //конструктор
{

}

dbUser DBUser::Check(const QString text, QString &error_text) //проверка пользователя
{
    DBCheck::errors e(DBCheck::E_);
    error_text = DBCheck::Check(text, e, 4);
    dbUser user;
    if (e == DBCheck::E_)
    {
        user = CheckOnDelete(text, error_text, "");
    }
    return user;
}

dbUser DBUser::CheckOnDelete(const QString text, QString &error_text, QString yourName) //проверка пользователя при удалении
{
    const QString name(text);
    const QString column("u_login");
    const QString condition(DBRequest::Statement(column, name));

    QString request(DBRequest::SelectWhere(T_USER, {"id", column, "u_admin"}, condition));
    dbUser user = dbtask->SelectUser(request, {"id", column, "u_admin"});

    if (user.login.isEmpty())
        error_text = "Такого разработчика не существует!";
    else if (user.login == yourName)
        error_text = "Нельзя удалить свой аккаунт";
    else if (!user.admin.isEmpty())
        error_text = "Нельзя удалить другого администратора";

    return user;
}

QCompleter *DBUser::GetCompleter() //получить комплектовщик
{
    QStringList list_name = QStringList();

    const QString column("u_login");
    const QString request(DBRequest::Select(T_USER, {"id", column}));

    const QList<dbUser> user = dbtask->SelectUsers(request, {"id", column});

    for (int i = 0; i < user.count(); i++)
        list_name.append(user[i].login);

    if (completer != nullptr)
        completer->deleteLater();

    completer = new QCompleter(list_name, this);

    return completer;
}
