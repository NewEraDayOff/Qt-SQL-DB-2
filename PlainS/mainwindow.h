#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>

#include <QMessageBox>
#include <QFile>
#include <QToolButton>
#include <QListWidgetItem>

#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>

#include "login_ui/dblogin.h"
#include "login_ui/dbadmin.h"
#include "task/dbtask.h"
#include "work/dbgame.h"
#include "work/dbdeveloper.h"
#include "work/dbpublisher.h"
#include "work/dbuser.h"
#include "dbitembuying.h"
#include "dbplay.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

Q_DECLARE_METATYPE(dbType)
Q_DECLARE_METATYPE(dbPage)
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void loginSlot(dbUser currentUser);

    void on_action_logOut_triggered();

    void change_page();

    void on_action_admin_triggered();

    void on_action_cart_triggered();

    void on_action_profile_triggered();

    void adminSlot(bool fl);

    void create_pushButton_clicked();

    void on_pushButton_deleteGame_open_clicked();

    void on_pushButton_deleteGame_clicked();

    void on_pushButton_deleteDev_open_clicked();

    void on_pushButton_deleteDev_clicked();

    void on_pushButton_deletePublish_open_clicked();

    void on_pushButton_deletePublish_clicked();

    void on_pushButton_deleteUser_open_clicked();

    void on_pushButton_deleteUser_clicked();

    void on_pushButton_u_gameDevAdd_clicked();

    void on_pushButton_u_gamePubAdd_clicked();

    void on_pushButton_u_gameNameSave_clicked();

    void on_pushButton_u_gamePriceSave_clicked();

    void on_pushButton_u_gameSdescriptSave_clicked();

    void on_pushButton_u_gameDescriptSave_clicked();

    void on_pushButton_u_gameDevSave_clicked();

    void on_pushButton_u_gamePubSave_clicked();

    void on_pushButton_u_gameAchieveSave_clicked();

    void on_pushButton_u_gameInventorySave_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_pushButton_f_search_clicked();

    void on_listWidget_f_games_itemDoubleClicked(QListWidgetItem *item);

    void on_lineEdit_l_searchLine_textEdited(const QString &arg1);

    void on_listWidget_l_games_itemClicked(QListWidgetItem *item);

    void on_pushButton_l_storePage_clicked();

    void on_pushButton_f_gameAddToCart_clicked();

    void go_to_store();

    void on_pushButton_cart_buy_clicked();

    void go_to_write_review_page();

    void on_pushButton_wr_review_clicked();

    void on_textEdit_wr_review_textChanged();

    void on_pushButton_l_gamePlay_clicked();

    void on_pushButton_l_delete_clicked();

    void on_pushButton_f_gameAchievements_clicked();

    void on_pushButton_l_achievements_clicked();

    void create_achieve_delete();

    void create_item_delete();

    void update_achieve_delete();

    void update_item_delete();

    void on_comboBox_f_gameRating_currentIndexChanged(int index);

    void on_pushButton_m_search_clicked();

    void buy_dialog_window();

    void close_item_dialog();

    void on_pushButton_l_inventory_clicked();

    void on_comboBox_profile_game_currentIndexChanged(const QString &arg1);

    void on_pushButton_profile_achievements_clicked();

    void on_pushButton_profile_inventory_clicked();

    void on_pushButton_profile_username_clicked();

private:
    Ui::MainWindow *ui;

    bool isAdmin;
    bool devIsPub;

    int libgame_id = -1;
    int cur_lib_row = -1;
    int profile_user_id = -1;

    QString gameName = QString();

    QMap<QString,QAction*> a_toolbar;
    QMap<QString,QToolButton*> toolbar;
    QMap<QString,QAction*> page;

    QSqlDatabase *database;
    DBAdmin *adminDialog;
    DBLogin *loginDialog;
    DBItemBuying *itemDialog = nullptr;
    DBPlay *playDialog = nullptr;
    DBTask *dbtask;
    DBGame *dbgame = nullptr;
    DBDeveloper *dbdeveloper = nullptr;
    DBPublisher *dbpublisher = nullptr;
    DBUser *dbuser = nullptr;

    dbClient *client = nullptr;
    dbStore *store = nullptr;
    dbItemCollection *market = nullptr;
    dbGame *featuredGame = nullptr;
    dbGame *gameForUpdate = nullptr;
    dbReview *writeReview = nullptr;

    void SetToolbar(const QStringList buttons, QWidget *parent = nullptr);
    void SetToolbutton(const QStringList actions, QWidget *parent = nullptr);
    void SetCreateGamePage();
    void SetDeletePage();
    void SetUpdateGamePage();
    void CallCreateGamePage();
    void CallUpdateGamePage();

    void LoadLibraryGames();
    void LoadFeaturedGames();
    void LoadCartGames();
    void SearchFeaturedGames();

    void LoadFeaturedGame(int id);

    void FillDevList(dbDev *devs, int amount);
    void FillPubList(dbPublish *pubs, int amount);
    void FillAchieveTable(dbAchieve *achieves, int amount);
    void FillInventoryTable(dbItem *items, int amount);

    void ResizeReviewsTable();

    QString CheckDevelopers(dbDev *&developers);
    QString CheckPublishers(dbPublish *&publishers);
    QString CheckAchievements(dbAchieve *&achievements);
    QString CheckItems(dbItem *&items);

    QString UpdateGame(int id);

};

#endif // MAINWINDOW_H
