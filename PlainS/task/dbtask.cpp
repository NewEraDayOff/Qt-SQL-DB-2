#include "dbtask.h"

DBTask::DBTask(QSqlDatabase *db) //конструктор
{
    database = db;
}

bool DBTask::BeginTransaction() //начать транзакцию
{
    return database->driver()->beginTransaction();
}

bool DBTask::CommitTransaction() //совершить транзакцию
{
    return database->driver()->commitTransaction();
}

bool DBTask::RollbackTransaction() //откат транзакции
{
    return database->driver()->rollbackTransaction();
}

dbUser DBTask::SUser(QSqlQuery &query, QStringList columns) //возвращение информации о пользователе
{
    dbUser user;
    //если запрос на всю информацию о пользователе
    if (columns.isEmpty())
    {
        user.id = query.value(0).toInt();
        user.login = query.value(1).toString();
        user.password = query.value(2).toString();
        user.name = query.value(3).toString();
        user.admin = query.value(4).toString();
    }
    //если запрос на конкретную информацию о пользователе
    else
    {
        const QStringList ltemp = list_user;
        for (int i = 0; i < columns.count(); i++)
        {
            if (columns[i] == ltemp[0])
                user.id = query.value(i).toInt();
            else if (columns[i] == ltemp[1])
                user.login = query.value(i).toString();
            else if (columns[i] == ltemp[2])
                user.password = query.value(i).toString();
            else if (columns[i] == ltemp[3])
                user.name = query.value(i).toString();
            else if (columns[i] == ltemp[4])
                user.admin = query.value(i).toString();
        }
    }
    return user;
}

dbGame DBTask::SGame(QSqlQuery &query, QStringList columns) //возвращение информации об игре
{
    dbGame game;
    //если запрос на всю информацию об игре
    if (columns.isEmpty())
    {
        game.id = query.value(0).toInt();
        game.name = query.value(1).toString();
        game.price = query.value(2).toDouble();
        game.sdescript = query.value(3).toString();
        game.descript = query.value(4).toString();
    }
    //если запрос на конкретную информацию об игре
    else
    {
        const QStringList ltemp = list_game;
        for (int i = 0; i < columns.count(); i++)
        {
            if (columns[i] == ltemp[0])
                game.id = query.value(i).toInt();
            else if (columns[i] == ltemp[1])
                game.name = query.value(i).toString();
            else if (columns[i] == ltemp[2])
                game.price = query.value(i).toDouble();
            else if (columns[i] == ltemp[3])
                game.sdescript = query.value(i).toString();
            else if (columns[i] == ltemp[4])
                game.descript = query.value(i).toString();
        }
    }
    return game;
}

dbAchieve DBTask::SAchievement(QSqlQuery &query, QStringList columns) //возвращение информации о достижениях
{
    dbAchieve achieve;
    //если запрос на всю информацию о достижениях
    if (columns.isEmpty())
    {
        achieve.id = query.value(0).toInt();
        achieve.name = query.value(2).toString();
        achieve.description = query.value(3).toString();
    }
    //если запрос на конкретную информацию о достижениях
    else
    {
        const QStringList ltemp = list_achieve;
        for (int i = 0; i < columns.count(); i++)
        {
            if (columns[i] == ltemp[0])
                achieve.id = query.value(i).toInt();
            else if (columns[i] == ltemp[1])
                achieve.name = query.value(i+1).toString();
            else if (columns[i] == ltemp[2])
                achieve.description = query.value(i+1).toString();
        }
    }
    return achieve;
}

dbItem DBTask::SItem(QSqlQuery &query, QStringList columns) //возвращение информации о предмете
{
    dbItem item;
    //если запрос на всю информацию о предмете
    if (columns.isEmpty())
    {
        item.id = query.value(0).toInt();
        item.g_id = query.value(1).toInt();
        item.name = query.value(2).toString();
        item.price = query.value(3).toDouble();
        item.description = query.value(4).toString();
    }
    //если запрос на конкретную информацию о предмете
    else
    {
        const QStringList ltemp = list_item;
        for (int i = 0; i < columns.count(); i++)
        {
            if (columns[i] == ltemp[0])
                item.id = query.value(i).toInt();
            else if (columns[i] == ltemp[1])
                item.g_id = query.value(i).toInt();
            else if (columns[i] == ltemp[2])
                item.name = query.value(i).toString();
            else if (columns[i] == ltemp[3])
                item.price = query.value(i).toDouble();
            else if (columns[i] == ltemp[4])
                item.description = query.value(i).toString();
        }
    }
    return item;
}

