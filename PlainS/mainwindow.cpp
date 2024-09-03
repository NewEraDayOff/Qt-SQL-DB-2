#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) //конструктор
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(QPixmap::fromImage(QImage(":/Res/logo.png"))));
    this->setWindowTitle("PLAIN");

    database = nullptr;
    database = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));

    QList<QHostAddress> list = QHostInfo::fromName(QHostInfo::localHostName()).addresses();
    for (int var = 0; var < list.size(); ++var)
    {
        qDebug() << list[var];
    }

    const QString db_name("Plain");
    const QString host("127.0.0.1");
    const QString db_user("postgres");
    const QString db_password("1975");

    database->setDatabaseName(db_name);
    database->setHostName(host);
    database->setPort(5432);

    database->open(db_user, db_password);

    dbtask = nullptr;
    dbtask = new DBTask(database);

    dbgame = new DBGame(dbtask, this);
    dbdeveloper = new DBDeveloper(dbtask, this);
    dbpublisher = new DBPublisher(dbtask, this);

    loginDialog = nullptr;
    loginDialog = new DBLogin(dbtask);
    connect(loginDialog, &DBLogin::loginSignal, this, &MainWindow::loginSlot);
    loginDialog->show();

    ui->action_admin->setVisible(false);
    ui->stackedWidget->setCurrentWidget(ui->page_profile);

    ui->pushButton_addGame->setProperty("page", CREATE_GAME_);
    connect(ui->pushButton_addGame, &QPushButton::clicked, this, &MainWindow::change_page);
    ui->pushButton_updateGame->setProperty("page", UPDATE_GAME_);
    connect(ui->pushButton_updateGame, &QPushButton::clicked, this, &MainWindow::change_page);


    SetToolbar({"STORE", "LIBRARY", "PROFILE", "ADMIN"}, ui->toolBar);

    isAdmin = NULL;
    devIsPub = NULL;

    adminDialog = nullptr;

    ui->checkBox_developerIsPublisher->hide();

    SetCreateGamePage();
    SetDeletePage();
    SetUpdateGamePage();

    ui->tableWidget_a_description->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_ua_description->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_f_gameReviews->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_cart_games->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_cart_items->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_m_items->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_inventory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->lineEdit_f_searchLine, &QLineEdit::returnPressed, ui->pushButton_f_search, &QPushButton::click);

    ui->pushButton_f_game_writeReview->setProperty("from", STORE_GAME_PAGE_);
    ui->pushButton_l_review->setProperty("from", LIBRARY_PAGE_);
    connect(ui->pushButton_f_game_writeReview, &QPushButton::clicked, this, &MainWindow::go_to_write_review_page);
    connect(ui->pushButton_l_review, &QPushButton::clicked, this, &MainWindow::go_to_write_review_page);
    connect(ui->pushButton_f_gameItems, &QPushButton::clicked, this, [&]()
    {
        gameName = featuredGame->name;
        ui->stackedWidget->setCurrentWidget(ui->page_market);
    });
    connect(ui->lineEdit_profile_username, &QLineEdit::textEdited, this, [&](const QString &text)
    {
        QString error_text(DBCheck::Check(text, 8));
        if (error_text.isEmpty())
        {
            ui->pushButton_profile_username->setEnabled(true);
        }
        else
        {
            ui->label_profile_error->setText(error_text);
        }
        ui->label_profile_error->setVisible(!error_text.isEmpty());
    });

    QSizePolicy policy(ui->widget_l_gameInfo->sizePolicy());
    policy.setRetainSizeWhenHidden(true);
    ui->widget_l_gameInfo->setSizePolicy(policy);

    ui->page_library->setProperty("page", LIBRARY_PAGE_);
    ui->page_featured->setProperty("page", FEATURED_PAGE_);
    ui->page_cart->setProperty("page", CART_PAGE_);
    ui->page_achievements->setProperty("page", ACHIEVEMENTS_PAGE_);
    ui->page_user_achievements->setProperty("page", USER_ACHIEVEMENTS_PAGE_);
    ui->page_market->setProperty("page", MARKET_PAGE_);
    ui->page_inventory->setProperty("page", INVENTORY_PAGE_);
    ui->page_profile->setProperty("page", PROFILE_PAGE_);
}

MainWindow::~MainWindow() //деструктор
{
    if (client != nullptr)
        delete client;
    if (dbtask != nullptr)
        delete dbtask;
    if (database != nullptr)
    {
        if (database->isOpen())
            database->close();
        delete database;
    }
    if (loginDialog != nullptr)
        delete loginDialog;
    if (adminDialog != nullptr)
        delete adminDialog;
    if (dbgame != nullptr)
        delete dbgame;
    if (dbuser != nullptr)
        delete dbuser;
    if (dbdeveloper != nullptr)
        delete dbdeveloper;
    if (dbpublisher != nullptr)
        delete dbpublisher;

    delete ui;
}

void MainWindow::SetCreateGamePage() //сборка страницы создания игры
{
    ui->label_c_gameLogo->setPixmap(
                QPixmap::fromImage(
                    QImage(":/Res/plain_wide.png").scaled(ui->label_c_gameLogo->size(),
                                                          Qt::KeepAspectRatioByExpanding,
                                                          Qt::SmoothTransformation)));

    ui->label_c_gameName_error->setVisible(false);
    ui->label_c_gameDeveloper_error->setVisible(false);
    ui->label_c_gamePrice_error->setVisible(false);
    ui->label_c_gamePublisher_error->setVisible(false);
    ui->label_c_gameNewDev_error->setVisible(false);
    ui->label_c_gameNewPublish_error->setVisible(false);   
    ui->label_c_gameAchievements_error->setVisible(false);
    ui->label_c_gameInventory_error->setVisible(false);

    ui->scrollArea_c_gameNewDev->setVisible(false);
    ui->scrollArea_c_gameNewPublish->setVisible(false);

    ui->pushButton_c_gameAchievementAdd->setVisible(false);
//    ui->pushButton_c_gameAchievementDelete->setVisible(false);
    ui->pushButton_c_gameItemAdd->setVisible(false);
//    ui->pushButton_c_gameItemDelete->setVisible(false);

    ui->tableWidget_c_gameAchievements->setVisible(false);
    ui->tableWidget_c_gameInventory->setVisible(false);

    ui->pushButton_c_gameDeveloperAdd->setProperty("type", DEVELOPER_);
    ui->pushButton_c_gameNewDev->setProperty("type", DEVELOPER_NEW_);
    ui->pushButton_c_gamePublisherAdd->setProperty("type", PUBLISHER_);
    ui->pushButton_c_gameNewPublish->setProperty("type", PUBLISHER_NEW_);
    ui->pushButton_c_gameContinue->setProperty("type", GAME_);

    connect(ui->pushButton_c_gameNewDev, &QPushButton::clicked, this, &MainWindow::create_pushButton_clicked);
    connect(ui->pushButton_c_gameNewPublish, &QPushButton::clicked, this, &MainWindow::create_pushButton_clicked);
    connect(ui->pushButton_c_gameContinue, &QPushButton::clicked, this, &MainWindow::create_pushButton_clicked);
    connect(ui->pushButton_c_gameDeveloperAdd, &QPushButton::clicked, this, &MainWindow::create_pushButton_clicked);
    connect(ui->pushButton_c_gamePublisherAdd, &QPushButton::clicked, this, &MainWindow::create_pushButton_clicked);
    connect(ui->pushButton_c_gameDeveloperDelete, &QPushButton::clicked, this, [&]()
    {
        if (ui->listWidget_c_gameDevelopers->count() > 0)
        {
            int row = ui->listWidget_c_gameDevelopers->currentRow();

            delete ui->listWidget_c_gameDevelopers->takeItem(row);
        }
    });
    connect(ui->pushButton_c_gamePublisherDelete, &QPushButton::clicked, this, [&]()
    {
        if (ui->listWidget_c_gamePublishers->count() > 0)
        {
            int row = ui->listWidget_c_gamePublishers->currentRow();

            delete ui->listWidget_c_gamePublishers->takeItem(row);
        }
    });

    connect(ui->lineEdit_c_gameDeveloper, &QLineEdit::textChanged, this, [&](const QString &str)
    {
       if (devIsPub)
           ui->lineEdit_c_gamePublisher->setText(str);
    });
    connect(ui->checkBox_developerIsPublisher, &QCheckBox::stateChanged, this, [&](int state)
    {
        devIsPub = state;
        ui->lineEdit_c_gamePublisher->setEnabled(!devIsPub);
        if (devIsPub)
            ui->lineEdit_c_gamePublisher->setText(ui->lineEdit_c_gameDeveloper->text());
    });
    connect(ui->pushButton_c_gameDeveloper, &QPushButton::clicked, this, [&]()
    {
        const QString developer(ui->lineEdit_c_gameDeveloper->text());
        if (!developer.trimmed().isEmpty() && !ui->scrollArea_c_gameNewDev->isVisible())
            ui->lineEdit_c_gameNewDevName->setText(developer);
        ui->scrollArea_c_gameNewDev->show();
    });
    connect(ui->pushButton_c_gamePublisher, &QPushButton::clicked, this, [&]()
    {
        const QString publisher(ui->lineEdit_c_gamePublisher->text());
        if (!publisher.trimmed().isEmpty() && !ui->scrollArea_c_gameNewPublish->isVisible())
            ui->lineEdit_c_gameNewPublishName->setText(publisher);
        ui->scrollArea_c_gameNewPublish->show();
    });
    connect(ui->pushButton_c_gameNewDevCancel, &QPushButton::clicked, ui->scrollArea_c_gameNewDev, &QWidget::hide);
    connect(ui->pushButton_c_gameNewPublishCancel, &QPushButton::clicked, ui->scrollArea_c_gameNewPublish, &QWidget::hide);

    connect(ui->pushButton_c_gameAchievementAdd, &QPushButton::clicked, this, [&]() //Add row
    {
        ui->tableWidget_c_gameAchievements->setRowCount(ui->tableWidget_c_gameAchievements->rowCount()+1);
        int row = ui->tableWidget_c_gameAchievements->rowCount();
        for (int i = 0; i < 3; i++)
        {
            if (ui->tableWidget_c_gameAchievements->item(row-1, i) == nullptr)
            {
                QTableWidgetItem *item = new QTableWidgetItem;
                ui->tableWidget_c_gameAchievements->setItem(row-1, i, item);
            }
        }
        QPushButton *pb = new QPushButton(this);
        pb->setText("Delete");
        connect(pb, &QPushButton::clicked, this, &MainWindow::create_achieve_delete);
        ui->tableWidget_c_gameAchievements->setCellWidget(row-1, 2, pb);
    });
    connect(ui->pushButton_c_gameItemAdd, &QPushButton::clicked, this, [&]() //Add row
    {
        ui->tableWidget_c_gameInventory->setRowCount(ui->tableWidget_c_gameInventory->rowCount()+1);
        int row = ui->tableWidget_c_gameInventory->rowCount();
        for (int i = 0; i < 4; i++)
        {
            if (ui->tableWidget_c_gameInventory->item(row-1, i) == nullptr)
            {
                QTableWidgetItem *item = new QTableWidgetItem;
                ui->tableWidget_c_gameInventory->setItem(row-1, i, item);
            }
        }
        QPushButton *pb = new QPushButton(this);
        pb->setText("Delete");
        connect(pb, &QPushButton::clicked, this, &MainWindow::create_item_delete);
        ui->tableWidget_c_gameInventory->setCellWidget(row-1, 3, pb);
    });
    connect(ui->checkBox_c_gameAchievements, &QCheckBox::stateChanged, this, [&](int state)
    {
        ui->pushButton_c_gameAchievementAdd->setVisible(state);
        ui->tableWidget_c_gameAchievements->setVisible(state);
    });
    connect(ui->checkBox_c_gameInventory, &QCheckBox::stateChanged, this, [&](int state)
    {
        ui->pushButton_c_gameItemAdd->setVisible(state);
        ui->tableWidget_c_gameInventory->setVisible(state);
    });
}

void MainWindow::SetDeletePage() //сборка страницы удаления
{
    ui->label_delete_logo->setPixmap(
                QPixmap::fromImage(
                    QImage(":/Res/plain_wide.png").scaled(ui->label_delete_logo->size(),
                                                          Qt::KeepAspectRatioByExpanding,
                                                          Qt::SmoothTransformation)));

    ui->label_deleteDev_error->setVisible(false);
    ui->label_deleteGame_error->setVisible(false);
    ui->label_deletePublish_error->setVisible(false);
    ui->label_deleteUser_error->setVisible(false);

    ui->label_deleteDev->setVisible(false);
    ui->label_deleteGame->setVisible(false);
    ui->label_deletePublish->setVisible(false);
    ui->label_deleteUser->setVisible(false);

    ui->lineEdit_deleteDev->setVisible(false);
    ui->lineEdit_deleteGame->setVisible(false);
    ui->lineEdit_deletePublish->setVisible(false);
    ui->lineEdit_deleteUser->setVisible(false);

    ui->pushButton_deleteDev->setVisible(false);
    ui->pushButton_deleteGame->setVisible(false);
    ui->pushButton_deletePublish->setVisible(false);
    ui->pushButton_deleteUser->setVisible(false);

    QSizePolicy policy;

    policy = ui->label_deleteDev->sizePolicy();
    policy.setRetainSizeWhenHidden(true);

    ui->label_delete_logo->setSizePolicy(policy);

    ui->label_deleteDev_error->setSizePolicy(policy);
    ui->label_deleteGame_error->setSizePolicy(policy);
    ui->label_deletePublish_error->setSizePolicy(policy);
    ui->label_deleteUser_error->setSizePolicy(policy);

    ui->label_deleteDev->setSizePolicy(policy);
    ui->label_deleteGame->setSizePolicy(policy);
    ui->label_deletePublish->setSizePolicy(policy);
    ui->label_deleteUser->setSizePolicy(policy);

    policy = ui->lineEdit_deleteDev->sizePolicy();
    policy.setRetainSizeWhenHidden(true);

    ui->lineEdit_deleteDev->setSizePolicy(policy);
    ui->lineEdit_deleteGame->setSizePolicy(policy);
    ui->lineEdit_deletePublish->setSizePolicy(policy);
    ui->lineEdit_deleteUser->setSizePolicy(policy);

    policy = ui->pushButton_deleteDev->sizePolicy();
    policy.setRetainSizeWhenHidden(true);

    ui->pushButton_deleteDev->setSizePolicy(policy);
    ui->pushButton_deleteGame->setSizePolicy(policy);
    ui->pushButton_deletePublish->setSizePolicy(policy);
    ui->pushButton_deleteUser->setSizePolicy(policy);
}

