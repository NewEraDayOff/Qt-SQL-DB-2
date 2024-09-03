#include "dbgame.h"

DBGame::DBGame(DBTask *dbt, QObject *parent) : DBWork(dbt, parent) //конструктор
{

}

dbGame DBGame::Check(const QString text, QString &error_text) //проверка игры
{
    DBCheck::errors e(DBCheck::E_);
    error_text = DBCheck::Check(text, e, MIN_GAME_NAME);
    dbGame game;
    if (e == DBCheck::E_)
    {
        const QString name(text);
        const QString column("g_name");
        const QString condition(DBRequest::Statement(column, name));

        QString request(DBRequest::SelectWhere(T_GAME, {"id", column}, condition));

        game = dbtask->SelectGame(request, {"id", column});

        if (game.name.isEmpty())
            error_text = "Такой игры не существует!";
    }
    return game;
}

QString DBGame::Check(const QString text) //проверка игры
{

    DBCheck::errors e(DBCheck::E_);
    QString error_text = DBCheck::Check(text, e, MIN_GAME_NAME);

    if (e == DBCheck::E_)
    {
        const QString name(text);
        const QString column("g_name");
        const QString condition(DBRequest::Statement(column, name));
        const QString request(DBRequest::SelectWhere(T_GAME, {"id", column}, condition));

        const dbGame game = dbtask->SelectGame(request, {"id", column});

        if (!game.name.isEmpty())
            error_text = "Игра с таким именем уже существует";
    }
    return error_text;
}

dbGame *DBGame::GetGame(const QString name, QString &error_text) //получить игру
{
    dbGame game;

    const QString column("g_name");
    const QString condition(DBRequest::Statement(column, name));

    QString request(DBRequest::SelectAllWhere(T_GAME, condition));

    game = dbtask->SelectGame(request);

    if (game.name.isEmpty())
        error_text = "Такой игры не существует!";
    else
        BuildGame(game);

    return new dbGame(game);
}

dbLibGame DBGame::GetGame(int game_id, int user_id) //получить игру
{
    QString strgame_id(QString::number(game_id));
    QString struser_id(QString::number(user_id));
    QString request(DBRequest::SelectAllWhere(T_LIBRARY, "g_id=" + strgame_id + " AND u_id=" + struser_id));
    dbLibGame game = dbtask->SelectGameForLibrary(request);
    return game;
}

dbGame DBGame::GetGame(int game_id) //получить игру
{
    QString strgame_id(QString::number(game_id));
    QString request(DBRequest::SelectAllWhere(T_GAME, "id=" + strgame_id));
    return dbtask->SelectGame(request);
}

QList<dbDev> DBGame::GetDeveloper(int game_id) //получить разработчиков
{
    QList<dbDev> devs = QList<dbDev>();

    QString request(DBRequest::SelectWhere(T_GAME_DEVELOPER, "d_id", "g_id="+QString::number(game_id)));
    QList<int> list_id = dbtask->SelectID(request);

    if (!list_id.isEmpty())
    {
        QString str_id = "id=" + QString::number(list_id.first());
        for (int i = 1; i < list_id.count(); i++)
            str_id.append(" OR id=" + QString::number(list_id[i]));
        request = DBRequest::SelectAllWhere(T_DEVELOPER, str_id);
    }
    devs = dbtask->SelectDevelopers(request);
    return devs;
}

QList<dbPublish> DBGame::GetPublisher(int game_id) //получить издателей
{
    QList<dbPublish> pubs = QList<dbPublish>();

    QString request(DBRequest::SelectWhere(T_GAME_PUBLISHER, "p_id", "g_id="+QString::number(game_id)));
    QList<int> list_id = dbtask->SelectID(request);

    if (!list_id.isEmpty())
    {
        QString str_id = "id=" + QString::number(list_id.first());
        for (int i = 1; i < list_id.count(); i++)
            str_id.append(" OR id=" + QString::number(list_id[i]));
        request = DBRequest::SelectAllWhere(T_PUBLISHER, str_id);
    }
    pubs = dbtask->SelectPublishers(request);
    return pubs;
}