dbReview DBTask::SReview(QSqlQuery &query, QStringList columns) //возвращение информации об отзыве
{
    dbReview review;
    //если запрос на всю информацию об отзыве
    if (columns.isEmpty())
    {
        review.id = query.value(0).toInt();
        review.g_id = query.value(1).toInt();
        review.u_id = query.value(2).toInt();
        review.text = query.value(3).toString();
        review.rating = query.value(4).toInt();
    }
    //если запрос на конкретную информацию об отзыве
    else
    {
        const QStringList ltemp = list_review;
        for (int i = 0; i < columns.count(); i++)
        {
            if (columns[i] == ltemp[0])
                review.id = query.value(i).toInt();
            else if (columns[i] == ltemp[1])
                review.g_id = query.value(i).toInt();
            else if (columns[i] == ltemp[2])
                review.u_id = query.value(i).toInt();
            else if (columns[i] == ltemp[3])
                review.text = query.value(i).toString();
            else if (columns[i] == ltemp[4])
                review.rating = query.value(i).toInt();
        }
    }
    return review;
}

dbDev DBTask::SDeveloper(QSqlQuery &query, QStringList columns) //возвращение информации о разработчике
{
    dbDev dev;
    //если запрос на всю информацию о разработчике
    if (columns.isEmpty())
    {
        dev.id = query.value(0).toInt();
        dev.name = query.value(1).toString();
    }
    //если запрос на конкретную информацию о разработчике
    else
    {
        const QStringList ltemp = list_dev;
        for (int i = 0; i < columns.count(); i++)
        {
            if (columns[i] == ltemp[0])
                dev.id = query.value(i).toInt();
            else if (columns[i] == ltemp[1])
                dev.name = query.value(i).toString();
        }
    }
    return dev;
}

dbPublish DBTask::SPublisher(QSqlQuery &query, QStringList columns) //возвращение информации об издателе
{
    dbPublish publisher;
    //если запрос на всю информацию об издателе
    if (columns.isEmpty())
    {
        publisher.id = query.value(0).toInt();
        publisher.name = query.value(1).toString();
    }
    //если запрос на конкретную информацию об издателе
    else
    {
        const QStringList ltemp = list_publisher;
        for (int i = 0; i < columns.count(); i++)
        {
            if (columns[i] == ltemp[0])
                publisher.id = query.value(i).toInt();
            else if (columns[i] == ltemp[1])
                publisher.name = query.value(i).toString();
        }
    }
    return publisher;
}

QString DBTask::Insert(const QString &request) //сформировать запрос добавления кортежа
{
    QSqlQuery query = database->exec(request);
    QString error = QString();

    if (query.lastError().type() != QSqlError::NoError)
        error = query.lastError().text();

    return error;
}

QString DBTask::Update(const QString &request) //сформировать запрос обновления кортежа
{
    QSqlQuery query = database->exec(request);
    QString error = QString();

    if (query.lastError().type() != QSqlError::NoError)
        error = query.lastError().text();

    return error;
}

dbUser DBTask::SelectUser(const QString& request, QStringList columns) //сформировать запрос данных пользователя
{
    dbUser user;

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        if (query.next())
            user = SUser(query, columns);
    }

    return user;
}

dbGame DBTask::SelectGame(const QString& request, QStringList columns) //сформировать запрос данных игры
{
    dbGame game;

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        if (query.next())
            game = SGame(query, columns);
    }
    return game;
}

dbDev DBTask::SelectDeveloper(const QString& request, QStringList columns) //сформировать запрос данных разработчика
{
    dbDev dev;

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        if (query.next())
            dev = SDeveloper(query, columns);
    }
    return dev;
}

dbPublish DBTask::SelectPublisher(const QString& request, QStringList columns) //сформировать запрос данных издателя
{
    dbPublish publisher;

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        if (query.next())
            publisher = SPublisher(query, columns);
    }
    return publisher;
}

dbReview DBTask::SelectReview(const QString &request, QStringList columns) //сформировать запрос данных обзора
{
    dbReview review;

    QSqlQuery query = database->exec(request);
    if (query.lastError().type() == QSqlError::NoError)
    {
        if (query.next())
            review = SReview(query, columns);
    }
    return review;
}

