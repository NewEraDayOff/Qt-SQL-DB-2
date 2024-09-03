#ifndef DBRETRIEVE_H
#define DBRETRIEVE_H

#include <QDialog>
#include <QCloseEvent>
#include <QLineEdit>

#include <QDebug>

#include "task/dbrequest.h"
#include "task/dbtask.h"
#include "task/dbstruct.h"

namespace Ui {
class DBRetrieve;
}

class DBRetrieve : public QDialog
{
    Q_OBJECT

public:
    explicit DBRetrieve(DBTask *dbt, QWidget *parent = nullptr);
    ~DBRetrieve();

    void closeEvent(QCloseEvent * event);
signals:
    void closeDialog(QString name);
private slots:
    void on_pushButton_next_clicked();

    void on_pushButton_confirm_clicked();

private:
    Ui::DBRetrieve *ui;

    int id;
    QString login;

    DBTask *dbtask;

    enum errors
    {
        E_,
        E_UNICODE,
        E_SPACES,
        E_SIZE_PASSWORD,
        E_DIFFERENT_PASSWORDS,
        E_PASSWORD_EMPTY
    };

    void ErrorHandler(errors *e);

    bool Check(QLineEdit *lineEdit, errors &e);
};

#endif // DBRETRIEVE_H