QList<dbAchieve> DBGame::GetAchieve(int game_id) //получить достижения
{
    QList<dbAchieve> achieves = QList<dbAchieve>();
    const QString str_id = "g_id=" + QString::number(game_id);
    const QString request = DBRequest::SelectAllWhere(T_ACHIEVEMENT, str_id);
    achieves = dbtask->SelectAchievements(request);
    return achieves;
}

QList<dbItem> DBGame::GetItem(int game_id) //получить предметы
{
    QList<dbItem> items = QList<dbItem>();
    const QString str_id = "g_id=" + QString::number(game_id);
    const QString request = DBRequest::SelectAllWhere(T_INVENTORY, str_id);
    items = dbtask->SelectItems(request);
    return items;
}

QString DBGame::Create(dbGame *game) //создать игру
{
    QString error_text = QString();

    error_text = this->CheckID(T_GAME, SEQ_GAME, game->id);

    if (error_text.isEmpty())
    {
        const QString str_id(QString::number(game->id));
        const QString str_price(QString::number(game->price, 'f', 2));
        const QStringList values{str_id, "'" + game->name + "'",
                                 str_price, "'" + game->sdescript + "'",
                                 "'" + game->descript + "'"};
        const QStringList columns{"id", "g_name","g_price","g_sdescription","g_description"};
        const QString request = DBRequest::Insert(T_GAME, values);
        if (dbtask->BeginTransaction())
        {
            error_text = dbtask->Insert(request);
            if (error_text.isEmpty())
            {
                if (game->dev_collection != nullptr)
                {
                    game->dev_collection->g_id = game->id;
                    error_text = CreateGameDeveloper(game->dev_collection);
                }
                else
                    error_text = "DBGAME: DEVELOPER_CREATE_NULLPOINTER_ERROR";
                if (!error_text.isEmpty())
                {
                    dbtask->RollbackTransaction();
                    return error_text;
                }
                if (game->publish_collection != nullptr)
                {
                    game->publish_collection->g_id = game->id;
                    error_text = CreateGamePublisher(game->publish_collection);
                }
                else
                    error_text = "DBGAME: DEVELOPER_CREATE_NULLPOINTER_ERROR";
                if (!error_text.isEmpty())
                {
                    dbtask->RollbackTransaction();
                    return error_text;
                }
                if (game->achieve_collection != nullptr)
                {
                    game->achieve_collection->g_id = game->id;
                    error_text = CreateAchievements(game->achieve_collection);
                }
                if (!error_text.isEmpty())
                {
                    dbtask->RollbackTransaction();
                    return error_text;
                }
                if (game->item_collection != nullptr)
                {
                    game->item_collection->id = game->id;
                    error_text = CreateInventory(game->item_collection);
                }
                if (!error_text.isEmpty())
                {
                    dbtask->RollbackTransaction();
                    return error_text;
                }
                if (error_text.isEmpty())
                {
                    dbtask->CommitTransaction();
                }
                else
                {
                    dbtask->RollbackTransaction();
                }
            }
            else
            {
                dbtask->RollbackTransaction();
            }
        }
        else
        {
            error_text = "ERROR: CANT_BEGIN_TRANSACTION";
        }
    }
    return error_text;
}

QString DBGame::DeleteConnect(int id1, int id2, dbTableType type) //удалить связь
{
    QString error_text = QString();
    QString table = QString();
    QString statement1 = QString();
    QString statement2 = QString();
    switch (type)
    {
    case dbTableType::GAME_DEVELOPER_: //игра-разработчик
    {
        table = T_GAME_DEVELOPER;
        statement1 = "g_id=" + QString::number(id1);
        statement2 = "d_id=" + QString::number(id2);
        break;
    }
    case dbTableType::GAME_PUBLISHER_: //игра-издатель
    {
        table = T_GAME_PUBLISHER;
        statement1 = "g_id=" + QString::number(id1);
        statement2 = "p_id=" + QString::number(id2);
        break;
    }
    }
    int id = GetID(statement1 + " AND " + statement2, table);
    error_text = Delete(table, id);

    return error_text;
}

