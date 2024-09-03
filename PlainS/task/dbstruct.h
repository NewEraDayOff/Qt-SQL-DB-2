#ifndef DBSTRUCT_H
#define DBSTRUCT_H

#include <QObject>
#include <QDebug>

#define MIN_GAME_NAME 4
#define MAX_GAME_NAME 128
#define MAX_GAME_SDESCRIPTION 1024
#define MAX_GAME_DESCRIPTION 8192

#define MIN_DEV_NAME 4
#define MAX_DEV_NAME 128

#define MIN_PUB_NAME 4
#define MAX_PUB_NAME 128

#define MAX_ACHIEVE_NAME 64
#define MAX_ACHIEVE_DESCRIPTION 256

#define MAX_ITEM_NAME 64
#define MAX_ITEM_DESCRIPTION 256

#define MAX_REVIEW_LENGTH 1024

#define MIN_USER_NAME 8
#define MIN_USER_PASSWORD 8
#define MIN_USER_USERNAME 8
#define MIN_USER_ADMIN 8
#define MAX_USER_NAME 16
#define MAX_USER_PASSWORD 16
#define MAX_USER_USERNAME 16
#define MAX_USER_ADMIN 16

#define T_GAME_DEVELOPER "public.game_developer"
#define T_GAME_PUBLISHER "public.game_publisher"
#define T_USER_INVENTORY "public.user_inventory"
#define T_CART_INVENTORY "public.cart_inventory"
#define T_USER_ACHIEVEMENT "public.user_achievement"

#define T_CART "public.cart"
#define T_REVIEW "public.review"
#define T_LIBRARY "public.library"
#define T_INVENTORY "public.inventory"
#define T_ACHIEVEMENT "public.achievement"

#define T_GAME "public.game"
#define T_USER "public.user"
#define T_DEVELOPER "public.developer"
#define T_PUBLISHER "public.publisher"

#define SEQ_CART "c_id_seq"
#define SEQ_GAME "g_id_seq"
#define SEQ_USER "u_id_seq"
#define SEQ_REVIEW "r_id_seq"
#define SEQ_LIBRARY "l_id_seq"
#define SEQ_DEVELOPER "d_id_seq"
#define SEQ_INVENTORY "i_id_seq"
#define SEQ_PUBLISHER "p_id_seq"
#define SEQ_ACHIEVEMENT "a_id_seq"

#define SEQ_CART_INVENTORY "c_i_id_seq"
#define SEQ_GAME_DEVELOPER "g_d_id_seq"
#define SEQ_GAME_PUBLISHER "g_p_id_seq"
#define SEQ_USER_INVENTORY "u_i_id_seq"
#define SEQ_USER_ACHIEVEMENT "u_a_id_seq"

#define GREEN_BUTTON "QPushButton \
                      { \
                          background-color:green; \
                          color:rgba(255,255,255,0.7); \
                          text-align: center; \
                          padding: 3px; \
                      } \
                      QPushButton:hover \
                      { \
                          color:rgba(255,255,255,0.9); \
                          background-color:rgba(0,150,0,1); \
                      }"

struct dbUser
{
    dbUser(){}
    dbUser(const dbUser &user)
    {
        id = user.id;
        login = user.login;
        password = user.password;
        name = user.name;
        admin = user.admin;
    }

    int id = -1;
    QString login = QString();
    QString password = QString();
    QString name = QString();
    QString admin = QString();
};

struct dbAchieve
{
    dbAchieve(){}
    dbAchieve(const dbAchieve &achieve)
    {
        id = achieve.id;
        name = achieve.name;
        description = achieve.description;
    }

    int id = -1;
    QString name = QString();
    QString description = QString();
};

struct dbAchieveCollection
{
    dbAchieveCollection(){}
    dbAchieveCollection(const dbAchieveCollection &collection)
    {
        g_id = collection.g_id;
        achieves_amount = collection.achieves_amount;
        if (achievements != nullptr) delete [] achievements;
        achievements = collection.achievements;
    }
    dbAchieveCollection(int amount, dbAchieve *achieves)
    {
        achieves_amount = amount;
        if (achievements != nullptr) delete [] achievements;
        achievements = achieves;
    }
    void Delete()
    {
        if (achievements != nullptr) delete [] achievements;
        delete this;
    }
    int g_id = -1;
    int achieves_amount = -1;
    dbAchieve *achievements = nullptr;
};

struct dbItem
{
    dbItem(){}
    dbItem(const dbItem &item)
    {
        id = item.id;
        g_id = item.g_id;
        name = item.name;
        price = item.price;
        description = item.description;
    }
    int id = -1;
    int g_id = -1;
    QString name = QString();
    double price = -1.0;
    QString description = QString();
};

