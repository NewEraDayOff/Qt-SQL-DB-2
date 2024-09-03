#ifndef DBPLAY_H
#define DBPLAY_H

#include <QDialog>
#include <QTimer>
#include <QRandomGenerator>
#include <QShowEvent>
#include <QIcon>

#include "task/dbtask.h"
#include "task/dbrequest.h"
#include "work/dbgame.h"
#include "dbwork.h"

namespace Ui {
class DBPlay;
}

class DBPlay : public QDialog
{
    Q_OBJECT

public:
    explicit DBPlay(DBTask *dbt, DBGame *dbg, int g_id, int u_id, QWidget *parent = nullptr);
    ~DBPlay();
signals:
    void NewAchievement();
    void CloseSignal(int time, int game_id, int user_id);
protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
private slots:
    void timer_tick();
private:
    Ui::DBPlay *ui;
    QTimer *timer = nullptr;
    int game_id = -1;
    int user_id = -1;
    int amount = 0;
    int earned = 0;
    int counter = 0;
    QList<int> left_achievement_id = QList<int>();
    DBTask *dbtask = nullptr;
    DBGame *dbgame = nullptr;
};

#endif // DBPLAY_H