QString DBGame::CreateGameDeveloper(dbDevCollection *collection) //создать связь игра-разработчик
{
    QString error_text = QString();

    QStringList strdev_id = QStringList();
    for (int i = 0; i < collection->devs_amount; i++)
        strdev_id.append(QString::number(collection->developers[i].id));

    for (int i = 0; i < strdev_id.count(); i++)
    {
        int id = -1;
        error_text = this->CheckID(T_GAME_DEVELOPER, SEQ_GAME_DEVELOPER, id);
        if (!error_text.isEmpty())
        {
            error_text.append("Что-то не так с id у разработчика (DBGAME)");
            break;
        }
        else
        {
            const QString str_id(QString::number(id));
            const QString strgame_id(QString::number(collection->g_id));
            const QString request = DBRequest::Insert(T_GAME_DEVELOPER, {str_id, strgame_id, strdev_id[i]});

            error_text = dbtask->Insert(request);
            if (!error_text.isEmpty())
                break;
        }
    }

    return error_text;
}

QString DBGame::CreateGameDeveloper(dbDev *dev, int game_id) //создать связь игра-разработчик
{
    QString error_text = QString();

    QString request(DBRequest::SelectWhere(T_DEVELOPER, "id", "d_name='" + dev->name + "'"));

    const QString strdev_id(QString::number(dbtask->Select(request)));

    int id = -1;
    error_text = this->CheckID(T_GAME_DEVELOPER, SEQ_GAME_DEVELOPER, id);
    if (!error_text.isEmpty())
    {
        error_text.append("Что-то не так с id у разработчика (DBGAME)");
    }
    else
    {
        const QString str_id(QString::number(id));
        const QString strgame_id(QString::number(game_id));

        request = DBRequest::Insert(T_GAME_DEVELOPER, {str_id, strgame_id, strdev_id});

        error_text = dbtask->Insert(request);
    }

    return error_text;
}

QString DBGame::CreateGamePublisher(dbPublishCollection *collection) //создать связь игра-издатель
{
    QString error_text = QString();

    QStringList strpub_id = QStringList();
    for (int i = 0; i < collection->publishers_amount; i++)
        strpub_id.append(QString::number(collection->publishers[i].id));

    for (int i = 0; i < strpub_id.count(); i++)
    {
        int id = -1;
        error_text = this->CheckID(T_GAME_PUBLISHER, SEQ_GAME_PUBLISHER, id);
        if (!error_text.isEmpty())
        {
            error_text.append("Что-то не так с id у издателя (DBGAME)");
            break;
        }
        else
        {
            const QString str_id(QString::number(id));
            const QString strgame_id(QString::number(collection->g_id));
            const QString request = DBRequest::Insert(T_GAME_PUBLISHER, {str_id, strgame_id, strpub_id[i]});
            error_text = dbtask->Insert(request);
            if (!error_text.isEmpty())
                break;
        }
    }

    return error_text;
}

QString DBGame::CreateGamePublisher(dbPublish *pub, int game_id) //создать связь игра-издатель
{
    QString error_text = QString();

    QString request(DBRequest::SelectWhere(T_PUBLISHER, "id", "p_name='" + pub->name + "'"));

    const QString strpub_id(QString::number(dbtask->Select(request)));

    int id = -1;
    error_text = this->CheckID(T_GAME_PUBLISHER, SEQ_GAME_PUBLISHER, id);
    if (!error_text.isEmpty())
    {
        error_text.append("Что-то не так с id у издателя (DBGAME)");
    }
    else
    {
        const QString str_id(QString::number(id));
        const QString strgame_id(QString::number(game_id));

        request = DBRequest::Insert(T_GAME_PUBLISHER, {str_id, strgame_id, strpub_id});

        error_text = dbtask->Insert(request);
    }

    return error_text;
}

QString DBGame::CreateAchievements(dbAchieveCollection *collection) //создать достижения
{
    QString error_text = QString();
    if (collection->achievements != nullptr)
    {
        DBAchievement *achievement = new DBAchievement(dbtask, this);
        for (int i = 0; i < collection->achieves_amount; i++)
        {
            error_text = achievement->Create(collection->achievements[i], collection->g_id);
            if (!error_text.isEmpty())
            {
                break;
            }
        }
        achievement->deleteLater();
    }
    else
    {
        error_text = "DBGAME: ACHIEVE_NULLPOINTER_ERROR";
    }
    return error_text;
}