struct dbItemCollection
{
    dbItemCollection(){}
    dbItemCollection(const dbItemCollection &collection)
    {
        id = collection.id;
        item_amount = collection.item_amount;
        if (items != nullptr) delete items;
        items = collection.items;
    }
    dbItemCollection(int amount, dbItem *i)
    {
        item_amount = amount;
        if (items != nullptr) delete [] items;
        items = i;
    }
    void Delete()
    {
        if (items != nullptr) delete [] items;
        delete this;
    }
    int id = -1;
    int item_amount = -1;
    dbItem *items = nullptr;
};

struct dbDev
{
    dbDev(){}
    dbDev(const dbDev &dev)
    {
        id = dev.id;
        name = dev.name;
    }
    int id = -1;
    QString name = QString();
};

struct dbDevCollection
{
    dbDevCollection(){}
    dbDevCollection(const dbDevCollection &collection)
    {
        g_id = collection.g_id;
        devs_amount = collection.devs_amount;
        if (developers != nullptr)
        {
            delete [] developers;
        }
        developers = collection.developers;
    }
    dbDevCollection(int amount, dbDev *devs)
    {
        devs_amount = amount;
        if (developers != nullptr)
        {
            delete [] developers;
        }
        developers = devs;
    }
    void Delete()
    {
        if (developers != nullptr)
        {
            delete [] developers;
        }
        delete this;
    }
    int g_id = -1;
    int devs_amount = -1;
    dbDev *developers = nullptr;
};

struct dbPublish
{
    dbPublish(){}
    dbPublish(const dbPublish &publisher)
    {
        id = publisher.id;
        name = publisher.name;
    }
    int id = -1;
    QString name = QString();
};

struct dbPublishCollection
{
    dbPublishCollection(){}
    dbPublishCollection(const dbPublishCollection &collection)
    {
        g_id = collection.g_id;
        publishers_amount = collection.publishers_amount;
        if (publishers != nullptr) delete [] publishers;
        publishers = collection.publishers;
    }
    dbPublishCollection(int amount, dbPublish *pubs)
    {
        publishers_amount = amount;
        if (publishers != nullptr) delete [] publishers;
        publishers = pubs;
    }
    void Delete()
    {
        if (publishers != nullptr) delete [] publishers;
        delete this;
    }
    int g_id = -1;
    int publishers_amount = -1;
    dbPublish *publishers = nullptr;
};

struct dbGame
{
    dbGame(){}
    dbGame(const dbGame &game)
    {
        id = game.id;
        name = game.name;
        price = game.price;
        sdescript = game.sdescript;
        descript = game.descript;

        if (dev_collection != nullptr)      delete dev_collection;
        if (publish_collection != nullptr)  delete publish_collection;
        if (achieve_collection != nullptr)  delete achieve_collection;
        if (item_collection != nullptr)     delete item_collection;

        dev_collection = game.dev_collection;
        publish_collection = game.publish_collection;
        achieve_collection = game.achieve_collection;
        item_collection = game.item_collection;
    }
    void Set(int amount, dbDev *devs)
    {
        if (dev_collection != nullptr) delete dev_collection;
        dev_collection = new dbDevCollection(amount, devs);
    }
    void Set(int amount, dbPublish *publishers)
    {
        if (publish_collection != nullptr) delete publish_collection;
        publish_collection = new dbPublishCollection(amount, publishers);
    }
    void Set(int amount, dbAchieve *achieves)
    {
        if (achieve_collection != nullptr) delete achieve_collection;
        achieve_collection = new dbAchieveCollection(amount, achieves);
    }
    void Set(int amount, dbItem *items)
    {
        if (item_collection != nullptr) delete item_collection;
        item_collection = new dbItemCollection(amount, items);
    }
    void Delete()
    {
        if (this->dev_collection != nullptr)
            this->dev_collection->Delete();
        if (this->publish_collection != nullptr)
            this->publish_collection->Delete();
        if (this->achieve_collection != nullptr)
            this->achieve_collection->Delete();
        if (this->item_collection != nullptr)
            this->item_collection->Delete();

        delete this;
    }

    int id = -1;
    QString name = QString();
    double price = -1.0;
    QString sdescript = QString();
    QString descript = QString();

    dbDevCollection *dev_collection = nullptr;
    dbPublishCollection *publish_collection = nullptr;
    dbAchieveCollection *achieve_collection = nullptr;
    dbItemCollection *item_collection = nullptr;
};

struct dbUserAchieve
{
    dbUserAchieve(){}
    dbUserAchieve(const dbUserAchieve &achieve)
    {
        if (achievement != nullptr)
        {
            delete achievement;
            achievement = nullptr;
        }
        achievement = achieve.achievement;
        isEarned = achieve.isEarned;
    }
    dbAchieve *achievement = nullptr;
    bool isEarned = false;
};

struct dbUserItem
{
    dbUserItem(){}
    dbUserItem(const dbUserItem &it)
    {
        if (item != nullptr)
        {
            delete item;
            item = nullptr;
        }
        item = it.item;
        amount = it.amount;
    }
    dbItem *item = nullptr;
    int amount = -1;
};