void MainWindow::SetUpdateGamePage() //сборка страницы обновления игры
{
    ui->label_u_logo->setPixmap(
                QPixmap::fromImage(
                    QImage(":/Res/plain_wide.png").scaled(ui->label_u_logo->size(),
                                                          Qt::KeepAspectRatioByExpanding,
                                                          Qt::SmoothTransformation)));

    ui->label_updateGame_error->setVisible(false);

    ui->label_u_gameAchieve_error->setVisible(false);
    ui->label_u_gameDev_error->setVisible(false);
    ui->label_u_gameInventory_error->setVisible(false);
    ui->label_u_gameName_error->setVisible(false);
    ui->label_u_gamePub_error->setVisible(false);
    ui->label_u_gamePrice_error->setVisible(false);

    connect(ui->lineEdit_u_gameName, &QLineEdit::textEdited, this, [&](){ui->pushButton_u_gameNameSave->setEnabled(true);});
    connect(ui->lineEdit_u_gamePrice, &QLineEdit::textEdited, this, [&](){ui->pushButton_u_gamePriceSave->setEnabled(true);});
    connect(ui->textEdit_u_gameDescript, &QTextEdit::textChanged, this, [&](){ui->pushButton_u_gameDescriptSave->setEnabled(true);});
    connect(ui->textEdit_u_gameSdescript, &QTextEdit::textChanged, this, [&](){ui->pushButton_u_gameSdescriptSave->setEnabled(true);});

    connect(ui->pushButton_u_gameDevDelete, &QPushButton::clicked, this, [&]()
    {
        int row = ui->listWidget_u_gameDev->currentRow();
        delete ui->listWidget_u_gameDev->takeItem(row);
        if (row != -1)
        {
            ui->label_u_gameDev_error->setVisible(false);
            ui->pushButton_u_gameDevSave->setEnabled(true);
        }
    });
    connect(ui->pushButton_u_gamePubDelete, &QPushButton::clicked, this, [&]()
    {
        int row = ui->listWidget_u_gamePub->currentRow();
        delete ui->listWidget_u_gamePub->takeItem(row);
        if (row != -1)
        {
            ui->label_u_gamePub_error->setVisible(false);
            ui->pushButton_u_gamePubSave->setEnabled(true);
        }
    });
    connect(ui->pushButton_u_gameAchieveAdd, &QPushButton::clicked, this, [&]()
    {
        int row = ui->tableWidget_u_gameAchieve->rowCount();
        row++;
        ui->tableWidget_u_gameAchieve->setRowCount(row);
        for (int i = 0; i < 3; i++)
        {
            if (ui->tableWidget_u_gameAchieve->item(row-1, i) == nullptr)
            {
                QTableWidgetItem *item = new QTableWidgetItem;
                ui->tableWidget_u_gameAchieve->setItem(row-1, i, item);
            }
        }
        QPushButton *pb = new QPushButton(this);
        pb->setText("Delete");
        connect(pb, &QPushButton::clicked, this, &MainWindow::update_achieve_delete);
        ui->tableWidget_u_gameAchieve->setCellWidget(row-1, 2, pb);
        ui->pushButton_u_gameAchieveSave->setEnabled(true);
    });
    connect(ui->tableWidget_u_gameAchieve, &QTableWidget::itemChanged, this, [&]()
    {ui->pushButton_u_gameAchieveSave->setEnabled(true);});
    connect(ui->pushButton_u_gameItemAdd, &QPushButton::clicked, this, [&]() //Add row
    {
        int row = ui->tableWidget_u_gameInventory->rowCount();
        row++;
        ui->tableWidget_u_gameInventory->setRowCount(row);
        for (int i = 0; i < 4; i++)
        {
            if (ui->tableWidget_u_gameInventory->item(row-1, i) == nullptr)
            {
                QTableWidgetItem *item = new QTableWidgetItem;
                ui->tableWidget_u_gameInventory->setItem(row-1, i, item);
            }
        }
        QPushButton *pb = new QPushButton(this);
        pb->setText("Delete");
        connect(pb, &QPushButton::clicked, this, &MainWindow::update_item_delete);
        ui->tableWidget_u_gameInventory->setCellWidget(row-1, 3, pb);
        ui->pushButton_u_gameInventorySave->setEnabled(true);
    });
    connect(ui->tableWidget_u_gameInventory, &QTableWidget::itemChanged, this, [&]()
    {ui->pushButton_u_gameInventorySave->setEnabled(true);});

    ui->tableWidget_u_gameAchieve->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_u_gameInventory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::SetToolbar(const QStringList buttons, QWidget *parent)
{
    QFont font(parent->font());
    font.setFamily("Arial");
    font.setPointSize(14);

    const QList<QStringList> names{{"FEATURED","CART","MARKET"},
                                   {"LIBRARY"},
                                   {"PROFILE"},
                                   {"CREATE","UPDATE","DELETE"}};
    QToolButton *button;
    QMenu *menu;
    for (int i = 0; i < buttons.count(); i++)
    {
        button = new QToolButton(parent);
        button->setToolButtonStyle(Qt::ToolButtonTextOnly);
        button->setPopupMode(QToolButton::InstantPopup);
        button->setFocusPolicy(Qt::NoFocus);
        button->setArrowType(Qt::NoArrow);
        button->setText(buttons[i]);
        button->setFont(font);

        menu = new QMenu(button);
        menu->setFocusPolicy(Qt::NoFocus);
        button->setMenu(menu);

        SetToolbutton(names[i], menu);
        toolbar.insert(buttons[i].toLower(), button);
        a_toolbar.insert(buttons[i].toLower(), ui->toolBar->addWidget(button));
    }

    a_toolbar["admin"]->setVisible(false);

    page["featured"]->setShortcut(Qt::CTRL | Qt::Key_F);
    page["cart"]->setShortcut(Qt::CTRL | Qt::Key_C);
    page["market"]->setShortcut(Qt::CTRL | Qt::Key_M);
    page["library"]->setShortcut(Qt::CTRL | Qt::Key_L);
    page["profile"]->setShortcut(Qt::CTRL | Qt::Key_P);

    page["create"]->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_C);
    page["delete"]->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_D);
    page["update"]->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_U);

}

void MainWindow::SetToolbutton(const QStringList actions, QWidget *parent) //сборка панели меню
{
    QFont font(parent->font());
    font.setFamily("Arial");
    font.setPointSize(8);

    QAction *action;
    QMenu *menu = dynamic_cast<QMenu*>(parent);
    for (int i = 0; i < actions.count(); i++)
    {
        action = new QAction(parent);
        action->setFont(font);
        action->setText(actions[i]);
        action->setProperty("page", actions[i].toLower());
        menu->addAction(action);
        connect(action, &QAction::triggered, this, &MainWindow::change_page);
        page.insert(actions[i].toLower(), action);
    }
}

void MainWindow::loginSlot(dbUser currentUser) //вход пользователя
{
    if (currentUser.id == -1)
        QApplication::quit();
    else
    {
        if (client == nullptr)
            client = new dbClient();

        if (client->user != nullptr)
        {
            delete client->user;
            client->user = nullptr;
        }
        if (client->cart != nullptr)
        {
            if (client->cart->games != nullptr)
            {
                delete [] client->cart->games;
                client->cart->games = nullptr;
            }
            delete client->cart;
            client->cart = nullptr;
        }

        client->user = new dbUser(currentUser);
        LoadCartGames();

        cur_lib_row = -1;

        if (loginDialog != nullptr)
        {
            delete loginDialog;
            loginDialog = nullptr;
        }

        ui->menu->setTitle(client->user->name);
        toolbar["profile"]->setText(client->user->name.toUpper());
        ui->action_logOut->setText("Выйти из аккаунта: " + client->user->login);

        profile_user_id = client->user->id;
        ui->stackedWidget->setCurrentWidget(ui->page_load);
        ui->stackedWidget->setCurrentWidget(ui->page_library);

        if (!currentUser.admin.isEmpty())
            ui->action_admin->setVisible(true);

        this->show();
    }
}

void MainWindow::on_action_admin_triggered() //вход от имени администратора
{
    if (isAdmin)
    {
        isAdmin = !isAdmin;
        a_toolbar["admin"]->setVisible(isAdmin);
        ui->action_admin->setText("Войти как админинстратор");
        ui->stackedWidget->setCurrentWidget(ui->page_library);
    }
    else
    {
        this->hide();
        adminDialog = new DBAdmin(dbtask, client->user->id, this);
        connect(adminDialog, &DBAdmin::closeDialog, this, &MainWindow::adminSlot);
        adminDialog->show();
    }
}

void MainWindow::adminSlot(bool fl) //работа входа как администратора
{
    isAdmin = fl;
    if (adminDialog != nullptr)
    {
        delete adminDialog;
        adminDialog = nullptr;
    }
    ui->action_admin->setText(fl ? "Выйти из аккаунта как администратор"
                                 : "Войти как админинстратор");
    a_toolbar["admin"]->setVisible(fl);

    this->show();
}

void MainWindow::on_action_logOut_triggered() //выход из аккаунта
{
    this->hide();

    loginDialog = new DBLogin(dbtask, client->user->login);
    connect(loginDialog, &DBLogin::loginSignal, this, &MainWindow::loginSlot);

    if (client->user != nullptr)
    {
        delete client->user;
        client->user = nullptr;
    }

    isAdmin = NULL;

    a_toolbar["admin"]->setVisible(false);
    ui->action_admin->setVisible(false); 

    loginDialog->show();
}

//Смена страниц основного окна
void MainWindow::change_page()
{

    QWidget *w = nullptr;

    QAction *a = qobject_cast<QAction*>(sender());
    if (a != nullptr)
    {
        const QString action = a->property("page").toString();

        if (action == "featured")
            w = ui->page_featured;
        else if (action == "cart")
        {
            w = ui->page_cart;
        }
        else if (action == "library")
        {
            profile_user_id = client->user->id;
            w = ui->page_library;
        }
        else if (action == "market")
        {
            w = ui->page_market;
        }
        else if (action == "profile")
        {
            profile_user_id = client->user->id;
            w = ui->page_profile;
        }
        else if (action == "create")
            w = ui->page_create;
        else if (action == "delete")
            w = ui->page_delete;
        else if (action == "update")
        {
            ui->lineEdit_updateGameName->setCompleter(dbgame->GetCompleter());
            w = ui->page_update;
        }
    }
    QPushButton *pb = qobject_cast<QPushButton*>(sender());
    if (pb != nullptr)
    {
        const dbPage page = qvariant_cast<dbPage>(pb->property("page"));

        switch (page)
        {
        case CREATE_GAME_:
        {
            CallCreateGamePage();
            w = ui->page_c_game;
            break;
        }
        case UPDATE_GAME_:
        {
            const QString name(ui->lineEdit_updateGameName->text().replace("'", "''"));
            QString error_text = QString();
            if (gameForUpdate != nullptr)
                gameForUpdate->Delete();
            gameForUpdate = dbgame->GetGame(name, error_text);
            if (error_text.isEmpty())
            {
                CallUpdateGamePage();
                w = ui->page_u_game;
            }
            else
                ui->label_updateGame_error->setText(error_text);

            ui->label_updateGame_error->setVisible(!error_text.isEmpty());
            break;
        }
        case DELETE_GAME_:
        {
            if (ui->label_deleteGame->isVisible())
            {
                ui->pushButton_deleteGame_open->click();
            }
            if (ui->label_deleteDev->isVisible())
            {
                ui->pushButton_deleteDev_open->click();
            }
            if (ui->label_deletePublish->isVisible())
            {
                ui->pushButton_deletePublish_open->click();
            }
            if (ui->label_deleteUser->isVisible())
            {
                ui->pushButton_deleteUser_open->click();
            }
            break;
        }
        default:break;
        }
    }

    if (w != nullptr)
    {
        ui->stackedWidget->setCurrentWidget(ui->page_load);
        ui->stackedWidget->setCurrentWidget(w);
    }
}

void MainWindow::CallCreateGamePage() //вызов страницы создания игры
{
    ui->lineEdit_c_gameDeveloper->setCompleter(dbdeveloper->GetCompleter());
    ui->lineEdit_c_gamePublisher->setCompleter(dbpublisher->GetCompleter());
}

void MainWindow::CallUpdateGamePage() //вызов страницы обновления игры
{
    ui->lineEdit_u_gameDev->setCompleter(dbdeveloper->GetCompleter());
    ui->lineEdit_u_gamePub->setCompleter(dbpublisher->GetCompleter());

    ui->lineEdit_u_gameDev->clear();
    ui->lineEdit_u_gameName->clear();
    ui->lineEdit_u_gamePrice->clear();
    ui->lineEdit_u_gamePub->clear();

    ui->listWidget_u_gameDev->clear();
    ui->listWidget_u_gamePub->clear();

    int row;
    row = ui->tableWidget_u_gameAchieve->rowCount();
    for (int i = 0; i < row; i++)
    {
        ui->tableWidget_u_gameAchieve->removeCellWidget(row, 2);
    }
    row = ui->tableWidget_u_gameInventory->rowCount();
    for (int i = 0; i < row; i++)
    {
        ui->tableWidget_u_gameInventory->removeCellWidget(row, 3);
    }

    ui->tableWidget_u_gameAchieve->clear();
    ui->tableWidget_u_gameInventory->clear();

    ui->label_u_gameAchieve_error->setVisible(false);
    ui->label_u_gameDev_error->setVisible(false);
    ui->label_u_gameInventory_error->setVisible(false);
    ui->label_u_gameName_error->setVisible(false);
    ui->label_u_gamePrice_error->setVisible(false);
    ui->label_u_gamePub_error->setVisible(false);

    ui->lineEdit_u_gameName->setText(gameForUpdate->name);
    ui->lineEdit_u_gamePrice->setText(QString::number(gameForUpdate->price, 'f', 2));
    ui->textEdit_u_gameSdescript->setText(gameForUpdate->sdescript);
    ui->textEdit_u_gameDescript->setText(gameForUpdate->descript);

    if (gameForUpdate->dev_collection != nullptr)
        for (int i = 0; i < gameForUpdate->dev_collection->devs_amount; i++)
                ui->listWidget_u_gameDev->addItem(gameForUpdate->dev_collection->developers[i].name);

    if (gameForUpdate->publish_collection != nullptr)
        for (int i = 0; i < gameForUpdate->publish_collection->publishers_amount; i++)
                ui->listWidget_u_gamePub->addItem(gameForUpdate->publish_collection->publishers[i].name);

    if (gameForUpdate->achieve_collection != nullptr)
    {
        ui->tableWidget_u_gameAchieve->setRowCount(gameForUpdate->achieve_collection->achieves_amount);
        for (int i = 0; i < gameForUpdate->achieve_collection->achieves_amount; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (ui->tableWidget_u_gameAchieve->item(i, j) == nullptr)
                {
                    QTableWidgetItem *item = new QTableWidgetItem;
                    ui->tableWidget_u_gameAchieve->setItem(i, j, item);
                }
            }
            ui->tableWidget_u_gameAchieve->item(i, 0)->setText(gameForUpdate->achieve_collection->achievements[i].name);
            ui->tableWidget_u_gameAchieve->item(i, 1)->setText(gameForUpdate->achieve_collection->achievements[i].description);

            QPushButton *pb = new QPushButton(this);
            pb->setText("Delete");
            connect(pb, &QPushButton::clicked, this, &MainWindow::update_achieve_delete);
            ui->tableWidget_u_gameAchieve->setCellWidget(i, 2, pb);
        }
    }

    if (gameForUpdate->item_collection != nullptr)
    {
        ui->tableWidget_u_gameInventory->setRowCount(gameForUpdate->item_collection->item_amount);
        for (int i = 0; i < gameForUpdate->item_collection->item_amount; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (ui->tableWidget_u_gameInventory->item(i, j) == nullptr)
                {
                    QTableWidgetItem *item = new QTableWidgetItem;
                    ui->tableWidget_u_gameInventory->setItem(i, j, item);
                }
            }
            ui->tableWidget_u_gameInventory->item(i, 0)->setText(gameForUpdate->item_collection->items[i].name);
            ui->tableWidget_u_gameInventory->item(i, 1)->setText(QString::number(gameForUpdate->item_collection->items[i].price, 'f', 2));
            ui->tableWidget_u_gameInventory->item(i, 2)->setText(gameForUpdate->item_collection->items[i].description);

            QPushButton *pb = new QPushButton(this);
            pb->setText("Delete");
            connect(pb, &QPushButton::clicked, this, &MainWindow::update_item_delete);
            ui->tableWidget_u_gameInventory->setCellWidget(i, 3, pb);
        }
    }

    ui->tableWidget_u_gameAchieve->setHorizontalHeaderLabels({"NAME","DESCRIPTION","DELETE"});
    ui->tableWidget_u_gameInventory->setHorizontalHeaderLabels({"NAME","PRICE","DESCRIPTION","DELETE"});

    ui->pushButton_u_gameDescriptSave->setEnabled(false);
    ui->pushButton_u_gameDevSave->setEnabled(false);
    ui->pushButton_u_gameNameSave->setEnabled(false);
    ui->pushButton_u_gamePriceSave->setEnabled(false);
    ui->pushButton_u_gamePubSave->setEnabled(false);
    ui->pushButton_u_gameSdescriptSave->setEnabled(false);
    ui->pushButton_u_gameAchieveSave->setEnabled(false);
    ui->pushButton_u_gameInventorySave->setEnabled(false);
}