QString DBGame::CreateAchievement(dbAchieve *achieve, int game_id) //создать достижения
{
    QString error_text = QString();

    int id = -1;
    error_text = this->CheckID(T_ACHIEVEMENT, SEQ_ACHIEVEMENT, id);
    if (!error_text.isEmpty())
    {
        error_text.append("Что-то не так с id у достижений (DBGAME)");
    }
    else
    {
        const QString str_id(QString::number(id));
        const QString strgame_id(QString::number(game_id));

        const QString request = DBRequest::Insert(T_ACHIEVEMENT,
                                                  {str_id,
                                                   strgame_id,
                                                   "'" + achieve->name.replace("'","''") + "'",
                                                   "'" + achieve->description.replace("'","''") + "'"});

        error_text = dbtask->Insert(request);
    }

    return error_text;
}

QString DBGame::CreateInventory(dbItemCollection *collection) //создать инвентарь
{
    QString error_text = QString();
    if (collection->items != nullptr)
    {
        DBItem *item = new DBItem(dbtask, this);
        for (int i = 0; i < collection->item_amount; i++)
        {
            error_text = item->Create(collection->items[i], collection->id);
            if (!error_text.isEmpty())
            {
                break;
            }
        }
        item->deleteLater();
    }
    else
    {
        error_text = "DBGAME: ACHIEVE_NULLPOINTER_ERROR";
    }
    return error_text;
}

QString DBGame::CreateItem(dbItem *item, int game_id) //создать предмет
{
    QString error_text = QString();

    int id = -1;
    error_text = this->CheckID(T_INVENTORY, SEQ_INVENTORY, id);
    if (!error_text.isEmpty())
    {
        error_text.append("Что-то не так с id у предметов (DBGAME)");
    }
    else
    {
        const QString str_id(QString::number(id));
        const QString strgame_id(QString::number(game_id));

        const QString request = DBRequest::Insert(T_INVENTORY,
                                                  {str_id,
                                                   strgame_id,
                                                   "'" + item->name.replace("'","''") + "'",
                                                   QString::number(item->price),
                                                   "'" + item->description.replace("'","''") + "'"});

        error_text = dbtask->Insert(request);
    }

    return error_text;
}

QCompleter *DBGame::GetCompleter() //получить комплектовщик
{
    QStringList list_name = QStringList();

    const QString column("g_name");
    const QString request(DBRequest::Select(T_GAME, column));

    const QList<dbGame> game = dbtask->SelectGames(request, {column});

    for (int i = 0; i < game.count(); i++)
        list_name.append(game[i].name);

    if (completer != nullptr)
        delete completer;

    completer = new QCompleter(list_name, this);

    return completer;
}

QString DBGame::Update(const QString &statement, int id, const QString table) //переопределение обновления
{
    return DBWork::Update(statement, id, table);
}

QString DBGame::Update(const QString &statement, const QString table, const QString condition) //переопределение обновления
{
    return DBWork::Update(statement, table, condition);
}

int DBGame::GetID(const QString &condition, const QString table) //переопределение получения индекса
{
    return DBWork::GetID(condition, table);
}

dbGame *DBGame::GetGame(int id, QString &error_text) //получить игру
{
    dbGame game;

    const QString column("id");
    const QString condition(DBRequest::Statement(column, id));

    QString request(DBRequest::SelectAllWhere(T_GAME, condition));

    game = dbtask->SelectGame(request);

    if (game.name.isEmpty())
        error_text = "Такой игры не существует!";
    else
    {
        BuildGame(game);
    }

    return new dbGame(game);
}

