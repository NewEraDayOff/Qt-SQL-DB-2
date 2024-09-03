#ifndef DBADMIN_H
#define DBADMIN_H

#include <QDialog>
#include <QCloseEvent>

#include "task/dbtask.h"
#include "task/dbrequest.h"

namespace Ui {
class DBAdmin;
}

class DBAdmin : public QDialog
{
    Q_OBJECT

public:
    explicit DBAdmin(DBTask *dbt, int index, QWidget *parent = nullptr);
    ~DBAdmin();
    void closeEvent(QCloseEvent * event);
signals:
    void closeDialog(bool signal = false);
private slots:
    void on_pushButton_login_clicked();

private:
    Ui::DBAdmin *ui;

    int id;
    bool fl = false;
    DBTask *dbtask;
};

#endif // DBADMIN_H