void MainWindow::create_pushButton_clicked() //Попытка создания игры/пользователя/издателя
{
    QPushButton *pb = qobject_cast<QPushButton*>(sender());
    const dbType t(qvariant_cast<dbType>(pb->property("type")));

    switch (t)
    {
    case DEVELOPER_NEW_:
    {
        if (dbdeveloper == nullptr)
        {
            ui->label_error->setText("ERROR_DBDEVELOPER_DOESN'T_EXIST");
            ui->stackedWidget->setCurrentWidget(ui->page_error);
        }
        else
        {

            QString error_text = dbdeveloper->Create(ui->lineEdit_c_gameNewDevName->text().replace("'", "''"));
            if (!error_text.isEmpty())
            {
                ui->label_error->setText(error_text);
                ui->stackedWidget->setCurrentWidget(ui->page_error);
            }
            else
            {
                ui->lineEdit_c_gameDeveloper->setCompleter(dbdeveloper->GetCompleter());
                ui->lineEdit_c_gameNewDevName->clear();
                ui->scrollArea_c_gameNewDev->hide();
            }
        }
        break;
    }
    case PUBLISHER_NEW_:
    {
        if (dbpublisher == nullptr)
        {
            ui->label_error->setText("ERROR_DBPUBLISHER_DOESN'T_EXIST");
            ui->stackedWidget->setCurrentWidget(ui->page_error);
        }
        else
        {
            QString error_text = dbpublisher->Create(ui->lineEdit_c_gameNewPublishName->text().replace("'", "''"));
            if (!error_text.isEmpty())
            {
                ui->label_error->setText(error_text);
                ui->stackedWidget->setCurrentWidget(ui->page_error);
            }
            else
            {
                ui->lineEdit_c_gamePublisher->setCompleter(dbpublisher->GetCompleter());
                ui->lineEdit_c_gameNewPublishName->clear();
                ui->scrollArea_c_gameNewPublish->hide();
            }
        }
        break;
    }
    case DEVELOPER_:
    {
        if (dbdeveloper != nullptr)
        {
            const QString dev_name(ui->lineEdit_c_gameDeveloper->text().replace("'", "''"));
            const QString dev_nameb(ui->lineEdit_c_gameDeveloper->text());
            QString error_text;
            dbdeveloper->Check(dev_name, error_text);
            if (error_text.isEmpty())
            {
                for (int i = 0; i < ui->listWidget_c_gameDevelopers->count(); i++)
                {
                    if (dev_name == ui->listWidget_c_gameDevelopers->item(i)->text())
                    {
                        error_text = "Такой разработчик уже добавлен в список разработчиков";
                        ui->label_c_gameDeveloper_error->setText(error_text);
                        break;
                    }
                }
                if (error_text.isEmpty())
                {
                    ui->listWidget_c_gameDevelopers->addItem(dev_nameb);
                    ui->lineEdit_c_gameDeveloper->clear();
                }
            }
            else
                ui->label_c_gameDeveloper_error->setText(error_text);

            ui->label_c_gameDeveloper_error->setVisible(!error_text.isEmpty());
        }
        break;
    }
    case PUBLISHER_:
    {
        if (dbpublisher != nullptr)
        {
            const QString pub_name(ui->lineEdit_c_gamePublisher->text().replace("'", "''"));
            const QString pub_nameb(ui->lineEdit_c_gamePublisher->text());
            QString error_text;
            dbpublisher->Check(pub_name, error_text);

            if (error_text.isEmpty())
            {
                for (int i = 0; i < ui->listWidget_c_gamePublishers->count(); i++)
                {
                    if (pub_name == ui->listWidget_c_gamePublishers->item(i)->text())
                    {
                        error_text = "Такой разработчик уже добавлен в список разработчиков";
                        ui->label_c_gamePublisher_error->setText(error_text);
                        break;
                    }
                }
                if (error_text.isEmpty())
                {
                    ui->listWidget_c_gamePublishers->addItem(pub_nameb);
                    ui->lineEdit_c_gamePublisher->clear();
                }
            }
            else
                ui->label_c_gamePublisher_error->setText(error_text);

            ui->label_c_gamePublisher_error->setVisible(!error_text.isEmpty());
        }
        break;
    }
    case GAME_:
    { 
        if (dbgame == nullptr || dbdeveloper == nullptr || dbpublisher == nullptr)
        {
            ui->label_error->setText("ERROR_DBDGAME_OR_DBDEV_OR_DBPUB_DOESN'T_EXIST");
            ui->stackedWidget->setCurrentWidget(ui->page_error);
        }
        else
        {
            const QString name(ui->lineEdit_c_gameName->text().replace("'", "''"));
            const QString price(ui->lineEdit_c_gamePrice->text());
            double double_game_price = -1;

            QString error_text(dbgame->Check(name));

            bool some_error = false;
            bool b_achievement = ui->checkBox_c_gameAchievements->isChecked();
            bool b_inventory = ui->checkBox_c_gameInventory->isChecked();

            //GAME NAME ERROR BEGIN//
            if (!error_text.isEmpty())
            {
                ui->label_c_gameName_error->setText(error_text);
                some_error = true;
            }
            ui->label_c_gameName_error->setVisible(!error_text.isEmpty());
            error_text.clear();
            //GAME NAME ERROR END//

            if (price.trimmed().isEmpty())
                error_text = "Введите цену продукта";
            else
            {
                bool fl;
                double d_price = price.toDouble(&fl);
                if (!fl || price.toDouble() < 0)
                    error_text = "Цена на продукт указана некорректно";
                else
                    double_game_price = d_price;
            }

            //PRICE ERROR BEGIN//
            if (!error_text.isEmpty())
            {
                ui->label_c_gamePrice_error->setText(error_text);
                some_error = true;
            }
            ui->label_c_gamePrice_error->setVisible(!error_text.isEmpty());
            error_text.clear();
            //PRICE ERROR END//

            dbDev *developers = nullptr;
            error_text = CheckDevelopers(developers);

            //DEVELOPERS ERROR BEGIN//
            if (!error_text.isEmpty())
            {
                ui->label_c_gameDeveloper_error->setText(error_text);
                some_error = true;
            }
            ui->label_c_gameDeveloper_error->setVisible(!error_text.isEmpty());
            error_text.clear();
            //DEVELOPER ERROR END//

            dbPublish *publishers = nullptr;
            error_text = CheckPublishers(publishers);

            //PUBLISHERS ERROR BEGIN//
            if (!error_text.isEmpty())
            {
                ui->label_c_gamePublisher_error->setText(error_text);
                some_error = true;
            }
            ui->label_c_gamePublisher_error->setVisible(!error_text.isEmpty());
            error_text.clear();
            //PUBLISHERS ERROR END//

            dbAchieve *achievements = nullptr;
            error_text = b_achievement ? CheckAchievements(achievements) : QString();

            //ACHIEVEMENTS ERROR BEGIN//
            if (!error_text.isEmpty())
            {
                ui->label_c_gameAchievements_error->setText(error_text);
                some_error = true;
            }
            ui->label_c_gameAchievements_error->setVisible(!error_text.isEmpty() && b_achievement);
            error_text.clear();
            //ACHIEVEMENTS ERROR END//

            dbItem *items = nullptr;
            error_text = b_inventory ? CheckItems(items) : QString();

            //ITEMS ERROR BEGIN//
            if (!error_text.isEmpty())
            {
                ui->label_c_gameInventory_error->setText(error_text);
                some_error = true;
            }
            ui->label_c_gameInventory_error->setVisible(!error_text.isEmpty() && b_inventory);
            error_text.clear();
            //ITEMS ERROR END//

            if (!some_error)
            {
                const int dev_amount = ui->listWidget_c_gameDevelopers->count();
                const int pub_amount = ui->listWidget_c_gamePublishers->count();
                const int achieve_amount = ui->tableWidget_c_gameAchievements->rowCount();
                const int item_amount = ui->tableWidget_c_gameInventory->rowCount();

                const QString sdescript(ui->textEdit_c_gameShortDescription->toPlainText().isEmpty()
                                        ? "Нет описания"
                                        : ui->textEdit_c_gameShortDescription->toPlainText().replace("'", "''"));
                const QString descript(ui->textEdit_c_gameDescription->toPlainText().isEmpty()
                                        ? "Нет описания"
                                        : ui->textEdit_c_gameDescription->toPlainText().replace("'", "''"));

                dbGame *temp_game = new dbGame;
                temp_game->name = name;
                temp_game->price = double_game_price;
                temp_game->sdescript = sdescript;
                temp_game->descript = descript;

                temp_game->Set(dev_amount, developers);
                temp_game->Set(pub_amount, publishers);
                if (b_achievement)
                    temp_game->Set(achieve_amount, achievements);
                if (b_inventory)
                    temp_game->Set(item_amount, items);

                error_text = dbgame->Create(temp_game);

                if (error_text.isEmpty())
                {
                    ui->checkBox_c_gameAchievements->setChecked(false);
                    ui->checkBox_c_gameInventory->setChecked(false);

                    ui->lineEdit_c_gameName->clear();
                    ui->lineEdit_c_gamePrice->clear();
                    ui->lineEdit_c_gameDeveloper->clear();
                    ui->lineEdit_c_gamePublisher->clear();
                    ui->lineEdit_c_gameNewDevName->clear();
                    ui->lineEdit_c_gameNewPublishName->clear();

                    ui->textEdit_c_gameShortDescription->clear();
                    ui->textEdit_c_gameDescription->clear();

                    ui->listWidget_c_gameDevelopers->clear();
                    ui->listWidget_c_gamePublishers->clear();

                    int row;
                    row = ui->tableWidget_c_gameAchievements->rowCount();
                    for (int i = 0; i < row; i++)
                    {
                        ui->tableWidget_c_gameAchievements->removeCellWidget(row, 2);
                    }
                    row = ui->tableWidget_c_gameInventory->rowCount();
                    for (int i = 0; i < row; i++)
                    {
                        ui->tableWidget_c_gameInventory->removeCellWidget(row, 3);
                    }

                    ui->tableWidget_c_gameAchievements->clear();
                    ui->tableWidget_c_gameInventory->clear();

                    ui->label_c_gameAchievements_error->hide();
                    ui->label_c_gameDeveloper_error->hide();
                    ui->label_c_gameInventory_error->hide();
                    ui->label_c_gameName_error->hide();
                    ui->label_c_gameNewDev_error->hide();
                    ui->label_c_gameNewPublish_error->hide();
                    ui->label_c_gamePrice_error->hide();
                    ui->label_c_gamePublisher_error->hide();

                    ui->scrollArea_c_gameNewDev->hide();
                    ui->scrollArea_c_gameNewPublish->hide();


                }
                else
                {
                    ui->label_error->setText(error_text);
                    ui->stackedWidget->setCurrentWidget(ui->page_error);
                }

                temp_game->Delete();
                temp_game = nullptr;
                developers = nullptr;
                publishers = nullptr;
                achievements = nullptr;
                items = nullptr;
            }

            if (developers != nullptr)
                delete [] developers;
            if (publishers != nullptr)
                delete [] publishers;
            if (achievements != nullptr)
                delete [] achievements;
            if (items != nullptr)
                delete [] items;
        }
        break;
    }
    default:break;
    }
}

QString MainWindow::CheckDevelopers(dbDev *&developers) //проверка разработчиков
{
    QString error_text = QString();
    const int row = ui->listWidget_c_gameDevelopers->count();
    if (row > 0)
    {
        if (developers != nullptr)
            delete [] developers;
        developers = new dbDev[row]();
        for (int i = 0; i < row; i++)
        {
            developers[i].id = dbdeveloper->Check(ui->listWidget_c_gameDevelopers->item(i)->text().replace("'", "''"), error_text).id;
            if (!error_text.isEmpty())
            {
                break;
            }
        }
    }
    else
        error_text = "Добавьте хотя бы одного разработчика";

    return error_text;
}

QString MainWindow::CheckPublishers(dbPublish *&publishers) //проверка издателей
{
    QString error_text = QString();
    int row = ui->listWidget_c_gamePublishers->count();
    if (row > 0)
    {
        if (publishers != nullptr)
            delete [] publishers;
        publishers = new dbPublish[row]();
        for (int i = 0; i < row; i++)
        {
            publishers[i].id = dbpublisher->Check(ui->listWidget_c_gamePublishers->item(i)->text().replace("'", "''"), error_text).id;
            if (!error_text.isEmpty())
            {
                break;
            }
        }
    }
    else
        error_text = "Добавьте хотя бы одного издателя";

    return error_text;
}

QString MainWindow::CheckAchievements(dbAchieve *&achievements) //проверка достижений
{
    QString error_text = QString();

    int row = ui->tableWidget_c_gameAchievements->rowCount();
    if (row > 0)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if (ui->tableWidget_c_gameAchievements->item(i, j) == nullptr)
                {
                    error_text = "Не все ячейки указаны корректно";
                    QTableWidgetItem *item = new QTableWidgetItem;
                    item->setText("");
                    ui->tableWidget_c_gameAchievements->setItem(i, j, item);
                    break;
                }
                if (ui->tableWidget_c_gameAchievements->item(i, j)->text().trimmed().isEmpty())
                {
                    error_text = "Некоторые ячейки пусты";
                    break;
                }
                if (ui->tableWidget_c_gameAchievements->item(i, j)->text().trimmed().count() >
                   (j ? MAX_ACHIEVE_DESCRIPTION : MAX_ACHIEVE_NAME))
                {
                    error_text = "Некоторые ячейки указаны некорректно";
                    break;
                }
            }
            if (!error_text.isEmpty())
                break;
        }
        if (error_text.isEmpty())
        {
            for (int i = 0; i < row; i++)
            {
                for (int j = i+1; j < row; j++)
                {
                    QString name1(ui->tableWidget_c_gameAchievements->item(i, 0)->text());
                    QString name2(ui->tableWidget_c_gameAchievements->item(j, 0)->text());
                    if (name1 == name2)
                    {
                        error_text = "Нельзя создать несколько достижений с одинаковыми именами";
                        break;
                    }
                }
                if (!error_text.isEmpty())
                    break;
            }
            if (error_text.isEmpty())
            {
                if (achievements != nullptr)
                    delete [] achievements;
                achievements = new dbAchieve[row]();

                for (int i = 0; i < row; i++)
                {
                    achievements[i].name = ui->tableWidget_c_gameAchievements->item(i, 0)->text().replace("'", "''");
                    achievements[i].description = ui->tableWidget_c_gameAchievements->item(i, 1)->text().replace("'", "''");
                }
            }
        }
    }
    else
        error_text = "Добавьте хотя бы одно достижение";
    return error_text;
}