struct dbReview
{
    dbReview(){};
    dbReview(const dbReview &review)
    {
        id = review.id;
        g_id = review.g_id;
        u_id = review.u_id;
        rating = review.rating;
        text = review.text;
    }
    int id = -1;
    int g_id = -1;
    int u_id = -1;
    int rating = -1;
    QString text = QString();
};

struct dbLibGame
{
    dbLibGame(){}
    dbLibGame(const dbLibGame &libgame)
    {
        g_id = libgame.g_id;
        g_name = libgame.g_name;
        isInstalled = libgame.isInstalled;
        timeplayed = libgame.timeplayed;
    }

    int g_id = -1;
    QString g_name = QString();
    bool isInstalled = false;
    int timeplayed = 0;
};

struct dbStoreGame
{
    dbStoreGame(){}
    dbStoreGame(const dbStoreGame &storegame)
    {
        g_id = storegame.g_id;
        g_name = storegame.g_name;
    }

    int g_id = -1;
    QString g_name = QString();
};

struct dbStore
{
    dbStore(){}
    dbStore(const dbStore &store)
    {
        if (games != nullptr)
            delete [] games;

        game_amount = store.game_amount;
        games = store.games;
    }

    int game_amount = -1;
    dbStoreGame *games = nullptr;
};

struct dbLibrary
{
    dbLibrary(){}
    dbLibrary(const dbLibrary &lib)
    {
        if (games != nullptr)
        {
            delete [] games;
            games = nullptr;
        }

        games = lib.games;
        game_amount = lib.game_amount;
    }

    dbLibGame *games = nullptr;
    int game_amount = -1; 
};

struct dbCurrentAchievements
{
    dbCurrentAchievements(){}
    dbCurrentAchievements(const dbCurrentAchievements &pack)
    {
        if (achievements != nullptr)
        {
            for (int i = 0; i < achievements_amount; i++)
            {
                if (achievements[i].achievement != nullptr)
                {
                    delete achievements[i].achievement;
                    achievements[i].achievement = nullptr;
                }
            }
            delete [] achievements;
            achievements = nullptr;
        }

        achievements = pack.achievements;
        achievements_amount = pack.achievements_amount;
    }
    dbUserAchieve *achievements = nullptr;
    int achievements_amount = -1;
};

struct dbCart
{
    dbCart(){}
    dbCart(const dbCart &cart)
    {
        if (games != nullptr)
        {
            delete [] games;
            games = nullptr;
        }

        if (items != nullptr)
        {
            delete [] items;
            items = nullptr;
            delete [] item_amount;
            item_amount = nullptr;
        }

        games = cart.games;
        game_amount = cart.game_amount;
        items = cart.items;
        item_amount = cart.item_amount;
        items_amount = cart.items_amount;
        sum = cart.sum;
    }

    dbGame *games = nullptr;
    int game_amount = -1;

    dbItem *items = nullptr;
    int *item_amount = nullptr;
    int items_amount = -1;

    double sum = 0.0;
};

struct dbClient
{
    dbClient(){}
    dbClient(const dbClient &client)
    {
        if (library != nullptr)
        {
            if (library->games != nullptr)
            {
                delete [] library->games;
                library->games = nullptr;
            }
            delete library;
            library = nullptr;
        }
        if (cart != nullptr)
        {
            if (cart->games != nullptr)
            {
                for (int i = 0; i < cart->game_amount; i++)
                {
                    cart->games[i].Delete();
                }
                cart->games = nullptr;
            }
            delete cart;
            cart = nullptr;
        }
        if (user != nullptr)
        {
            delete user;
            user = nullptr;
        }
        if (reviews != nullptr)
        {
            delete reviews;
            reviews = nullptr;
        }

        user = client.user;
        library = client.library;
        cart = client.cart;
        reviews = client.reviews;
    }
    dbUser *user = nullptr;
    dbLibrary *library = nullptr;
    dbCart *cart = nullptr;
    dbReview *reviews = nullptr;
};

enum dbType
{
    USER_,
    GAME_,
    DEVELOPER_NEW_,
    PUBLISHER_NEW_,
    DEVELOPER_,
    PUBLISHER_
};

enum dbPage
{
    CREATE_GAME_,
    UPDATE_GAME_,
    DELETE_GAME_,
    LIBRARY_PAGE_,
    FEATURED_PAGE_,
    STORE_GAME_PAGE_,
    MARKET_PAGE_,
    CART_PAGE_,
    ACHIEVEMENTS_PAGE_,
    USER_ACHIEVEMENTS_PAGE_,
    INVENTORY_PAGE_,
    PROFILE_PAGE_
};

enum dbTableType
{
    GAME_DEVELOPER_,
    GAME_PUBLISHER_
};

#endif // DBSTRUCT_H
