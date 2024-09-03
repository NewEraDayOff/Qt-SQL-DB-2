#ifndef DBDEVELOPER_H
#define DBDEVELOPER_H

#include <dbwork.h>
#include <QCompleter>

class DBDeveloper : public DBWork
{
    Q_OBJECT
public:
    explicit DBDeveloper(DBTask *dbt, QObject *parent = nullptr);
    /*! \brief Проверка разработчика на существование*/
    dbDev Check(const QString text, QString &error_text);
    dbDev CheckExist(const QString text, QString &error_text);
    QString Check(const QString text);
    /*! \brief Попытка создать разработчика*/
    QString Create(const QString text);
    QString Update(const QString &condition, int id, const QString table = "public.developer") override;
//    QString Update(const QString &statement, const QString table = "public.developer", const QString condition = nullptr) override;
//    QString Delete(int id);
    QCompleter *GetCompleter();
private:
    QCompleter *completer = nullptr;
};

#endif // DBDEVELOPER_H