QString MainWindow::CheckItems(dbItem *&items) //проверка предметов
{
    QString error_text = QString();

    int row = ui->tableWidget_c_gameInventory->rowCount();
    if (row > 0)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (ui->tableWidget_c_gameInventory->item(i, j) == nullptr)
                {
                    error_text = "Не все ячейки указаны корректно";
                    QTableWidgetItem *item = new QTableWidgetItem;
                    item->setText("");
                    ui->tableWidget_c_gameInventory->setItem(i, j, item);
                    break;
                }
                if (ui->tableWidget_c_gameInventory->item(i, j)->text().trimmed().isEmpty())
                {
                    error_text = "Некоторые ячейки пусты";
                    break;
                }
                if (j % 2 == 1)
                {
                    bool fl;
                    double d_price = ui->tableWidget_c_gameInventory->item(i, j)->text().toDouble(&fl);
                    if (!fl || d_price < 0)
                    {
                        error_text = "Цена на некоторые предметы указана некорректно";
                        break;
                    }
                }
                if (j % 2 != 1)
                {
                    if (ui->tableWidget_c_gameInventory->item(i, j)->text().trimmed().count()
                     > (j ? MAX_ITEM_DESCRIPTION : MAX_ITEM_NAME))
                    {
                        error_text = "Не все ячейки указаны корректно";
                        break;
                    }
                }
            }
            if (!error_text.isEmpty())
                break;
        }
        if (error_text.isEmpty())
        {
            for (int i = 0; i < row; i++)
            {
                for (int j = i+1; j < row; j++)
                {
                    QString name1(ui->tableWidget_c_gameInventory->item(i, 0)->text());
                    QString name2(ui->tableWidget_c_gameInventory->item(j, 0)->text());
                    if (name1 == name2)
                    {
                        error_text = "Нельзя создать несколько предметов с одинаковыми именами";
                        break;
                    }
                }
                if (!error_text.isEmpty())
                    break;
            }
            if (error_text.isEmpty())
            {
                if (items != nullptr)
                    delete [] items;
                items = new dbItem[row]();

                for (int i = 0; i < row; i++)
                {
                    items[i].name = ui->tableWidget_c_gameInventory->item(i, 0)->text().replace("'", "''");
                    items[i].price = ui->tableWidget_c_gameInventory->item(i, 1)->text().toDouble();
                    items[i].description = ui->tableWidget_c_gameInventory->item(i, 2)->text().replace("'", "''");
                }
            }
        }
    }
    else
        error_text = "Добавьте хотя бы один предмет";
    return error_text;
}

void MainWindow::on_pushButton_deleteGame_open_clicked() //открытие удаления игры
{
    bool fl = ui->label_deleteGame->isVisible();

    if (!fl)
    {
        ui->lineEdit_deleteGame->setCompleter(dbgame->GetCompleter());
    }

    ui->label_deleteGame->setVisible(!fl);
    ui->lineEdit_deleteGame->setVisible(!fl);
    ui->pushButton_deleteGame->setVisible(!fl);

    ui->label_deleteGame_error->setVisible(false);
}


void MainWindow::on_pushButton_deleteGame_clicked() //удаление игры
{
    QString name(ui->lineEdit_deleteGame->text().replace("'", "''"));
    QString error_text = QString();

    dbGame tgame = dbgame->Check(name, error_text);

    if (error_text.isEmpty())
    {
        error_text = dbgame->Delete(T_GAME, tgame.id);
        if (error_text.isEmpty())
        {
            ui->lineEdit_deleteGame->setCompleter(dbgame->GetCompleter());
            ui->lineEdit_deleteGame->clear();
        }
    }

    ui->label_deleteGame_error->setText(error_text);
    ui->label_deleteGame_error->setVisible(!error_text.isEmpty());
}


void MainWindow::on_pushButton_deleteDev_open_clicked() //открытие удаления разработчика
{
    if (dbdeveloper == nullptr)
        dbdeveloper = new DBDeveloper(dbtask, this);

    bool fl = ui->label_deleteDev->isVisible();

    if (!fl)
    {
        ui->lineEdit_deleteDev->setCompleter(dbdeveloper->GetCompleter());
    }

    ui->label_deleteDev->setVisible(!fl);
    ui->lineEdit_deleteDev->setVisible(!fl);
    ui->pushButton_deleteDev->setVisible(!fl);

    ui->label_deleteDev_error->setVisible(false);
}


void MainWindow::on_pushButton_deleteDev_clicked() //удаление разработчика
{
    QString name(ui->lineEdit_deleteDev->text().replace("'", "''"));
    QString error_text = QString();

    dbDev tdev = dbdeveloper->Check(name, error_text);

    if (error_text.isEmpty())
    {
        error_text = dbdeveloper->Delete(T_DEVELOPER, tdev.id);
        if (error_text.isEmpty())
        {
            ui->lineEdit_deleteDev->setCompleter(dbdeveloper->GetCompleter());
            ui->lineEdit_deleteDev->clear();
        }
    }

    ui->label_deleteDev_error->setText(error_text);
    ui->label_deleteDev_error->setVisible(!error_text.isEmpty());
}


void MainWindow::on_pushButton_deletePublish_open_clicked() //открытие удаления издателя
{
    if (dbpublisher == nullptr)
        dbpublisher = new DBPublisher(dbtask, this);

    bool fl = ui->label_deletePublish->isVisible();

    if (!fl)
    {
        ui->lineEdit_deletePublish->setCompleter(dbpublisher->GetCompleter());
    }

    ui->label_deletePublish->setVisible(!fl);
    ui->lineEdit_deletePublish->setVisible(!fl);
    ui->pushButton_deletePublish->setVisible(!fl);

    ui->label_deletePublish_error->setVisible(false);
}


void MainWindow::on_pushButton_deletePublish_clicked() //удаление издателя
{
    QString name(ui->lineEdit_deletePublish->text().replace("'", "''"));
    QString error_text = QString();

    dbPublish tpub = dbpublisher->Check(name, error_text);

    if (error_text.isEmpty())
    {
        error_text = dbdeveloper->Delete(T_PUBLISHER, tpub.id);
        if (error_text.isEmpty())
        {
            ui->lineEdit_deletePublish->setCompleter(dbpublisher->GetCompleter());
            ui->lineEdit_deletePublish->clear();
        }
    }

    ui->label_deletePublish_error->setText(error_text);
    ui->label_deletePublish_error->setVisible(!error_text.isEmpty());
}


void MainWindow::on_pushButton_deleteUser_open_clicked() //открытие удаления пользоватля
{
    if (dbuser == nullptr)
        dbuser = new DBUser(dbtask, this);

    bool fl = ui->label_deleteUser->isVisible();

    if (!fl)
    {
        ui->lineEdit_deleteUser->setCompleter(dbuser->GetCompleter());
    }

    ui->label_deleteUser->setVisible(!fl);
    ui->lineEdit_deleteUser->setVisible(!fl);
    ui->pushButton_deleteUser->setVisible(!fl);

    ui->label_deleteUser_error->setVisible(false);
}


void MainWindow::on_pushButton_deleteUser_clicked() //удаление пользователя
{
    QString name(ui->lineEdit_deleteUser->text().replace("'", "''"));
    QString error_text = QString();

    dbUser tuser = dbuser->CheckOnDelete(name, error_text, client->user->login);

    if (error_text.isEmpty())
    {
        error_text = dbuser->Delete(T_USER, tuser.id);
        if (error_text.isEmpty())
        {
            ui->lineEdit_deleteUser->setCompleter(dbuser->GetCompleter());
            ui->lineEdit_deleteUser->clear();
        }
    }

    ui->label_deleteUser_error->setText(error_text);
    ui->label_deleteUser_error->setVisible(!error_text.isEmpty());
}


void MainWindow::on_pushButton_u_gameDevAdd_clicked() //добавление разработчика при обновлении
{
    QString error_text = QString();
    QString name(ui->lineEdit_u_gameDev->text());
    dbDev dev = dbdeveloper->CheckExist(name.replace("'", "''"), error_text);

    if (error_text.isEmpty())
    {
        for (int i = 0; i < ui->listWidget_u_gameDev->count(); i++)
        {
            if (ui->listWidget_u_gameDev->item(i)->text() == dev.name)
            {
                error_text = "Такой разработчик уже добавлен";
                break;
            }
        }
        if (error_text.isEmpty())
        {
            ui->listWidget_u_gameDev->addItem(dev.name);
            ui->pushButton_u_gameDevSave->setEnabled(true);
            ui->lineEdit_u_gameDev->clear();
        }
    }
    ui->label_u_gameDev_error->setText(error_text);
    ui->label_u_gameDev_error->setVisible(!error_text.isEmpty());
}


void MainWindow::on_pushButton_u_gamePubAdd_clicked() //Добавить издателя при изменении информации об игре
{
    QString error_text = QString();
    QString name(ui->lineEdit_u_gamePub->text());
    dbPublish pub = dbpublisher->CheckExist(name.replace("'", "''"), error_text);

    if (error_text.isEmpty())
    {
        for (int i = 0; i < ui->listWidget_u_gamePub->count(); i++)
        {
            if (ui->listWidget_u_gamePub->item(i)->text() == pub.name)
            {
                error_text = "Такой издатель уже добавлен";
                break;
            }
        }
        if (error_text.isEmpty())
        {
            ui->listWidget_u_gamePub->addItem(pub.name);
            ui->pushButton_u_gamePubSave->setEnabled(true);
            ui->lineEdit_u_gamePub->clear();
        }
    }
    ui->label_u_gamePub_error->setText(error_text);
    ui->label_u_gamePub_error->setVisible(!error_text.isEmpty());
}


void MainWindow::on_pushButton_u_gameNameSave_clicked() //Сохранить название при изменении информации об игре
{
    QString name(ui->lineEdit_u_gameName->text());
    QString error_text = QString();

    error_text = dbgame->Check(QString(name).replace("'", "''"));

    if (error_text.isEmpty())
    {
        if (name.trimmed().isEmpty())
            error_text = "Строка имени пуста";
        else
        {
            if (name != gameForUpdate->name)
                error_text = dbgame->Update("g_name='" + QString(name).replace("'", "''") + "'", gameForUpdate->id);

            if (error_text.isEmpty())
            {
                ui->pushButton_u_gameNameSave->setEnabled(false);
            }
        }
    }

    ui->label_u_gameName_error->setText(error_text);
    ui->label_u_gameName_error->setVisible(!error_text.isEmpty());
}


void MainWindow::on_pushButton_u_gamePriceSave_clicked() //Сохранить цену при изменении информации об игре
{
    QString price(ui->lineEdit_u_gamePrice->text());
    QString error_text = QString();

    if (price.trimmed().isEmpty())
        error_text = "Укажите цену";
    else
    {
        bool fl;
        double dprice = price.toDouble(&fl);
        if (!fl || dprice < 0)
            error_text = "Цена указана некорректно";
        else
        {
            if (dprice != gameForUpdate->price)
                error_text = dbgame->Update("g_price=" + QString::number(dprice), gameForUpdate->id);

            if (error_text.isEmpty())
            {
                ui->pushButton_u_gamePriceSave->setEnabled(false);
            }
        }
    }

    ui->label_u_gamePrice_error->setText(error_text);
    ui->label_u_gamePrice_error->setVisible(!error_text.isEmpty());
}


void MainWindow::on_pushButton_u_gameSdescriptSave_clicked() //Сохранить короткое описание при изменении информации об игре
{
    QString sdescript(ui->textEdit_u_gameSdescript->toPlainText());
    QString error_text = QString();

    if (sdescript.isEmpty())
        sdescript = "Нет описания";

    if (sdescript != gameForUpdate->sdescript)
        error_text = dbgame->Update("g_sdescription='" + sdescript.replace("'","''") + "'", gameForUpdate->id);

    if (error_text.isEmpty())
        ui->pushButton_u_gameSdescriptSave->setEnabled(false);
    else
    {
        ui->label_error->setText(error_text);
        ui->stackedWidget->setCurrentWidget(ui->page_error);
    }
}


void MainWindow::on_pushButton_u_gameDescriptSave_clicked() //Сохранить описание при изменении информации об игре
{
    QString descript(ui->textEdit_u_gameDescript->toPlainText());
    QString error_text = QString();

    if (descript.isEmpty())
        descript = "Нет описания";

    if (descript != gameForUpdate->sdescript)
        error_text = dbgame->Update("g_description='" + descript.replace("'","''") + "'", gameForUpdate->id);

    if (error_text.isEmpty())
        ui->pushButton_u_gameDescriptSave->setEnabled(false);
    else
    {
        ui->label_error->setText(error_text);
        ui->stackedWidget->setCurrentWidget(ui->page_error);
    }
}

QString MainWindow::UpdateGame(int id) //Изменить игру
{
    QString error_text = QString();

    if (gameForUpdate != nullptr)
        delete gameForUpdate;
    gameForUpdate = dbgame->GetGame(id, error_text);

    return error_text;
}


void MainWindow::on_pushButton_u_gameDevSave_clicked() //Сохранить разработчиков при изменении информации об игре
{
    QString error_text = QString();

    if (ui->listWidget_u_gameDev->count() < 1)
        error_text = "Добавьте хотя бы одного разработчика";

    if (error_text.isEmpty())
    {
        QStringList names = QStringList();

        for (int i = 0; i < ui->listWidget_u_gameDev->count(); i++)
        {
            names.append(ui->listWidget_u_gameDev->item(i)->text());
        }
        if (gameForUpdate->dev_collection != nullptr)
        {
            QStringList namesExist = QStringList();
            for (int i = 0; i < names.count(); i++)
            {
                for (int j = 0; j < gameForUpdate->dev_collection->devs_amount; j++)
                {
                    if (names[i] == gameForUpdate->dev_collection->developers[j].name)
                    {
                        namesExist.append(names[i]);
                        names.removeAt(i);
                        i--;
                        break;
                    }
                }
            }

            for (int i = 0; i < gameForUpdate->dev_collection->devs_amount; i++) //для каждого существующего разработчика
            {
                bool isExist = false;
                foreach (QString name, namesExist) //для каждого повторяющегося имени
                {
                    if (name == gameForUpdate->dev_collection->developers[i].name) //если данный разработчик уже существует, то пропускаем его
                    {
                        isExist = !isExist;
                        break;
                    }
                }
                if (names.isEmpty() && i-1 != gameForUpdate->dev_collection->devs_amount && !isExist)
                {
                    dbgame->DeleteConnect(gameForUpdate->id, gameForUpdate->dev_collection->developers[i].id, GAME_DEVELOPER_);
                }
                else if (!isExist) //если данный разработчик был изменен, то вызываем апдейт
                {
                    dbDev dev = dbdeveloper->CheckExist(names[0], error_text);
                    if (!error_text.isEmpty())
                        break;
                    error_text = dbgame->Update("d_id=" + QString::number(dev.id), T_GAME_DEVELOPER,
                                                "g_id=" + QString::number(gameForUpdate->id) + " AND " +
                                                "d_id=" + QString::number(gameForUpdate->dev_collection->developers[i].id));
                    if (!error_text.isEmpty())
                        break;
                    names.removeFirst();

                }
            }
        }

        for (int i = 0; i < names.count(); i++)
        {
            dbDev dev = dbDev();
            dev.name = names[i];
            error_text = dbgame->CreateGameDeveloper(&dev, gameForUpdate->id);

            if (!error_text.isEmpty())
                break;
        }

        UpdateGame(gameForUpdate->id);
    }
    ui->pushButton_u_gameDevSave->setEnabled(!error_text.isEmpty());
    ui->label_u_gameDev_error->setText(error_text);
    ui->label_u_gameDev_error->setVisible(!error_text.isEmpty());
}


