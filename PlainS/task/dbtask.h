#ifndef DBTASK_H
#define DBTASK_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>

#include "task/dbstruct.h"

class DBTask
{
public:
    DBTask(QSqlDatabase *db);

    bool BeginTransaction();
    bool CommitTransaction();
    bool RollbackTransaction();

    QString Insert(const QString &request);
    QString Update(const QString &request);

    dbUser SelectUser(const QString &request, QStringList columns = QStringList());
    dbGame SelectGame(const QString &request, QStringList columns = QStringList());
    dbDev SelectDeveloper(const QString &request, QStringList columns = QStringList());
    dbPublish SelectPublisher(const QString &request, QStringList columns = QStringList());
    dbReview SelectReview(const QString &request, QStringList columns = QStringList());
    QList<dbUser> SelectUsers(const QString &request, QStringList columns = QStringList());
    QList<dbGame> SelectGames(const QString &request, QStringList columns = QStringList());
    QList<dbDev> SelectDevelopers(const QString &request, QStringList columns = QStringList());
    QList<dbPublish> SelectPublishers(const QString &request, QStringList columns = QStringList());
    QList<dbAchieve> SelectAchievements(const QString &request, QStringList columns = QStringList());
    QList<dbItem> SelectItems(const QString &request, QStringList columns = QStringList());
    QList<dbReview> SelectReviews(const QString &request, QStringList columns = QStringList());

    int Select(const QString &query);
    QList<int> SelectID(const QString &query);

    dbLibGame SelectGameForLibrary(const QString &query);
    QList<dbLibGame> SelectGamesForLibrary(const QString &query);
//signals:
//    void ErrorAppears(QString error_text);

private:
    const QStringList list_user{"id","u_login","u_password","u_username","u_admin"};
    const QStringList list_game{"id","g_name","g_price","g_sdescription","g_description"};
    const QStringList list_dev{"id","d_name"};
    const QStringList list_publisher{"id","p_name"};
    const QStringList list_achieve{"id","a_name","a_description"};
    const QStringList list_item{"id","g_id","i_name","i_price","i_description"};
    const QStringList list_review{"id","g_id","u_id","r_text","r_rating"};
    const QStringList list_id{"id","g_id","d_id","a_id","u_id"};

    QSqlDatabase *database = nullptr;

    dbUser SUser(QSqlQuery &query, QStringList columns = QStringList());
    dbGame SGame(QSqlQuery &query, QStringList columns = QStringList());
    dbItem SItem(QSqlQuery &query, QStringList columns = QStringList());
    dbDev SDeveloper(QSqlQuery &query, QStringList columns = QStringList());
    dbPublish SPublisher(QSqlQuery &query, QStringList columns = QStringList());
    dbAchieve SAchievement(QSqlQuery &query, QStringList columns = QStringList());
    dbReview SReview(QSqlQuery &query, QStringList columns = QStringList());
};

#endif // DBTASK_H
