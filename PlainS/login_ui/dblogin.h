#ifndef DBLOGIN_H
#define DBLOGIN_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QIcon>
#include <QDebug>
#include <QCloseEvent>

#include "task/dbtask.h"
#include "login_ui/dbcreation.h"
#include "login_ui/dbretrieve.h"

namespace Ui {
class DBLogin;
}

class DBLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DBLogin(DBTask *dbt, QString defName = nullptr, QWidget *parent = nullptr);
    ~DBLogin();

    void closeEvent(QCloseEvent * event);

    void SetDefault(QString name = nullptr);
signals:
    void loginSignal(dbUser user);
private slots:
    void on_pushButton_login_clicked();

//    void on_pushButton_cancel_clicked();

    void on_pushButton_createNew_clicked();

    void on_checkBox_showPassword_stateChanged(int arg1);

    void on_pushButton_retrieve_clicked();

    void closeDBCreation(QString name = nullptr);

    void closeDBRetrieve(QString name = nullptr);
private:
    Ui::DBLogin *ui;

    DBCreation *dbCreation;
    DBRetrieve *dbRetrieve;

    DBTask *dbtask;

    dbUser user;

    void SetToolTip();
};

#endif // DBLOGIN_H