void MainWindow::on_pushButton_u_gamePubSave_clicked() //Сохранить издателей при изменении информации об игре
{
    QString error_text = QString();

    if (ui->listWidget_u_gamePub->count() < 1)
        error_text = "Добавьте хотя бы одного издателя";

    if (error_text.isEmpty())
    {
        QStringList names = QStringList();

        for (int i = 0; i < ui->listWidget_u_gamePub->count(); i++)
        {
            names.append(ui->listWidget_u_gamePub->item(i)->text());
        }
        if (gameForUpdate->publish_collection != nullptr)
        {
            QStringList namesExist = QStringList();
            for (int i = 0; i < names.count(); i++)
            {
                for (int j = 0; j < gameForUpdate->publish_collection->publishers_amount; j++)
                {
                    if (names[i] == gameForUpdate->publish_collection->publishers[j].name)
                    {
                        namesExist.append(names[i]);
                        names.removeAt(i);
                        i--;
                        break;
                    }
                }
            }

            for (int i = 0; i < gameForUpdate->publish_collection->publishers_amount; i++) //для каждого существующего разработчика
            {
                bool isExist = false;
                foreach (QString name, namesExist) //для каждого повторяющегося имени
                {
                    if (name == gameForUpdate->publish_collection->publishers[i].name) //если данный разработчик уже существует, то пропускаем его
                    {
                        isExist = !isExist;
                        break;
                    }
                }
                if (names.isEmpty() && i-1 != gameForUpdate->publish_collection->publishers_amount && !isExist)
                {
                    dbgame->DeleteConnect(gameForUpdate->id, gameForUpdate->publish_collection->publishers[i].id, GAME_PUBLISHER_);
                }
                else if (!isExist) //если данный разработчик был изменен, то вызываем апдейт
                {
                    dbPublish pub = dbpublisher->CheckExist(names[0], error_text);
                    if (!error_text.isEmpty())
                        break;
                    error_text = dbgame->Update("p_id=" + QString::number(pub.id), T_GAME_PUBLISHER,
                                                "g_id=" + QString::number(gameForUpdate->id) + " AND " +
                                                "p_id=" + QString::number(gameForUpdate->publish_collection->publishers[i].id));
                    if (!error_text.isEmpty())
                        break;
                    names.removeFirst();

                }
            }
        }

        for (int i = 0; i < names.count(); i++)
        {
            dbPublish pub = dbPublish();
            pub.name = names[i];
            error_text = dbgame->CreateGamePublisher(&pub, gameForUpdate->id);

            if (!error_text.isEmpty())
                break;
        }

        if (error_text.isEmpty())
            UpdateGame(gameForUpdate->id);
    }
    ui->pushButton_u_gamePubSave->setEnabled(!error_text.isEmpty());
    ui->label_u_gamePub_error->setText(error_text);
    ui->label_u_gamePub_error->setVisible(!error_text.isEmpty());
}


void MainWindow::on_pushButton_u_gameAchieveSave_clicked() //Сохранить достижения при изменении информации об игре
{
    QString error_text = QString();
    bool noA = false;
    if (ui->tableWidget_u_gameAchieve->rowCount() < 1)
        noA = !noA;
    else
    {
        for (int i = 0; i < ui->tableWidget_u_gameAchieve->rowCount(); i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if (ui->tableWidget_u_gameAchieve->item(i, j) == nullptr)
                {
                    error_text = "Не все ячейки указаны корректно";
                    QTableWidgetItem *item = new QTableWidgetItem;
                    item->setText("");
                    ui->tableWidget_u_gameAchieve->setItem(i, j, item);
                    break;
                }
                if (ui->tableWidget_u_gameAchieve->item(i, j)->text().trimmed().isEmpty()
                 || ui->tableWidget_u_gameAchieve->item(i, j)->text().trimmed().count()
                 > (j ? MAX_ACHIEVE_DESCRIPTION : MAX_ACHIEVE_NAME))
                {
                    error_text = "Не все достижения указаны корректно";
                    break;
                }
            }
            if (!error_text.isEmpty())
                break;
        }
    }

    if (error_text.isEmpty() && !noA)
    {
        for (int i = 0; i < ui->tableWidget_u_gameAchieve->rowCount(); i++)
        {
            for (int j = i+1; j < ui->tableWidget_u_gameAchieve->rowCount(); j++)
            {
                QString name1(ui->tableWidget_u_gameAchieve->item(i, 0)->text());
                QString name2(ui->tableWidget_u_gameAchieve->item(j, 0)->text());
                if (name1 == name2)
                {
                    error_text = "Название достижений не должно повторяться";
                    break;
                }
            }
            if (!error_text.isEmpty())
                break;
        }
    }

    if (error_text.isEmpty() && !noA)
    {
        QStringList names = QStringList();
        QStringList descripts = QStringList();

        for (int i = 0; i < ui->tableWidget_u_gameAchieve->rowCount(); i++)
        {
            names.append(ui->tableWidget_u_gameAchieve->item(i, 0)->text().trimmed());
            descripts.append(ui->tableWidget_u_gameAchieve->item(i, 1)->text().trimmed());
        }
        if (gameForUpdate->achieve_collection != nullptr)
        {
            QStringList namesExist = QStringList();
            QStringList descrExist = QStringList();
            for (int i = 0; i < names.count(); i++)
            {
                for (int j = 0; j < gameForUpdate->achieve_collection->achieves_amount; j++)
                {
                    if (names[i] == gameForUpdate->achieve_collection->achievements[j].name)
                    {
                        namesExist.append(names[i]);
                        descrExist.append(descripts[i]);
                        names.removeAt(i);
                        descripts.removeAt(i);
                        i--;
                        break;
                    }
                }
            }

            for (int i = 0; i < gameForUpdate->achieve_collection->achieves_amount; i++) //для каждого существующего разработчика
            {
                bool isNExist = false;
                bool isDExist = false;
                QString statement = QString();
                for (int j = 0; j < namesExist.count(); j++)
                {
                    if (namesExist[j] == gameForUpdate->achieve_collection->achievements[i].name)
                    {
                        statement = descrExist[j];
                        if (descrExist[j] == gameForUpdate->achieve_collection->achievements[i].description)
                            isDExist = !isDExist;
                        isNExist = !isNExist;
                        break;
                    }
                }
                if (names.isEmpty() && i-1 != gameForUpdate->achieve_collection->achieves_amount && !isNExist)
                {
                    dbgame->Delete(T_ACHIEVEMENT, gameForUpdate->achieve_collection->achievements[i].id);
                }
                else if (isNExist && !isDExist)
                {
                    error_text = dbgame->Update("a_description='" + statement + "'",
                                                gameForUpdate->achieve_collection->achievements[i].id, T_ACHIEVEMENT);
                    if (!error_text.isEmpty())
                        break;
                }
                else if (!isNExist) //если данный разработчик был изменен, то вызываем апдейт
                {
                    error_text = dbgame->Update("a_name='" + names[0] + "', a_description='" + descripts[0] + "'",
                            gameForUpdate->achieve_collection->achievements[i].id, T_ACHIEVEMENT);
                    if (!error_text.isEmpty())
                        break;
                    names.removeFirst();
                    descripts.removeFirst();

                }
            }
        }

        for (int i = 0; i < names.count(); i++)
        {
            dbAchieve achieve = dbAchieve();
            achieve.name = names[i];
            achieve.description = descripts[i];
            error_text = dbgame->CreateAchievement(&achieve, gameForUpdate->id);

            if (!error_text.isEmpty())
                break;
        }

        UpdateGame(gameForUpdate->id);
    }
    if (error_text.isEmpty() && noA)
    {
        error_text = dbgame->DeleteAll(T_ACHIEVEMENT, "g_id=" + QString::number(gameForUpdate->id));
        UpdateGame(gameForUpdate->id);
    }
    ui->pushButton_u_gameAchieveSave->setEnabled(!error_text.isEmpty());
    ui->label_u_gameAchieve_error->setText(error_text);
    ui->label_u_gameAchieve_error->setVisible(!error_text.isEmpty());
}


void MainWindow::on_pushButton_u_gameInventorySave_clicked() //Сохранить инвентарь при изменении информации об игре
{
    QString error_text = QString();
    bool noItem = false;

    if (ui->tableWidget_u_gameInventory->rowCount() < 1)
        noItem = !noItem;
    else
    {
        for (int i = 0; i < ui->tableWidget_u_gameInventory->rowCount(); i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (ui->tableWidget_u_gameInventory->item(i, j) == nullptr)
                {
                    error_text = "Не все ячейки указаны корректно";
                    QTableWidgetItem *item = new QTableWidgetItem;
                    item->setText("");
                    ui->tableWidget_u_gameInventory->setItem(i, j, item);
                    break;
                }
                if (ui->tableWidget_u_gameInventory->item(i, 0)->text().trimmed().isEmpty())
                {
                    error_text = "Не все предметы указаны корректно";
                    break;
                }
                if (j % 2 == 1)
                {
                    bool fl;
                    double d_price = ui->tableWidget_u_gameInventory->item(i, j)->text().toDouble(&fl);
                    if (!fl || d_price < 0)
                    {
                        error_text = "Цена на некоторые предметы указана некорректно";
                        break;
                    }
                }
                if (j % 2 != 1)
                {
                    if (ui->tableWidget_u_gameInventory->item(i, j)->text().trimmed().count()
                        > (j ? MAX_ITEM_DESCRIPTION : MAX_ITEM_NAME))
                    {
                        error_text = "Не все предметы указаны корректно";
                        break;
                    }
                }
            }
            if (!error_text.isEmpty())
                break;
        }
    }
    if (error_text.isEmpty() && !noItem)
    {
        for (int i = 0; i < ui->tableWidget_u_gameInventory->rowCount(); i++)
        {
            for (int j = i+1; j < ui->tableWidget_u_gameInventory->rowCount(); j++)
            {
                QString name1(ui->tableWidget_u_gameInventory->item(i, 0)->text());
                QString name2(ui->tableWidget_u_gameInventory->item(j, 0)->text());
                if (name1 == name2)
                {
                    error_text = "Название предметов не должно повторяться";
                    break;
                }
            }
            if (!error_text.isEmpty())
                break;
        }
    }
    if (error_text.isEmpty() && !noItem)
    {
        QStringList names = QStringList();
        QStringList descripts = QStringList();
        QList<double> prices = QList<double>();

        for (int i = 0; i < ui->tableWidget_u_gameInventory->rowCount(); i++)
        {
            names.append(ui->tableWidget_u_gameInventory->item(i, 0)->text().trimmed());
            prices.append(ui->tableWidget_u_gameInventory->item(i, 1)->text().toDouble());
            descripts.append(ui->tableWidget_u_gameInventory->item(i, 2)->text().trimmed());
        }
        if (gameForUpdate->item_collection != nullptr)
        {
            QStringList namesExist = QStringList();
            QStringList descrExist = QStringList();
            QList<double> priceExist = QList<double>();
            for (int i = 0; i < names.count(); i++)
            {
                for (int j = 0; j < gameForUpdate->item_collection->item_amount; j++)
                {
                    if (names[i] == gameForUpdate->item_collection->items[j].name)
                    {
                        namesExist.append(names[i]);
                        descrExist.append(descripts[i]);
                        priceExist.append(prices[i]);
                        names.removeAt(i);
                        descripts.removeAt(i);
                        prices.removeAt(i);
                        i--;
                        break;
                    }
                }
            }

            for (int i = 0; i < gameForUpdate->item_collection->item_amount; i++) //для каждого существующего разработчика
            {
                bool isNExist = false;
                bool isPExist = false;
                bool isDExist = false;
                QString statement = QString();
                QString price = QString();
                for (int j = 0; j < namesExist.count(); j++)
                {
                    if (namesExist[j] == gameForUpdate->item_collection->items[i].name)
                    {
                        price = QString::number(priceExist[j], 'f', 2);
                        statement = descrExist[j];
                        if (descrExist[j] == gameForUpdate->item_collection->items[i].description)
                            isDExist = !isDExist;
                        if (priceExist[j] == gameForUpdate->item_collection->items[i].price)
                            isPExist = !isPExist;
                        isNExist = !isNExist;
                        break;
                    }
                }
                if (names.isEmpty() && i-1 != gameForUpdate->item_collection->item_amount && !isNExist)
                {
                    dbgame->Delete(T_INVENTORY, gameForUpdate->item_collection->items[i].id);
                }
                else if (isNExist && (!isDExist || !isPExist))
                {
                    if (!isDExist)
                        error_text = dbgame->Update("i_description='" + statement + "'",
                                                    gameForUpdate->item_collection->items[i].id, T_INVENTORY);
                    if (!error_text.isEmpty())
                        break;

                    if (!isPExist)
                        error_text = dbgame->Update("i_price=" + price,
                                                    gameForUpdate->item_collection->items[i].id, T_INVENTORY);
                    if (!error_text.isEmpty())
                        break;
                }
                else if (!isNExist) //если данный разработчик был изменен, то вызываем апдейт
                {
                    error_text = dbgame->Update("i_name='" + names[0]
                            + "',i_price=" + QString::number(prices[0], 'f', 2)
                            + ",i_description='" + descripts[0] + "'",
                            gameForUpdate->item_collection->items[i].id, T_INVENTORY);
                    if (!error_text.isEmpty())
                        break;
                    names.removeFirst();
                    descripts.removeFirst();
                    prices.removeFirst();
                }
            }
        }

        for (int i = 0; i < names.count(); i++)
        {
            dbItem item = dbItem();
            item.name = names[i];
            item.price = prices[i];
            item.description = descripts[i];
            error_text = dbgame->CreateItem(&item, gameForUpdate->id);

            if (!error_text.isEmpty())
                break;
        }

        UpdateGame(gameForUpdate->id);
    }
    if (error_text.isEmpty() && noItem)
    {
        error_text = dbgame->DeleteAll(T_INVENTORY, "g_id=" + QString::number(gameForUpdate->id));
        UpdateGame(gameForUpdate->id);
    }
    ui->pushButton_u_gameInventorySave->setEnabled(!error_text.isEmpty());
    ui->label_u_gameInventory_error->setText(error_text);
    ui->label_u_gameInventory_error->setVisible(!error_text.isEmpty());
}

