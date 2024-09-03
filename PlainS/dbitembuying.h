#ifndef DBITEMBUYING_H
#define DBITEMBUYING_H

#include <QDialog>
#include <QCloseEvent>

#include "task/dbtask.h"
#include "task/dbrequest.h"
#include "dbwork.h"

namespace Ui {
class DBItemBuying;
}

class DBItemBuying : public QDialog
{
    Q_OBJECT

public:
    explicit DBItemBuying(DBTask *dbt, int u_id, int i_id, int amount = 1, QWidget *parent = nullptr);
    ~DBItemBuying();
    void closeEvent(QCloseEvent *event);
signals:
    void closeSignal();
private slots:
    void add_to_cart();
private:
    Ui::DBItemBuying *ui;

    DBWork *work = nullptr;
    DBTask *dbtask = nullptr;
    dbItem *item = nullptr;
    int user_id = -1;
};

#endif // DBITEMBUYING_H
