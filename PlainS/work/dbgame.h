#ifndef DBGAME_H
#define DBGAME_H

#include <dbwork.h>

#include "work/dbachievement.h"
#include "work/dbitem.h"

class DBGame : public DBWork
{
    Q_OBJECT
public:
    explicit DBGame(DBTask *dbt, QObject *parent = nullptr);
    dbGame *GetGame(int id, QString &error_text);
    dbGame *GetGame(const QString name, QString &error_text);
    dbGame GetGame(int game_id);
    dbLibGame GetGame(int game_id, int user_id);
    dbGame Check(const QString text, QString &error_text);
    QString Check(const QString text);

    void AddGameToCart(int game_id, int user_id);
    void DeleteCartGame(int game_id, int user_id);
    void DeleteCartGames(int user_id);
    void DeleteCartItem(int item_id, int user_id);
    void DeleteCartItems(int user_id);

    bool HasAchievements(int game_id);
    bool HasInventory(int game_id);
    bool HasInventory(int game_id, int user_id);

    QString Create(dbGame *game);
    QString CreateAchievement(dbAchieve *achieve, int game_id);
    QString CreateItem(dbItem *item, int game_id);
    QString CreateGameDeveloper(dbDev *dev, int game_id);
    QString CreateGamePublisher(dbPublish *pub, int game_id);
    QString DeleteConnect(int id1, int id2, dbTableType type);
    QString Update(const QString &statement, int id, const QString table = "public.game") override;
    QString Update(const QString &statement, const QString table = "public.game", const QString condition = nullptr) override;

    int GetID(const QString &condition, const QString table = "public.game") override;

    dbCurrentAchievements GetUserAchievements(int game_id, int user_id);

    QList<dbUserItem> GetItems(int game_id, int user_id);
    dbItemCollection GetItems(QString game_name);
    QList<dbItem> GetItems(QString game_name, QString item_name);

    dbReview GetReview(int game_id, int user_id);
    dbLibrary GetGames(int user_id);
    dbCart GetCartGames(int user_id);
    dbStore GetRandomGames(int amount);
    dbStore GetSearchGames(int amount, QString line);

    QList<dbReview> GetReviews(int game_id, int rating = -1);

    QString GetUserName(int user_id);

    void WriteReview(dbReview review);

    QCompleter *GetCompleter();
private:
    QString CreateGameDeveloper(dbDevCollection *collection);
    QString CreateGamePublisher(dbPublishCollection *collection);
    QString CreateAchievements(dbAchieveCollection *collection);
    QString CreateInventory(dbItemCollection *collection);

    void BuildGame(dbGame &game);

    QList<dbDev> GetDeveloper(int game_id);
    QList<dbPublish> GetPublisher(int game_id);
    QList<dbAchieve> GetAchieve(int game_id);
    QList<dbItem> GetItem(int game_id);

    QCompleter *completer = nullptr;
};

#endif // DBGAME_H