void MainWindow::LoadLibraryGames() //Загрузить игры в библиотеке
{

    if (client->library != nullptr)
    {
        delete client->library;
        client->library = nullptr;
    }
    client->library = new dbLibrary(dbgame->GetGames(client->user->id));

}

void MainWindow::LoadCartGames() //Загрузить игры в корзине
{
    if (client->cart != nullptr)
    {
        if (client->cart->games != nullptr)
        {
            delete [] client->cart->games;
            client->cart->games = nullptr;
        }
        delete client->cart;
        client->cart = nullptr;
    }

    client->cart = new dbCart(dbgame->GetCartGames(client->user->id));
}

void MainWindow::LoadFeaturedGames() //Загрузить случайные 100 игр в магазине
{
    if (store != nullptr)
    {
        if (store->games != nullptr)
            delete [] store->games;
        delete store;
    }

    store = new dbStore(dbgame->GetRandomGames(100));
}

void MainWindow::SearchFeaturedGames() //Поиск игр для магазина
{

    if (store != nullptr)
    {
        if (store->games != nullptr)
            delete [] store->games;
        delete store;
    }

    store = new dbStore(dbgame->GetSearchGames(100, ui->lineEdit_f_searchLine->text().trimmed().replace("'", "''")));
}

void MainWindow::on_stackedWidget_currentChanged(int arg1) //Если пользователь перешел на другую страницу
{
    arg1++;
    dbPage page(qvariant_cast<dbPage>(ui->stackedWidget->currentWidget()->property("page")));
    switch (page)
    {
    case LIBRARY_PAGE_: //Страница библиотеки
    {
        ui->page_library->setVisible(false);
        ui->widget_l_gameInfo->setVisible(false);
        LoadLibraryGames();
        QStringList list = QStringList();
        for (int i = 0; i < client->library->game_amount; i++)
        {
            list.append(client->library->games[i].g_name);
        }
        qStableSort(list);
        ui->listWidget_l_games->clear();
        ui->listWidget_l_games->addItems(list);
        for (int i = 0; i < ui->listWidget_l_games->count(); i++)
        {
            ui->listWidget_l_games->item(i)->setForeground(client->library->games[i].isInstalled ? Qt::white : Qt::gray);
        }
        ui->page_library->setVisible(true);

        if (cur_lib_row != -1)
        {
            emit ui->listWidget_l_games->itemClicked(ui->listWidget_l_games->item(cur_lib_row));
            ui->widget_l_gameInfo->setVisible(true);
        }
        break;
    }
    case FEATURED_PAGE_: //Страница магазина
    {
        ui->page_featured->hide();
        LoadFeaturedGames();
        ui->lineEdit_f_searchLine->setCompleter(dbgame->GetCompleter());
        QStringList list = QStringList();
        for (int i = 0; i < store->game_amount; i++)
        {
            list.append(store->games[i].g_name);
        }
        qStableSort(list);
        ui->listWidget_f_games->clear();
        ui->listWidget_f_games->addItems(list);
        ui->page_featured->show();
        break;
    }
    case CART_PAGE_: //Страница корзины
    {
        ui->page_cart->hide();
        //Очистка таблицы с играми
        for (int i = 0; i < ui->tableWidget_cart_games->rowCount(); i++)
        {
            ui->tableWidget_cart_games->removeCellWidget(i, 2);
            ui->tableWidget_cart_games->removeCellWidget(i, 3);
        }
        ui->tableWidget_cart_games->clear();
        ui->tableWidget_cart_games->setRowCount(0);
        //Очистка таблицы с предметами
        for (int i = 0; i < ui->tableWidget_cart_items->rowCount(); i++)
        {
            ui->tableWidget_cart_items->removeCellWidget(i, 3);
            ui->tableWidget_cart_items->removeCellWidget(i, 4);
            ui->tableWidget_cart_items->removeCellWidget(i, 5);
        }
        ui->tableWidget_cart_items->clear();
        ui->tableWidget_cart_items->setRowCount(0);
        //Загрузка игр в корзину
        LoadCartGames();
        double sum = 0.0;
        ui->label_cart_empty->setVisible(client->cart == nullptr);
        //Заполнение страницы с корзиной
        if (client->cart != nullptr)
        {
            if (client->cart->games != nullptr)
            {
                int amount = client->cart->game_amount;
                if (amount > 0)
                {
                    ui->tableWidget_cart_games->setRowCount(amount);
                    for (int i = 0; i < amount; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            if (ui->tableWidget_cart_games->item(i, j) == nullptr)
                            {
                                QTableWidgetItem *item = new QTableWidgetItem();
                                ui->tableWidget_cart_games->setItem(i, j, item);
                            }
                        }
                        ui->tableWidget_cart_games->item(i, 0)->setText(client->cart->games[i].name);
                        ui->tableWidget_cart_games->item(i, 1)->setText(QString::number(client->cart->games[i].price, 'f', 2));
                        sum += client->cart->games[i].price;

                        QPushButton *pb = new QPushButton;
                        pb->setText("Go to store page");
                        pb->setProperty("game id", client->cart->games[i].id);
                        connect(pb, &QPushButton::clicked, this, &MainWindow::go_to_store);
                        ui->tableWidget_cart_games->setCellWidget(i, 2, pb);

                        pb = new QPushButton;
                        pb->setText("Remove from cart");
                        pb->setProperty("game id", client->cart->games[i].id);
                        connect(pb, &QPushButton::clicked, this, [&]()
                        {
                            QPushButton *pb = qobject_cast<QPushButton *>(sender());
                            int id = pb->property("game id").toInt();
                            int row = ui->tableWidget_cart_games->currentRow();
                            double sum = client->cart->sum;
                            sum -= ui->tableWidget_cart_games->item(row, 1)->text().toDouble();
                            client->cart->sum = sum;
                            dbgame->DeleteCartGame(id, client->user->id);
                            ui->tableWidget_cart_games->removeCellWidget(row, 2);
                            ui->tableWidget_cart_games->removeCellWidget(row, 3);
                            ui->tableWidget_cart_games->removeRow(row);
                            ui->label_cart_price->setText(QString::number(sum, 'f', 2) + " руб.");

                            ui->stackedWidget->setCurrentWidget(ui->page_load);
                            ui->stackedWidget->setCurrentWidget(ui->page_cart);
                        });
                        ui->tableWidget_cart_games->setCellWidget(i, 3, pb);
                    }

                    ui->tableWidget_cart_games->resizeColumnsToContents();
                }
            }
            if (client->cart->items != nullptr)
            {
                int amount = client->cart->items_amount;
                if (amount > 0)
                {
                    ui->tableWidget_cart_items->setRowCount(amount);
                    for (int i = 0; i < amount; i++)
                    {
                        for (int j = 0; j < 6; j++)
                        {
                            if (ui->tableWidget_cart_items->item(i, j) == nullptr)
                            {
                                QTableWidgetItem *item = new QTableWidgetItem();
                                ui->tableWidget_cart_items->setItem(i, j, item);
                            }
                        }
                        if (client->cart->item_amount[i] == 0)
                        {
                            int id = client->cart->items[i].id;
                            dbgame->DeleteCartItem(id, client->user->id);
                            ui->tableWidget_cart_items->removeRow(i);
                            continue;
                        }
                        ui->tableWidget_cart_items->item(i, 0)->setText(client->cart->items[i].name);
                        ui->tableWidget_cart_items->item(i, 1)->setText(QString::number(client->cart->items[i].price, 'f', 2));
                        ui->tableWidget_cart_items->item(i, 2)->setText(QString::number(client->cart->item_amount[i]));
                        sum += (client->cart->items[i].price * static_cast<double>(client->cart->item_amount[i]));

                        QPushButton *pb = new QPushButton;
                        pb->setText("Go to store page");
                        pb->setProperty("game id", client->cart->items[i].g_id);
                        connect(pb, &QPushButton::clicked, this, &MainWindow::go_to_store);
                        ui->tableWidget_cart_items->setCellWidget(i, 3, pb);

                        pb = new QPushButton;
                        pb->setText("Add more");
                        pb->setProperty("item id", client->cart->items[i].id);
                        pb->setProperty("amount", client->cart->item_amount[i]);
                        connect(pb, &QPushButton::clicked, this, &MainWindow::buy_dialog_window);
                        ui->tableWidget_cart_items->setCellWidget(i, 4, pb);

                        pb = new QPushButton;
                        pb->setText("Remove from cart");
                        pb->setProperty("item id", client->cart->items[i].id);
                        connect(pb, &QPushButton::clicked, this, [&]()
                        {
                            QPushButton *pb = qobject_cast<QPushButton *>(sender());
                            int id = pb->property("item id").toInt();
                            int row = ui->tableWidget_cart_items->currentRow();
                            double sum = client->cart->sum;
                            double price = ui->tableWidget_cart_items->item(row, 1)->text().toDouble();
                            double amount = ui->tableWidget_cart_items->item(row, 2)->text().toDouble();
                            sum -= price * amount;
                            client->cart->sum = sum;
                            dbgame->DeleteCartItem(id, client->user->id);
                            ui->tableWidget_cart_items->removeCellWidget(row, 3);
                            ui->tableWidget_cart_items->removeCellWidget(row, 4);
                            ui->tableWidget_cart_items->removeCellWidget(row, 5);
                            ui->tableWidget_cart_items->removeRow(row);
                            ui->label_cart_price->setText(QString::number(sum, 'f', 2) + " руб.");
                        });
                        ui->tableWidget_cart_items->setCellWidget(i, 5, pb);
                    }

                    ui->tableWidget_cart_items->resizeColumnsToContents();
                }
            }
            client->cart->sum = sum;
        }

        ui->label_cart_price->setText(QString::number(sum, 'f', 2) + " руб.");
        ui->tableWidget_cart_games->setHorizontalHeaderLabels({"NAME","PRICE","STORE","DELETE"});
        ui->tableWidget_cart_items->setHorizontalHeaderLabels({"NAME","PRICE","AMOUNT","STORE","ADD","DELETE"});
        ui->page_cart->show();
        break;
    }
    case ACHIEVEMENTS_PAGE_:
    {
        //SET ACHIEVEMENTS PAGE
        ui->page_achievements->setVisible(false);
        int row = featuredGame->achieve_collection->achieves_amount;
        ui->tableWidget_a_description->setRowCount(row);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (ui->tableWidget_a_description->item(i, j) == nullptr)
                {
                    QTableWidgetItem *item = new QTableWidgetItem;
                    ui->tableWidget_a_description->setItem(i, j, item);
                }
            }

            QString name(featuredGame->achieve_collection->achievements[i].name);
            QString description(featuredGame->achieve_collection->achievements[i].description);
            QString percent("%1%");
            QString strgame_id(QString::number(featuredGame->id));
            QString strachieve_id(QString::number(featuredGame->achieve_collection->achievements[i].id));

            double withGame = static_cast<double>(dbgame->GetSomeAmount(T_LIBRARY, "u_id", "g_id=" + strgame_id));
            double withAchievement = static_cast<double>(dbgame->GetSomeAmount(T_USER_ACHIEVEMENT, "u_id", "a_id=" + strachieve_id));
            double stats = (withAchievement >= 0 && withGame >= 0) ? (withAchievement / withGame * 100.0) : 0.0;

            ui->tableWidget_a_description->item(i, 0)->setText(name);
            ui->tableWidget_a_description->item(i, 1)->setText(description);
            ui->tableWidget_a_description->item(i, 2)->setText(percent.arg(QString::number(stats, 'f', 2)));
        }
        ui->tableWidget_a_description->resizeRowsToContents();
        ui->tableWidget_a_description->resizeColumnsToContents();
        ui->page_achievements->setVisible(true);
        break;
    }
    case USER_ACHIEVEMENTS_PAGE_:
    {
        //SET USER ACHIEVEMENTS PAGE
        ui->page_user_achievements->setVisible(false);
        dbCurrentAchievements *pack = new dbCurrentAchievements(dbgame->GetUserAchievements(libgame_id, profile_user_id));
        int row = pack->achievements_amount;
        ui->label_ua_username->setText(QString("Достижения пользователя %1").arg(dbgame->GetUserName(profile_user_id)));
        ui->tableWidget_ua_description->setRowCount(row);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (ui->tableWidget_ua_description->item(i, j) == nullptr)
                {
                    QTableWidgetItem *item = new QTableWidgetItem;
                    ui->tableWidget_ua_description->setItem(i, j, item);
                }
            }

            QString name(pack->achievements[i].achievement->name);
            QString description(pack->achievements[i].achievement->description);
            bool isEarned = pack->achievements[i].isEarned;

            ui->tableWidget_ua_description->item(i, 0)->setText(name);
            ui->tableWidget_ua_description->item(i, 1)->setText(description);
            ui->tableWidget_ua_description->item(i, 2)->setText(isEarned ? "Earned" : "Not Earned");
        }
        for (int i = 0; i < pack->achievements_amount; i++)
        {
            delete pack->achievements[i].achievement;
        }
        delete [] pack->achievements;
        delete pack;

        ui->tableWidget_ua_description->resizeRowsToContents();
        ui->tableWidget_ua_description->resizeColumnsToContents();
        ui->page_user_achievements->setVisible(true);
        break;
    }
    case MARKET_PAGE_:
    {
        ui->lineEdit_m_itemName->clear();
        ui->lineEdit_m_gameName->clear();
        QString game_name = gameName;
        ui->lineEdit_m_gameName->setCompleter(dbgame->GetCompleter());

        for (int i = 0; i < ui->tableWidget_m_items->rowCount(); i++)
        {
            ui->tableWidget_m_items->removeCellWidget(i, 3);
            ui->tableWidget_m_items->removeCellWidget(i, 4);
        }
        ui->tableWidget_m_items->clear();
        ui->tableWidget_m_items->setRowCount(0);

        if (market != nullptr)
        {
            if (market->items != nullptr)
            {
                delete [] market->items;
                market->items = nullptr;
            }

            delete market;
            market = nullptr;
        }
        if (!game_name.isEmpty())
        {
            ui->lineEdit_m_gameName->setText(game_name);
            market = new dbItemCollection(dbgame->GetItems(game_name));
        }
        if (market != nullptr)
        {
            ui->tableWidget_m_items->setRowCount(market->item_amount);
            for (int i = 0; i < market->item_amount; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (ui->tableWidget_m_items->item(i, j) == nullptr)
                    {
                        QTableWidgetItem *item = new QTableWidgetItem;
                        ui->tableWidget_m_items->setItem(i, j, item);
                    }
                }

                ui->tableWidget_m_items->item(i, 0)->setText(market->items[i].name);
                ui->tableWidget_m_items->item(i, 1)->setText(market->items[i].description);
                ui->tableWidget_m_items->item(i, 2)->setText(QString::number(market->items[i].price, 'f', 2));

                QPushButton *pb = new QPushButton();
                pb->setText(featuredGame->name);
                connect(pb, &QPushButton::clicked, this, [&](){LoadFeaturedGame(featuredGame->id);});
                ui->tableWidget_m_items->setCellWidget(i, 3, pb);

                pb = new QPushButton();
                pb->setText("Buy item");
                pb->setProperty("item id", market->items[i].id);
                connect(pb, &QPushButton::clicked, this, &MainWindow::buy_dialog_window);
                ui->tableWidget_m_items->setCellWidget(i, 4, pb);
            }
        }
        gameName = QString();
        ui->tableWidget_m_items->resizeColumnsToContents();
        ui->tableWidget_m_items->setHorizontalHeaderLabels({"NAME","DESCRIPTION","PRICE","GAME","BUY"});
        break;
    }
    case INVENTORY_PAGE_:
    {
        ui->page_inventory->setVisible(false);
        ui->label_inventory_username->setText(QString("Инвентарь пользователя %1").arg(dbgame->GetUserName(profile_user_id)));
        QList<dbUserItem> items = dbgame->GetItems(libgame_id, profile_user_id);
        ui->tableWidget_inventory->clear();
        ui->tableWidget_inventory->setRowCount(0);

        if (!items.isEmpty())
        {
            ui->tableWidget_inventory->setRowCount(items.count());
            for (int i = 0; i < items.count(); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (ui->tableWidget_inventory->item(i, j) == nullptr)
                    {
                        QTableWidgetItem *item = new QTableWidgetItem;
                        ui->tableWidget_inventory->setItem(i, j, item);
                    }
                }

                ui->tableWidget_inventory->item(i, 0)->setText(items[i].item->name);
                ui->tableWidget_inventory->item(i, 1)->setText(items[i].item->description);
                ui->tableWidget_inventory->item(i, 2)->setText(QString::number(items[i].item->price, 'f', 2));
                ui->tableWidget_inventory->item(i, 3)->setText(QString::number(items[i].amount));
            }
        }

        foreach (dbUserItem item, items)
        {
            delete item.item;
        }
        items.clear();

        ui->tableWidget_inventory->resizeColumnsToContents();
        ui->tableWidget_inventory->setHorizontalHeaderLabels({"NAME","DESCRIPTION","PRICE","AMOUNT"});

        ui->page_inventory->setVisible(true);
        break;
    }
    case PROFILE_PAGE_:
    {
        ui->page_profile->setVisible(false);
        ui->pushButton_profile_username->setEnabled(false);
        ui->label_profile_error->setVisible(false);
        ui->comboBox_profile_game->clear();
        if (profile_user_id == client->user->id)
        {
            ui->label_profile_username_->setVisible(false);
            ui->lineEdit_profile_username->setVisible(true);
            ui->pushButton_profile_username->setVisible(true);

            ui->lineEdit_profile_username->setText(client->user->name);
        }
        else
        {
            ui->label_profile_username_->setVisible(true);
            ui->lineEdit_profile_username->setVisible(false);
            ui->pushButton_profile_username->setVisible(false);

            ui->label_profile_username_->setText(dbgame->GetUserName(profile_user_id));
        }
        dbLibrary *library = new dbLibrary(dbgame->GetGames(profile_user_id));

        QStringList names = QStringList();
        for (int i = 0; i < library->game_amount; i++)
        {
            names.append(library->games[i].g_name);
        }
        ui->comboBox_profile_game->addItems(names);

        if (library->games != nullptr)
        {
            delete [] library->games;
            library->games = nullptr;
        }
        delete library;

        ui->page_profile->setVisible(true);
        break;
    }
    default:break;
    }
}