void DBGame::BuildGame(dbGame &game) //собрать игру
{
    QList<dbDev> devs = GetDeveloper(game.id);
    if (!devs.isEmpty())
    {
        dbDev *developers = nullptr;
        developers = new dbDev[devs.count()]();
        for (int i = 0; i < devs.count(); i++)
        {
            developers[i].id = devs[i].id;
            developers[i].name = devs[i].name;
        }
        game.dev_collection = new dbDevCollection(devs.count(), developers);
        game.dev_collection->g_id = game.id;
    }
    QList<dbPublish> pubs = GetPublisher(game.id);
    if (!pubs.isEmpty())
    {
        dbPublish *publishers = nullptr;
        publishers = new dbPublish[pubs.count()]();
        for (int i = 0; i < pubs.count(); i++)
        {
            publishers[i].id = pubs[i].id;
            publishers[i].name = pubs[i].name;
        }
        game.publish_collection = new dbPublishCollection(pubs.count(), publishers);
        game.publish_collection->g_id = game.id;
    }
    QList<dbAchieve> achieves = GetAchieve(game.id);
    if (!achieves.isEmpty())
    {
        dbAchieve *achievements = nullptr;
        achievements = new dbAchieve[achieves.count()]();
        for (int i = 0; i < achieves.count(); i++)
        {
            achievements[i].id = achieves[i].id;
            achievements[i].name = achieves[i].name;
            achievements[i].description = achieves[i].description;
        }
        game.achieve_collection = new dbAchieveCollection(achieves.count(), achievements);
        game.achieve_collection->g_id = game.id;
    }
    QList<dbItem> items = GetItem(game.id);
    if (!items.isEmpty())
    {
        dbItem *items_ = nullptr;
        items_ = new dbItem[items.count()]();
        for (int i = 0; i < items.count(); i++)
        {
            items_[i].id = items[i].id;
            items_[i].name = items[i].name;
            items_[i].price = items[i].price;
            items_[i].description = items[i].description;
        }
        game.item_collection = new dbItemCollection(items.count(), items_);
        game.item_collection->id = game.id;
    }
}

dbLibrary DBGame::GetGames(int user_id) //получить библиотеку игр
{
    dbLibrary library;

    QString struser_id(QString::number(user_id));
    QString request(DBRequest::SelectAllWhere(T_LIBRARY, "u_id=" + struser_id));

    QList<dbLibGame> games = dbtask->SelectGamesForLibrary(request);

    library.game_amount = games.count();

    if (library.game_amount > 0)
    {
        library.games = new dbLibGame[library.game_amount]();

        for (int i = 0; i < library.game_amount; i++)
        {
            library.games[i] = games[i];
        }
    }

    return library;
}

dbCart DBGame::GetCartGames(int user_id) //получить корзину игр
{
    dbCart cart;

    QString struser_id(QString::number(user_id));
    QString request(DBRequest::SelectWhere(T_CART, "g_id", "u_id=" + struser_id));

    QList<int> games_id = dbtask->SelectID(request);
    cart.game_amount = games_id.count();

    if (cart.game_amount > 0)
    {
        cart.games = new dbGame[cart.game_amount]();

        for (int i = 0; i < cart.game_amount; i++)
        {
            const QString strgame_id(QString::number(games_id[i]));
            request = DBRequest::SelectWhere(T_GAME, {"g_name", "g_price"}, "id=" + strgame_id);
            dbGame game = dbtask->SelectGame(request, {"g_name", "g_price"});
            cart.games[i] = game;
            cart.games[i].id = games_id[i];
        }
    }

    request = DBRequest::SelectWhere(T_CART_INVENTORY, "i_id", "u_id=" + struser_id);
    QList<int> items_id = dbtask->SelectID(request);
    if (items_id.count() > 0)
    {
        cart.items_amount = items_id.count();
        cart.items = new dbItem[cart.items_amount]();
        cart.item_amount = new int[cart.items_amount]();

        for (int i = 0; i < cart.items_amount; i++)
        {
            QString str_id(QString::number(items_id[i]));
            request = DBRequest::SelectWhere(T_CART_INVENTORY, "c_i_amount", "i_id=" + str_id + " AND u_id=" + struser_id);
            cart.item_amount[i] = dbtask->Select(request);
            request = DBRequest::SelectAllWhere(T_INVENTORY, "id=" + str_id);
            QList<dbItem> tlist = dbtask->SelectItems(request);
            cart.items[i] = tlist.first();
        }
    }

    return cart;
}

