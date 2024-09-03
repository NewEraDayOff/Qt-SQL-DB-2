#ifndef DBREQUEST_H
#define DBREQUEST_H

#include <QObject>

class DBRequest
{
public:
    /*! \brief column = value;*/
    static QString Statement(QString column, int value);
    /*! \brief column = 'value';*/
    static QString Statement(QString column, QString value);

    /*! \brief condition1 connector condition2 connector condition3...;*/
    static QString ConditionConnector(const QStringList conditions, const QString connector);

    /*! \brief INSERT INTO table VALUES values[];*/
    static QString Insert(const QString &table, const QStringList &values);
    /*! \brief DELETE FROM table WHERE id=index;*/
    static QString Delete(const QString &table, const int index);
    /*! \brief DELETE FROM table WHERE condition;*/
    static QString DeleteAll(const QString &table, const QString &condition);

//    static QString Insert(const QString &table, const QStringList &columns, const QStringList &values);

    /*! \brief SELECT column FROM table;*/
    static QString Select(const QString &table, const QString &column);
    /*! \brief SELECT columns[] FROM table;*/
    static QString Select(const QString &table, const QStringList &columns);
    /*! \brief SELECT column FROM table WHERE condition;*/
    static QString SelectWhere(const QString &table, const QString &column, const QString &conditions);
    /*! \brief SELECT columns[] FROM table WHERE condition;*/
    static QString SelectWhere(const QString &table, const QStringList &columns, const QString &conditions);
    /*! \brief SELECT column FROM table WHERE condition LIKE 'like%';*/
    static QString SelectWhereLike(const QString &table, const QString &column, const QString &condition, const QString &like);
    /*! \brief SELECT column FROM table WHERE condition LIKE 'like' LIMIT amount;*/
    static QString SelectWhereLikeLimit(const QString &table, const QString &column, const QString &condition, const QString &like, const int amount);
    /*! \brief SELECT column FROM table WHERE condition LIKE 'like' LIMIT amount;*/
    static QString SelectWhereLikeLimit(const QString &table, const QStringList &columns, const QString &condition, const QString &like, const int amount);

    /*! \brief SELECT * FROM table;*/
    static QString SelectAll(const QString &table);
    /*! \brief SELECT * FROM table WHERE condition;*/
    static QString SelectAllWhere(const QString &table, const QString &conditions);

    /*! \brief SELECT columns[] FROM table ORDER BY RANDOM() LIMIT amount;*/
    static QString SelectByRandomLimit(const QString &table, const QStringList &columns, const int amount);

    /*! \brief SELECT setval('sequence', (SELECT MAX(id)+1 FROM table));*/
    static QString SelectLastID(const QString &table, const QString &sequence);

    /*! \brief UPDATE table SET statements WHERE conditions;*/
    static QString Update(const QString &table, const QString &statements, const QString &conditions);
};

#endif // DBREQUEST_H