void MainWindow::go_to_store() //Если пользователь нажал кнопку войти в магазин в корзине
{
    QPushButton *pb = qobject_cast<QPushButton *>(sender());

    int id = pb->property("game id").toInt();

    this->LoadFeaturedGame(id);
}


void MainWindow::on_pushButton_f_search_clicked() //Строка поиска игр в магазине
{
    if (!ui->lineEdit_f_searchLine->text().trimmed().isEmpty())
    {
        SearchFeaturedGames();
    }
    else
    {
        LoadFeaturedGames();
    }
    QStringList list = QStringList();
    for (int i = 0; i < store->game_amount; i++)
    {
        list.append(store->games[i].g_name);
    }
    qStableSort(list);
    ui->listWidget_f_games->clear();
    ui->listWidget_f_games->addItems(list);
}

void MainWindow::LoadFeaturedGame(int id) //Загрузить страницу магазина для игры
{
    QString error_text = QString();
    if (featuredGame != nullptr)
    {
        delete featuredGame;
        featuredGame = nullptr;
    }
    featuredGame = dbgame->GetGame(id, error_text);
    QList<dbReview> reviews = dbgame->GetReviews(id);
    if (!error_text.isEmpty())
    {
        ui->label_error->setText(error_text);
        ui->stackedWidget->setCurrentWidget(ui->page_error);
    }
    else
    {
        QString price(QString::number(featuredGame->price, 'f', 2) + " руб.");
        QStringList devs = QStringList();
        QStringList pubs = QStringList();
        if (featuredGame->dev_collection != nullptr)
        {
            for (int i = 0; i < featuredGame->dev_collection->devs_amount; i++)
            {
                devs.append(featuredGame->dev_collection->developers[i].name);
            }
        }
        ui->label_f_gameDev_->setText(devs.isEmpty() ? "No Info" : devs.join(","));
        if (featuredGame->publish_collection != nullptr)
        {
            for (int i = 0; i < featuredGame->publish_collection->publishers_amount; i++)
            {
                pubs.append(featuredGame->publish_collection->publishers[i].name);
            }
        }
        ui->label_f_gamePub_->setText(pubs.isEmpty() ? "No Info" : pubs.join(","));
        ui->label_f_gameName->setText(featuredGame->name);
        ui->label_f_gamePrice->setText(price);
        ui->textEdit_f_gameSDescript->setText(featuredGame->sdescript);
        ui->textEdit_f_gameDescript->setText(featuredGame->descript);
        bool enableToBuy = true;
        for (int i = 0; i < client->library->game_amount; i++)
        {
            //Если игра уже в библиотеке
            if (client->library->games[i].g_id == featuredGame->id)
            {
                enableToBuy = !enableToBuy;
                break;
            }
        }
        ui->pushButton_f_game_writeReview->setEnabled(!enableToBuy);
        ui->pushButton_f_game_writeReview->setToolTip(enableToBuy ? "Сначала купите игру" : QString());

        bool hasAchievements = featuredGame->achieve_collection != nullptr;
        ui->pushButton_f_gameAchievements->setEnabled(hasAchievements);
        ui->pushButton_f_gameAchievements->setToolTip(!hasAchievements ? "У игры нет достижений" : QString());

        bool hasInventory = featuredGame->item_collection != nullptr;
        ui->pushButton_f_gameItems->setEnabled(hasInventory);
        ui->pushButton_f_gameItems->setToolTip(!hasInventory ? "У игры нет предметов" : QString());

        ui->label_f_gameRating_2->setVisible(!reviews.isEmpty());
        ui->comboBox_f_gameRating->setVisible(!reviews.isEmpty());
        ui->tableWidget_f_gameReviews->setVisible(!reviews.isEmpty());

        for (int i = 0; i < ui->tableWidget_f_gameReviews->rowCount(); i++)
        {
            ui->tableWidget_f_gameReviews->removeCellWidget(i, 0);
        }
        ui->tableWidget_f_gameReviews->clear();
        ui->tableWidget_f_gameReviews->setRowCount(0);
        if (!reviews.isEmpty())
        {
            ui->tableWidget_f_gameReviews->setRowCount(reviews.count());
            int ratingSum = 0;
            for (int i = 0; i < reviews.count(); i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (ui->tableWidget_f_gameReviews->item(i, j) == nullptr)
                    {
                        QTableWidgetItem *item = new QTableWidgetItem;
                        ui->tableWidget_f_gameReviews->setItem(i, j, item);
                    }
                }
                QString username(dbgame->GetUserName(reviews[i].u_id));
                QString text(reviews[i].text);
                QString rating(QString("%1/10").arg(reviews[i].rating));
                ratingSum += reviews[i].rating;

                QPushButton *pb = new QPushButton(username, this);
                connect(pb, &QPushButton::clicked, this, [&]()
                {
                    profile_user_id = dbgame->GetID(QString("u_username='%1'").arg(qobject_cast<QPushButton*>(sender())->text()), T_USER);
                    ui->stackedWidget->setCurrentWidget(ui->page_profile);
                });
                ui->tableWidget_f_gameReviews->setCellWidget(i, 0, pb);
                ui->tableWidget_f_gameReviews->item(i, 1)->setText(text);
                ui->tableWidget_f_gameReviews->item(i, 2)->setText(rating);
            }

            double rating = static_cast<double>(ratingSum) / static_cast<double>(reviews.count());
            ui->label_f_gameRating_->setText(QString::number(rating, 'f', 1) + "/10");
            ui->tableWidget_f_gameReviews->resizeColumnsToContents();
            ui->tableWidget_f_gameReviews->resizeRowsToContents();
            ui->tableWidget_f_gameReviews->setHorizontalHeaderLabels({"NAME","REVIEW","RATING"});
            this->ResizeReviewsTable();
        }
        else
        {
            ui->tableWidget_f_gameReviews->setRowCount(0);
            ui->label_f_gameRating_->setText("0.0/10");
        }

        for (int i = 0; i < client->cart->game_amount; i++)
        {
            //Если игра уже в корзине для покупок
            if (client->cart->games[i].id == featuredGame->id)
            {
                enableToBuy = !enableToBuy;
                break;
            }
        }
        ui->pushButton_f_gameAddToCart->setEnabled(enableToBuy);

        ui->stackedWidget->setCurrentWidget(ui->page_f_game);
    }
}

void MainWindow::ResizeReviewsTable() //изменить размер таблицы отзывов
{
    int length = 30;
    for(int i = 0; i < ui->tableWidget_f_gameReviews->rowCount(); i++)
        length += ui->tableWidget_f_gameReviews->rowHeight(i);
    ui->tableWidget_f_gameReviews->setFixedHeight(length);
}

void MainWindow::on_listWidget_f_games_itemDoubleClicked(QListWidgetItem *item) //В магазине пользователь выбрал игру и дважды кликнул
{
    for (int i = 0; i < store->game_amount; i++)
    {
        if (item->text() == store->games[i].g_name)
        {
            LoadFeaturedGame(store->games[i].g_id);
            break;
        }
    }
}


void MainWindow::on_lineEdit_l_searchLine_textEdited(const QString &arg1) //Строка поиска игр в библиотеке
{
    QFont font(ui->lineEdit_l_searchLine->font());
    font.setItalic(arg1.trimmed().isEmpty());
    ui->lineEdit_l_searchLine->setFont(font);

    LoadLibraryGames();
    QStringList list = QStringList();
    for (int i = 0; i < client->library->game_amount; i++)
    {
        list.append(client->library->games[i].g_name);
    }

    if (arg1.trimmed().isEmpty())
    {
        qStableSort(list);
        ui->listWidget_l_games->clear();
        ui->listWidget_l_games->addItems(list);
    }
    else
    {
        QStringList newlist = QStringList();
        foreach (QString name, list)
        {
            if (name.contains(arg1))
            {
                newlist.append(name);
            }
        }
        ui->listWidget_l_games->clear();
        if (!newlist.isEmpty())
        {
            qStableSort(newlist);
            ui->listWidget_l_games->addItems(newlist);
        }
    }
}


void MainWindow::on_listWidget_l_games_itemClicked(QListWidgetItem *item) //Пользователь в библиотеке выбрал игру, нажав на нее
{
    ui->widget_l_gameInfo->setVisible(false);
    ui->listWidget_l_games->setCurrentItem(item);
    cur_lib_row = ui->listWidget_l_games->currentRow();
    for (int i = 0; i < client->library->game_amount; i++) //Для каждой игры из библиотеки
    {
        if (item->text() == client->library->games[i].g_name) //Если название игры из библиотеки сопоставляется с названием выбранной игры
        {
            libgame_id = client->library->games[i].g_id;
            ui->label_l_gameName->setText(client->library->games[i].g_name);
            int timeplayed = client->library->games[i].timeplayed;
            QString strtime = QString();
            if (timeplayed / 60 < 1)
                strtime = QString::number(timeplayed % 60) + " seconds";
            else if (timeplayed / 3600 < 1)
                strtime = QString::number(timeplayed / 60) + " minutes";
            else
                strtime = QString::number(timeplayed / 3600) + "," + QString::number(timeplayed / 60 * 100) + " hours";
            ui->label_l_gamePlayTime_->setText(strtime);

            ui->pushButton_l_delete->setEnabled(client->library->games[i].isInstalled);
            ui->pushButton_l_delete->setToolTip(!client->library->games[i].isInstalled ? "Игра не установлена" : QString());
            if (client->library->games[i].isInstalled)
            {
                ui->pushButton_l_gamePlay->setText("PLAY");
                ui->pushButton_l_gamePlay->setStyleSheet(GREEN_BUTTON);
            }
            else
            {
                ui->pushButton_l_gamePlay->setText("INSTALL");
                QString changedStyle(GREEN_BUTTON);

                changedStyle.replace("green", "rgba(0,0,232,1)");
                changedStyle.replace("0,150,0,1","0,0,255,1");
                ui->pushButton_l_gamePlay->setStyleSheet(changedStyle);
            }
            bool hasAchievements = dbgame->HasAchievements(libgame_id);
            ui->pushButton_l_achievements->setEnabled(hasAchievements);
            ui->pushButton_l_achievements->setToolTip(!hasAchievements ? "У игры нет достижений" : QString());
            bool hasItems = dbgame->HasInventory(libgame_id);
            ui->pushButton_l_inventory->setEnabled(hasItems);
            ui->pushButton_l_inventory->setToolTip(!hasItems ? "У игры нет предметов" : QString());
            if (hasItems)
            {
                bool hasInventory = dbgame->HasInventory(libgame_id, client->user->id);
                ui->pushButton_l_inventory->setEnabled(hasInventory);
                ui->pushButton_l_inventory->setToolTip(!hasInventory ? "У вас нет предметов данной игры" : QString());
            }
            break;
        }
    }
    ui->widget_l_gameInfo->setVisible(true);
}


void MainWindow::on_pushButton_l_storePage_clicked() //Пользователь открыл страницу игры через кнопку "Store page" в библиотеке
{
    LoadFeaturedGame(libgame_id);
}

void MainWindow::on_action_cart_triggered() //Пользователь открыл корзину через меню
{
    ui->stackedWidget->setCurrentWidget(ui->page_cart);
}

void MainWindow::on_action_profile_triggered() //Пользователь открыл профиль через меню
{
    profile_user_id = client->user->id;
    ui->stackedWidget->setCurrentWidget(ui->page_profile);
}

void MainWindow::on_pushButton_f_gameAddToCart_clicked() //Пользователь нажал кнопку купить игру
{
    dbgame->AddGameToCart(featuredGame->id, client->user->id); //Игра добавляется в корзину
    ui->stackedWidget->setCurrentWidget(ui->page_cart);
}


void MainWindow::on_pushButton_cart_buy_clicked() //Нажата кнопка купить в корзине
{
    QMessageBox::information(this, "Поздравляем вас с приобретением", "Благодарим вас за покупку, приходите еще!");

    dbgame->DeleteCartGames(client->user->id);
    dbgame->DeleteCartItems(client->user->id);

    ui->stackedWidget->setCurrentWidget(ui->page_library);
}