dbStore DBGame::GetRandomGames(int amount) //получить несколько случайных игр для магазина
{
    dbStore store;

    QString request(DBRequest::SelectByRandomLimit(T_GAME, {"id","g_name"}, amount));

    QList<dbGame> games = dbtask->SelectGames(request, {"id","g_name"});

    store.game_amount = games.count();

    if (store.game_amount > 0)
    {
        store.games = new dbStoreGame[store.game_amount]();

        for (int i = 0; i < store.game_amount; i++)
        {
            store.games[i].g_id = games[i].id;
            store.games[i].g_name = games[i].name;
        }
    }

    return store;
}

dbStore DBGame::GetSearchGames(int amount, QString line) //получить несколько игр по поиску для магазина
{
    dbStore store;

    QString request(DBRequest::SelectWhereLikeLimit(T_GAME, {"id","g_name"}, "g_name", line, amount));

    QList<dbGame> games = dbtask->SelectGames(request, {"id","g_name"});

    store.game_amount = games.count();

    if (store.game_amount > 0)
    {
        store.games = new dbStoreGame[store.game_amount]();

        for (int i = 0; i < store.game_amount; i++)
        {
            store.games[i].g_id = games[i].id;
            store.games[i].g_name = games[i].name;
        }
    }

    return store;
}

QList<dbUserItem> DBGame::GetItems(int game_id, int user_id) //получить достижения пользователя для конкретной игры
{
    QList<dbUserItem> items = QList<dbUserItem>();
    QString struser_id(QString::number(user_id));
    QString strgame_id(QString::number(game_id));

    QString request(DBRequest::SelectWhere(T_USER_INVENTORY, "i_id", "u_id=" + struser_id));
    QList<int> item_id = dbtask->SelectID(request);
    request = DBRequest::SelectWhere(T_USER_INVENTORY, "u_i_amount", "u_id=" + struser_id);
    QList<int> item_amount = dbtask->SelectID(request);
    for (int i = 0; i < item_id.count(); i++)
    {
        QString str_id(QString::number(item_id[i]));
        request = DBRequest::SelectAllWhere(T_INVENTORY, "id=" + str_id + " AND g_id=" + strgame_id);
        QList<dbItem> itms = dbtask->SelectItems(request);
        dbUserItem item;
        item.amount = item_amount[i];
        item.item = new dbItem(itms.first());
        items.append(item);
    }
    return items;
}

dbItemCollection DBGame::GetItems(QString game_name) //получить предметы игры
{
    dbItemCollection collection;

    QString request(DBRequest::SelectWhere(T_GAME, "id", "g_name='" + game_name.replace("'", "''") + "'"));
    int game_id = dbtask->Select(request);
    collection.id = game_id;
    QString strgame_id(QString::number(game_id));
    request = DBRequest::SelectAllWhere(T_INVENTORY, "g_id=" + strgame_id);
    QList<dbItem> items = dbtask->SelectItems(request);

    collection.items = new dbItem[items.count()]();
    collection.item_amount = items.count();
    for (int i = 0; i < items.count(); i++)
    {
        collection.items[i].id = items[i].id;
        collection.items[i].name = items[i].name;
        collection.items[i].price = items[i].price;
        collection.items[i].description = items[i].description;
    }
    return collection;
}

QList<dbItem> DBGame::GetItems(QString game_name, QString item_name) //получить предметы по поиску
{
    bool noGame = game_name.isEmpty();
    bool noItem = item_name.isEmpty();

    QList<dbItem> items = QList<dbItem>();
    if (!noGame || !noItem)
    {
        QList<int> game_id = QList<int>();
        if (!noGame)
        {
            QString request(DBRequest::SelectWhereLike(T_GAME, "id", "g_name", game_name));
            game_id = dbtask->SelectID(request);
            foreach (int id, game_id)
            {
                QString str_id(QString::number(id));
                request = DBRequest::SelectWhere(T_INVENTORY, "*", "g_id=" + str_id + (noItem ? "" : " AND i_name LIKE '%" + item_name + "%'"));
                items.append(dbtask->SelectItems(request));
            }
        }
        else
        {
            QString request(DBRequest::SelectWhereLike(T_INVENTORY, "*", "i_name", item_name));
            items.append(dbtask->SelectItems(request));
        }
    }

    return items;
}