dbLibGame DBTask::SelectGameForLibrary(const QString &query) //сформировать запрос данных игры для библиотеки
{
    dbLibGame game;

    QSqlQuery q = database->exec(query);

    if (q.lastError().type() == QSqlError::NoError)
    {
        if (q.next())
        {
            int g_id = q.value(1).toInt();
            game.g_id = g_id;
            QString strg_id(QString::number(g_id));
            int timep = q.value(3).toInt();
            game.timeplayed = timep;
            bool isI = q.value(4).toBool();
            game.isInstalled = isI;
            QString request("SELECT g_name FROM %1 WHERE id=" + strg_id);
            QSqlQuery q2 = database->exec(request.arg(T_GAME));
            if (q2.next())
            {
                game.g_name = q2.value(0).toString();
            }
        }
    }
    return game;
}

int DBTask::Select(const QString& request) //сформировать запрос целочисленных данных
{
    int id = -1;

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        if (query.next())
            id = query.value(0).toInt();
    }
    return id;
}

QList<int> DBTask::SelectID(const QString& request) //сформировать запрос массива целочисленных данных
{
    QList<int> lid = QList<int>();

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        while (query.next())
            lid.append(query.value(0).toInt());
    }
    return lid;
}

QList<dbUser> DBTask::SelectUsers(const QString &request, QStringList columns) //сформировать запрос данных пользователей
{
    QList<dbUser> users;

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        while (query.next())
        {
            dbUser user = SUser(query, columns);
            users.append(user);
        }
    }
    return users;
}

QList<dbGame> DBTask::SelectGames(const QString &request, QStringList columns) //сформировать запрос данных игр
{
    QList<dbGame> games;

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        while (query.next())
        {
            dbGame game = SGame(query, columns);
            games.append(game);
        }
    }
    return games;
}

QList<dbDev> DBTask::SelectDevelopers(const QString &request, QStringList columns) //сформировать запрос данных разработчиков
{
    QList<dbDev> devs;

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        while (query.next())
        {
            dbDev dev = SDeveloper(query, columns);
            devs.append(dev);
        }
    }
    return devs;
}

QList<dbPublish> DBTask::SelectPublishers(const QString &request, QStringList columns) //сформировать запрос данных издателей
{
    QList<dbPublish> publishers;

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        while (query.next())
        {
            dbPublish publisher = SPublisher(query, columns);
            publishers.append(publisher);
        }
    }

    return publishers;
}

QList<dbAchieve> DBTask::SelectAchievements(const QString &request, QStringList columns) //сформировать запрос данных достижений
{
    QList<dbAchieve> achieves;

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        while (query.next())
        {
            dbAchieve achieve = SAchievement(query, columns);
            achieves.append(achieve);
        }
    }
    return achieves;
}

QList<dbItem> DBTask::SelectItems(const QString &request, QStringList columns) //сформировать запрос данных предметов
{
    QList<dbItem> items;

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        while (query.next())
        {
            dbItem item = SItem(query, columns);
            items.append(item);
        }
    }
    return items;
}

QList<dbReview> DBTask::SelectReviews(const QString &request, QStringList columns) //сформировать запрос данных обзоров
{
    QList<dbReview> reviews;

    QSqlQuery query = database->exec(request);

    if (query.lastError().type() == QSqlError::NoError)
    {
        while (query.next())
        {
            dbReview review = SReview(query, columns);
            reviews.append(review);
        }
    }
    return reviews;
}

QList<dbLibGame> DBTask::SelectGamesForLibrary(const QString &query) //сформировать запрос данных игр для библиотеки
{
    QList<dbLibGame> list = QList<dbLibGame>();

    QSqlQuery q = database->exec(query);

    if (q.lastError().type() == QSqlError::NoError)
    {
        while (q.next())
        {
            dbLibGame libgame;

            int g_id = q.value(1).toInt();
            libgame.g_id = g_id;
            QString strg_id(QString::number(g_id));
            int timep = q.value(3).toInt();
            libgame.timeplayed = timep;
            bool isI = q.value(4).toBool();
            libgame.isInstalled = isI;
            QString request("SELECT g_name FROM %1 WHERE id=" + strg_id);
            QSqlQuery q2 = database->exec(request.arg(T_GAME));
            if (q2.next())
            {
                libgame.g_name = q2.value(0).toString();
            }

            list.append(libgame);

        }
    }

    return list;
}
