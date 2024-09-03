#ifndef DBCREATION_H
#define DBCREATION_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QCloseEvent>
#include <QLineEdit>

#include <QDebug>

#include "task/dbrequest.h"
#include "task/dbtask.h"
#include "task/dbstruct.h"
//#include "objective/dbobjective.h"

namespace Ui {
class DBCreation;
}

class DBCreation : public QDialog
{
    Q_OBJECT

public:
    explicit DBCreation(DBTask *dbt, QWidget *parent = nullptr);
    ~DBCreation();

    void closeEvent(QCloseEvent * event);
signals:
    void closeDialog(QString name);

private slots:

    void on_pushButton_done_clicked();

//    void on_pushButton_cancel_clicked();

    void on_lineEdit_login_textEdited(const QString &arg1);

private:
    Ui::DBCreation *ui;

    bool userExist;
    QString login;

    DBTask *dbtask;

    enum errors
    {
        E_,
        E_EXIST,
        E_UNICODE,
        E_SPACES,
        E_SIZE_PASSWORD,
        E_SIZE_NAME,
        E_DIFFERENT_PASSWORDS,
        E_PASSWORD_EMPTY,
        E_NAME_EMPTY
    };

    void ErrorHandler(errors *e);

    bool Check(QLineEdit *lineEdit, errors &e);
};

#endif // DBCREATION_H