dbReview DBGame::GetReview(int game_id, int user_id) //получить отзыв
{
    QString struser_id(QString::number(user_id));
    QString strgame_id(QString::number(game_id));

    QString request(DBRequest::SelectAllWhere(T_REVIEW, "g_id=" + strgame_id + " AND u_id=" + struser_id));
    dbReview review = dbtask->SelectReview(request);
    return review;
}

dbCurrentAchievements DBGame::GetUserAchievements(int game_id, int user_id) //получить пользовательские достижения
{
    QString struser_id(QString::number(user_id));
    QString strgame_id(QString::number(game_id));

    QString request(DBRequest::SelectAllWhere(T_ACHIEVEMENT, "g_id=" + strgame_id));
    QList<dbAchieve> achievements = dbtask->SelectAchievements(request);

    request = DBRequest::SelectWhere(T_USER_ACHIEVEMENT, "a_id", "u_id=" + struser_id);
    QList<int> earned = dbtask->SelectID(request);

    dbCurrentAchievements pack;
    pack.achievements = new dbUserAchieve[achievements.count()]();
    pack.achievements_amount = achievements.count();
    for (int i = 0; i < achievements.count(); i++)
    {
        pack.achievements[i].achievement = new dbAchieve(achievements[i]);
        if (earned.contains(achievements[i].id))
        {
            pack.achievements[i].isEarned = !pack.achievements[i].isEarned;
        }
    }

    return pack;
}

QList<dbReview> DBGame::GetReviews(int game_id, int rating) //получить отзывы по рейтингу
{
    QString strgame_id(QString::number(game_id));
    QString condition("g_id=" + strgame_id + ( rating+1 ? " AND r_rating=" + QString::number(rating) : ""));
    QString request(DBRequest::SelectAllWhere(T_REVIEW, condition));

    return dbtask->SelectReviews(request);
}

QString DBGame::GetUserName(int user_id) //получить имя пользователя
{
    QString struser_id(QString::number(user_id));
    QString request(DBRequest::SelectWhere(T_USER, "u_username", "id=" + struser_id));

    return dbtask->SelectUser(request, {"u_username"}).name;
}

bool DBGame::HasAchievements(int game_id) //проверка игры на достижения
{
    QString strgame_id(QString::number(game_id));
    int amount = this->GetIDAmount(T_ACHIEVEMENT, "g_id=" + strgame_id);
    return (amount + 1);
}

bool DBGame::HasInventory(int game_id) //проверка игры на предметы
{
    QString strgame_id(QString::number(game_id));
    int amount = this->GetIDAmount(T_INVENTORY, "g_id=" + strgame_id);
    return (amount + 1);
}

bool DBGame::HasInventory(int game_id, int user_id) //проверка пользователя на инвентарь конкретной игры
{
    QString struser_id(QString::number(user_id));
    QString request(DBRequest::SelectWhere(T_USER_INVENTORY, "i_id", "u_id=" + struser_id));
    QList<int> item_id = dbtask->SelectID(request);
    request = DBRequest::SelectWhere(T_USER_INVENTORY, "u_i_amount", "u_id=" + struser_id);
    int amount = 0;
    for (int i = 0; i < item_id.count(); i++)
    {
        QString str_id(QString::number(item_id[i]));
        request = DBRequest::SelectWhere(T_INVENTORY, "g_id", "id=" + str_id);
        if (dbtask->Select(request) == game_id)
        {
            amount++;
        }
    }
    return amount;
}

void DBGame::WriteReview(dbReview review) //написать отзыв
{
    if (review.id == -1)
    {
        //INSERT
        int id;
        this->CheckID(T_REVIEW, SEQ_REVIEW, id);
        QString str_id(QString::number(id));
        QString struser_id(QString::number(review.u_id));
        QString strgame_id(QString::number(review.g_id));
        QString strrating(QString::number(review.rating));
        QString request(DBRequest::Insert(T_REVIEW, {str_id, strgame_id, struser_id, "'" + review.text + "'", strrating}));
        dbtask->Insert(request);
    }
    else
    {
        //UPDATE
        QString str_id(QString::number(review.id));
        QString statement_text(DBRequest::Statement("r_text", review.text));
        QString statement_rating(DBRequest::Statement("r_rating",review.rating));
        QString statement(statement_text + "," + statement_rating);
        QString request(DBRequest::Update(T_REVIEW, statement, "id=" + str_id));
        dbtask->Update(request);
    }
}