void MainWindow::go_to_write_review_page() //страница написания обзора
{
    QPushButton *pb = qobject_cast<QPushButton *>(sender());

    int from = qvariant_cast<dbPage>(pb->property("from"));
    int currentGameID = -1;

    if (writeReview != nullptr)
    {
        delete writeReview;
        writeReview = nullptr;
    }

    switch (from)
    {
    case STORE_GAME_PAGE_:
    {
        currentGameID = featuredGame->id;
        writeReview = new dbReview(dbgame->GetReview(currentGameID, client->user->id));
        break;
    }
    case LIBRARY_PAGE_:
    {
        currentGameID = libgame_id;
        writeReview = new dbReview(dbgame->GetReview(currentGameID, client->user->id));
        break;
    }
    default:
    {
        writeReview = nullptr;
        break;
    }
    }

    ui->comboBox_wr_rating->setCurrentIndex(0);
    ui->textEdit_wr_review->clear();
    if (writeReview != nullptr)
    {
        if (writeReview->u_id == -1)
        {
            writeReview->u_id = client->user->id;
        }
        if (writeReview->g_id == -1)
        {
            writeReview->g_id = currentGameID;
        }
        if (writeReview->rating != -1)
        {
            ui->comboBox_wr_rating->setCurrentText(QString::number(writeReview->rating));
        }
        if (!writeReview->text.isEmpty())
        {
            ui->textEdit_wr_review->setText(writeReview->text);
        }

        QString amount("%1 / 1024");
        bool isEmpty = ui->textEdit_wr_review->toPlainText().isEmpty();
        QString strnum(isEmpty ? "0" : QString::number(ui->textEdit_wr_review->toPlainText().count()));
        ui->label_wr_amount->setText(amount.arg(strnum));
    }

    ui->stackedWidget->setCurrentWidget(ui->page_writeReview);
}

void MainWindow::on_pushButton_wr_review_clicked() //Написать или изменить обзор
{
    writeReview->text = ui->textEdit_wr_review->toPlainText();
    writeReview->rating = ui->comboBox_wr_rating->currentText().toInt();
    dbgame->WriteReview(*writeReview);
    ui->stackedWidget->setCurrentWidget(ui->page_library);
}


void MainWindow::on_textEdit_wr_review_textChanged() //Изменение текста обзора
{
    QString stramount("%1 / 1024");
    int amount = ui->textEdit_wr_review->toPlainText().count();
    if (amount > MAX_REVIEW_LENGTH)
    {
        int difference = amount - MAX_REVIEW_LENGTH;
        QString newText(ui->textEdit_wr_review->toPlainText());
        newText.chop(difference);
        ui->textEdit_wr_review->setText(newText);
        QTextCursor cursor(ui->textEdit_wr_review->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->textEdit_wr_review->setTextCursor(cursor);
        amount -= difference;
    }

    ui->label_wr_amount->setText(stramount.arg(QString::number(amount)));
}


void MainWindow::on_pushButton_l_gamePlay_clicked() //Играть/установить игру
{
    dbLibGame game = dbgame->GetGame(libgame_id, client->user->id);
    if (game.isInstalled)
    {
        //PLAY GAME
        if (playDialog != nullptr)
        {
            playDialog->close();
        }
        if (playDialog == nullptr)
        {
            playDialog = new DBPlay(dbtask, dbgame, libgame_id, client->user->id, this);
            connect(playDialog, &DBPlay::NewAchievement, this, [&]()
            {
                if (ui->stackedWidget->currentWidget() == ui->page_user_achievements)
                {
                    ui->stackedWidget->setCurrentWidget(ui->page_load);
                    ui->stackedWidget->setCurrentWidget(ui->page_user_achievements);
                }
            });
            connect(playDialog, &DBPlay::CloseSignal, this, [&](int time, int game_id, int user_id)
            {
                int timeplayed = dbgame->GetGame(game_id, user_id).timeplayed + time;
                QString strtimeplayed(QString::number(timeplayed));
                QString strgame_id(QString::number(game_id));
                QString struser_id(QString::number(user_id));

                dbgame->Update("l_timeplayed=" + strtimeplayed, T_LIBRARY, "g_id=" + strgame_id + " AND u_id=" + struser_id);

                if (ui->stackedWidget->currentWidget() == ui->page_library)
                {
                    ui->stackedWidget->setCurrentWidget(ui->page_load);
                    ui->stackedWidget->setCurrentWidget(ui->page_library);
                }

                if (playDialog != nullptr)
                {
                    delete playDialog;
                    playDialog = nullptr;
                }
            });
        }
        playDialog->show();
    }
    else
    {
        //INSTALL GAME
        game.isInstalled = !game.isInstalled;
        QString statement("l_installed=true");
        QString strgame_id(QString::number(game.g_id));
        dbgame->Update(statement, T_LIBRARY, "g_id=" + strgame_id);

        ui->stackedWidget->setCurrentWidget(ui->page_load);
        ui->stackedWidget->setCurrentWidget(ui->page_library);
    }
}


void MainWindow::on_pushButton_l_delete_clicked() //Удалить игру
{
    dbLibGame game = dbgame->GetGame(libgame_id, client->user->id);

    if (game.isInstalled)
    {
        //DELETE GAME
        game.isInstalled = !game.isInstalled;
        QString statement("l_installed=false");
        QString strgame_id(QString::number(game.g_id));
        dbgame->Update(statement, T_LIBRARY, "g_id=" + strgame_id);

        ui->stackedWidget->setCurrentWidget(ui->page_load);
        ui->stackedWidget->setCurrentWidget(ui->page_library);
    }
}

void MainWindow::on_pushButton_f_gameAchievements_clicked() //открыть достижения игры
{
    ui->stackedWidget->setCurrentWidget(ui->page_achievements);
}


void MainWindow::on_pushButton_l_achievements_clicked() //открыть достижения пользователя
{
    ui->stackedWidget->setCurrentWidget(ui->page_user_achievements);
}

void MainWindow::create_achieve_delete() //удаление достижения при создании игры
{
    int row = ui->tableWidget_c_gameAchievements->currentRow();
    ui->tableWidget_c_gameAchievements->removeCellWidget(row, 2);
    ui->tableWidget_c_gameAchievements->removeRow(row);
}

void MainWindow::create_item_delete() //удаление предмета при создании игры
{
    int row = ui->tableWidget_c_gameInventory->currentRow();
    ui->tableWidget_c_gameInventory->removeCellWidget(row, 3);
    ui->tableWidget_c_gameInventory->removeRow(row);
}

void MainWindow::update_achieve_delete() //удаление достижения при обновлении игры
{
    int row = ui->tableWidget_u_gameAchieve->currentRow();
    ui->tableWidget_u_gameAchieve->removeCellWidget(row, 2);
    ui->tableWidget_u_gameAchieve->removeRow(row);
    ui->label_u_gameAchieve_error->setVisible(false);
    ui->pushButton_u_gameAchieveSave->setEnabled(true);
}

void MainWindow::update_item_delete() //удаление предмета при обновлении игры
{
    int row = ui->tableWidget_u_gameInventory->currentRow();
    ui->tableWidget_u_gameInventory->removeCellWidget(row, 3);
    ui->tableWidget_u_gameInventory->removeRow(row);
    ui->label_u_gameInventory_error->setVisible(false);
    ui->pushButton_u_gameInventorySave->setEnabled(true);
}

void MainWindow::on_comboBox_f_gameRating_currentIndexChanged(int index) //поиск по оценке обзора
{
    for (int i = 0; i < ui->tableWidget_f_gameReviews->rowCount(); i++)
    {
        ui->tableWidget_f_gameReviews->removeCellWidget(i, 0);
    }
    ui->tableWidget_f_gameReviews->clear();
    ui->tableWidget_f_gameReviews->setRowCount(0);
    QList<dbReview> reviews = dbgame->GetReviews(featuredGame->id, index ? index : -1);

    ui->tableWidget_f_gameReviews->setRowCount(reviews.count());
    for (int i = 0; i < reviews.count(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (ui->tableWidget_f_gameReviews->item(i, j) == nullptr)
            {
                QTableWidgetItem *item = new QTableWidgetItem;
                ui->tableWidget_f_gameReviews->setItem(i, j, item);
            }
        }
        QString username(dbgame->GetUserName(reviews[i].u_id));
        QString text(reviews[i].text);
        QString rating(QString("%1/10").arg(reviews[i].rating));

        QPushButton *pb = new QPushButton(username, this);
        connect(pb, &QPushButton::clicked, this, [&]()
        {
            profile_user_id = dbgame->GetID(QString("u_username='%1'").arg(qobject_cast<QPushButton*>(sender())->text()), T_USER);
            ui->stackedWidget->setCurrentWidget(ui->page_profile);
        });
        ui->tableWidget_f_gameReviews->setCellWidget(i, 0, pb);
        ui->tableWidget_f_gameReviews->item(i, 1)->setText(text);
        ui->tableWidget_f_gameReviews->item(i, 2)->setText(rating);
    }
    ui->tableWidget_f_gameReviews->setHorizontalHeaderLabels({"NAME","REVIEW","RATING"});
}


void MainWindow::on_pushButton_m_search_clicked() //поиск предметов на торговой площадке
{
    QString game_name = ui->lineEdit_m_gameName->text().trimmed().replace("'", "''");
    QString item_name = ui->lineEdit_m_itemName->text().trimmed().replace("'", "''");

    for (int i = 0; i < ui->tableWidget_m_items->rowCount(); i++)
    {
        ui->tableWidget_m_items->removeCellWidget(i, 3);
        ui->tableWidget_m_items->removeCellWidget(i, 4);
    }
    ui->tableWidget_m_items->clear();
    ui->tableWidget_m_items->setRowCount(0);

    QList<dbItem> items = dbgame->GetItems(game_name, item_name);
    if (!items.isEmpty())
    {
        ui->tableWidget_m_items->setRowCount(items.count());
        for (int i = 0; i < items.count(); i++)
        {
            for (int j = 0; j < 5; j++)
            {
                QTableWidgetItem *item = new QTableWidgetItem;
                ui->tableWidget_m_items->setItem(i, j, item);
            }

            ui->tableWidget_m_items->item(i, 0)->setText(items[i].name);
            ui->tableWidget_m_items->item(i, 1)->setText(items[i].description);
            ui->tableWidget_m_items->item(i, 2)->setText(QString::number(items[i].price, 'f', 2));

            QPushButton *pb = new QPushButton;
            pb->setText(dbgame->GetGame(items[i].g_id).name);
            pb->setProperty("game id", items[i].g_id);
            connect(pb, &QPushButton::clicked, this, [&]()
            {
                QPushButton *pb = qobject_cast<QPushButton *>(sender());
                this->LoadFeaturedGame(pb->property("game id").toInt());
            });
            ui->tableWidget_m_items->setCellWidget(i, 3, pb);

            pb = new QPushButton;
            pb->setText("Buy item");
            pb->setProperty("item id", items[i].id);
            connect(pb, &QPushButton::clicked, this, &MainWindow::buy_dialog_window);
            ui->tableWidget_m_items->setCellWidget(i, 4, pb);
        }
    }

    ui->tableWidget_m_items->resizeColumnsToContents();
    ui->tableWidget_m_items->setHorizontalHeaderLabels({"NAME","DESCRIPTION","PRICE","GAME","BUY"});
}

void MainWindow::buy_dialog_window() //окно покупки предмета
{
    QPushButton *pb = qobject_cast<QPushButton *>(sender());
    int item_id = pb->property("item id").toInt();
    bool fl;
    int amount = pb->property("amount").toInt(&fl);
    if (itemDialog != nullptr)
    {
        delete itemDialog;
        itemDialog = nullptr;
    }

    itemDialog = new DBItemBuying(dbtask, client->user->id, item_id, fl ? amount : 1, this);
    connect(itemDialog, &DBItemBuying::closeSignal, this, &MainWindow::close_item_dialog);
    itemDialog->exec();

    if (ui->stackedWidget->currentWidget() == ui->page_cart)
    {
        ui->stackedWidget->setCurrentWidget(ui->page_load);
        ui->stackedWidget->setCurrentWidget(ui->page_cart);
    }
}

void MainWindow::close_item_dialog() //закрытие окна покупки предмета
{
    if (itemDialog != nullptr)
    {
        delete itemDialog;
        itemDialog = nullptr;
    }
}

void MainWindow::on_pushButton_l_inventory_clicked() //открытие инветоря пользователя
{
    ui->stackedWidget->setCurrentWidget(ui->page_inventory);
}


void MainWindow::on_comboBox_profile_game_currentIndexChanged(const QString &arg1) //выбор игры в профиле
{
    if (!arg1.isEmpty())
    {
        int game_id = dbgame->GetID("g_name='" + arg1 + "'");
        libgame_id = game_id;
        bool hasAchievements = dbgame->HasAchievements(game_id);
        ui->pushButton_profile_achievements->setEnabled(hasAchievements);
        ui->pushButton_profile_achievements->setToolTip(!hasAchievements ? "У игры нет достижений" : QString());
        bool hasItems = dbgame->HasInventory(game_id);
        ui->pushButton_profile_inventory->setEnabled(hasItems);
        ui->pushButton_profile_inventory->setToolTip(!hasItems ? "У игры нет предметов" : QString());
        if (hasItems)
        {
            bool hasInventory = dbgame->HasInventory(game_id, profile_user_id);
            ui->pushButton_profile_inventory->setEnabled(hasInventory);
            ui->pushButton_profile_inventory->setToolTip(!hasInventory ? "У пользователя нет предметов данной игры" : QString());
        }

        ui->pushButton_profile_achievements->setEnabled(dbgame->HasAchievements(game_id));
    }
    else
    {
        ui->pushButton_profile_achievements->setEnabled(false);
        ui->pushButton_profile_achievements->setToolTip("Выберите игру");
        ui->pushButton_profile_inventory->setEnabled(false);
        ui->pushButton_profile_inventory->setToolTip("Выберите игру");
    }
}


void MainWindow::on_pushButton_profile_achievements_clicked() //посмотреть достижения пользователя из профиля
{
    ui->stackedWidget->setCurrentWidget(ui->page_user_achievements);
}


void MainWindow::on_pushButton_profile_inventory_clicked() //посмотреть инвентарь пользователя из профиля
{
    ui->stackedWidget->setCurrentWidget(ui->page_inventory);
}

void MainWindow::on_pushButton_profile_username_clicked() //изменить имя пользователя
{
    QString name(ui->lineEdit_profile_username->text());
    QString error_text(DBCheck::Check(name, 8));
    if (error_text.isEmpty())
    {
        error_text = dbgame->Update("u_username='" + QString(name).replace("'","''") + "'",
                                    T_USER, "id=" + QString::number(client->user->id));
        if (error_text.isEmpty())
        {
            client->user->name = name;
            ui->menu->setTitle(client->user->name);
            toolbar["profile"]->setText(client->user->name.toUpper());
            ui->pushButton_profile_username->setEnabled(false);
        }
    }
    ui->label_profile_error->setText(error_text);
    ui->label_profile_error->setVisible(!error_text.isEmpty());
}