void DBGame::AddGameToCart(int game_id, int user_id) //добавить игру в корзину конкретного пользователя
{
    int id;
    this->CheckID(T_CART, SEQ_CART, id);
    QString str_id(QString::number(id));
    QString struser_id(QString::number(user_id));
    QString strgame_id(QString::number(game_id));
    QString request(DBRequest::Insert(T_CART, {str_id, strgame_id, struser_id}));

    dbtask->Insert(request);
}

void DBGame::DeleteCartGame(int game_id, int user_id) //удалить игру из корзины
{
    QString struser_id(QString::number(user_id));
    QString strgame_id(QString::number(game_id));
    QString condition("g_id=%1 AND u_id=%2");
    QString request(DBRequest::DeleteAll(T_CART, condition.arg(strgame_id, struser_id)));
    dbtask->Insert(request);
}

void DBGame::DeleteCartGames(int user_id) //купить все игры в корзине
{
    QString struser_id(QString::number(user_id));
    QString condition("u_id=%1");
    QString request(DBRequest::SelectWhere(T_CART, "g_id", condition.arg(struser_id)));
    QList<int> game_id = dbtask->SelectID(request);

    for (int i = 0; i < game_id.count(); i++)
    {
        int id;
        this->CheckID(T_LIBRARY, SEQ_LIBRARY, id);
        QString str_id(QString::number(id));
        QString strgame_id(QString::number(game_id[i]));
        request = DBRequest::Insert(T_LIBRARY, {str_id, strgame_id, struser_id, "0", "false"});
        dbtask->Insert(request);
    }
    request = DBRequest::DeleteAll(T_CART, condition.arg(struser_id));
    dbtask->Insert(request);
}

void DBGame::DeleteCartItem(int item_id, int user_id) //удалить предмет из корзины
{
    QString struser_id(QString::number(user_id));
    QString stritem_id(QString::number(item_id));
    QString condition("i_id=%1 AND u_id=%2");
    QString request(DBRequest::DeleteAll(T_CART_INVENTORY, condition.arg(stritem_id, struser_id)));
    dbtask->Insert(request);
}

void DBGame::DeleteCartItems(int user_id) //купить все предметы из корзины
{
    QString struser_id(QString::number(user_id));
    QString condition(QString("u_id=%1").arg(struser_id));
    QString request(DBRequest::SelectWhere(T_CART_INVENTORY, "i_id", condition));
    QList<int> item_id = dbtask->SelectID(request);
    request = DBRequest::SelectWhere(T_CART_INVENTORY, "c_i_amount", condition);
    QList<int> amounts = dbtask->SelectID(request);

    for (int i = 0; i < item_id.count(); i++)
    {
        QString stritem_id(QString::number(item_id[i]));
        request = DBRequest::SelectWhere(T_USER_INVENTORY, "u_i_amount", condition +" AND i_id=" + stritem_id);
        int amount = dbtask->Select(request);
        if (amount + 1)
        {
            QString stra(QString::number(amount + amounts[i]));
            request = DBRequest::Update(T_USER_INVENTORY, "u_i_amount=" + stra, condition +" AND i_id=" + stritem_id);
        }
        else
        {
            int id;
            this->CheckID(T_USER_INVENTORY, SEQ_USER_INVENTORY, id);
            QString str_id(QString::number(id));
            QString stritem_id(QString::number(item_id[i]));
            QString stra(QString::number(amounts[i]));
            request = DBRequest::Insert(T_USER_INVENTORY, {str_id, struser_id, stritem_id, stra});
        }
        dbtask->Insert(request);
    }
    request = DBRequest::DeleteAll(T_CART_INVENTORY, condition);
    dbtask->Insert(request);
}
