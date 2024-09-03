#include "dbplay.h"
#include "ui_dbplay.h"

DBPlay::DBPlay(DBTask *dbt, DBGame *dbg, int g_id, int u_id, QWidget *parent) : //конструктор
    QDialog(parent),
    ui(new Ui::DBPlay)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    this->setWindowIcon(QIcon(QPixmap::fromImage(QImage(":/Res/logo.png"))));
    this->setFixedSize(this->size());

    dbtask = dbt;
    dbgame = dbg;
    game_id = g_id;
    user_id = u_id;

    if (!ui->textEdit_log->toPlainText().isEmpty())
    {
        ui->textEdit_log->clear();
    }

    QString request(DBRequest::SelectWhere(T_ACHIEVEMENT, "id", "g_id=" + QString::number(game_id)));
    QList<int> achievement_id = dbtask->SelectID(request);
    request = DBRequest::SelectWhere(T_USER_ACHIEVEMENT, "a_id", "u_id=" + QString::number(user_id));
    QList<int> uachievement_id = dbtask->SelectID(request);

    double ua = uachievement_id.isEmpty() ? 0 : static_cast<double>(uachievement_id.count());
    double a = achievement_id.isEmpty() ? 0 : static_cast<double>(achievement_id.count());
    if (!achievement_id.isEmpty())
    {
        amount = achievement_id.count();
    }
    if (!achievement_id.isEmpty())
    {
        earned = uachievement_id.count();
    }
    foreach (int id, uachievement_id)
    {
        achievement_id.removeOne(id);
    }
    if (achievement_id.isEmpty())
    {
        ui->textEdit_log->append("Все достижения получены!");
        ui->label_progress->setText("Игра пройдена!");
        ui->label_progress->setStyleSheet("color:lime;");

        ui->progressBar_progress->setValue(100);
    }
    else
    {
        ui->label_progress->setText("Игра еще не пройдена!");
        ui->label_progress->setStyleSheet("color:red;");

        double value = ua / a * 100.0;
        ui->progressBar_progress->setValue(static_cast<int>(value));
    }
    QString name(dbgame->GetGame(game_id).name);
    this->setWindowTitle(name);
    ui->label_name->setText(name);
    left_achievement_id = achievement_id;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DBPlay::timer_tick);
}

DBPlay::~DBPlay() //деструктор
{
    if (timer != nullptr)
    {
        if (timer->isActive())
        {
            timer->stop();
        }
        delete timer;
        timer = nullptr;
    }
    delete ui;
}

void DBPlay::showEvent(QShowEvent *event) //событие открытия окна
{
    timer->start(1000);
    event->accept();
}

void DBPlay::closeEvent(QCloseEvent *event) //событие закрытия окна
{
    emit CloseSignal(counter, game_id, user_id);
    event->accept();
}

void DBPlay::timer_tick() //итерация таймера
{
    counter++;
    if (!left_achievement_id.isEmpty() && counter % 10 == 0)
    {
        int id = left_achievement_id[QRandomGenerator::global()->bounded(0, left_achievement_id.count())];
        QString request(DBRequest::SelectWhere(T_ACHIEVEMENT, "g_id, a_name", "id=" + QString::number(id)));
        QList<dbAchieve> a = dbtask->SelectAchievements(request, {"a_name"});
        QString name(a.first().name);
        ui->textEdit_log->append("Получено достижение " + name);
        int u_a_id;
        dbgame->CheckID(T_USER_ACHIEVEMENT, SEQ_USER_ACHIEVEMENT, u_a_id);
        QString stru_a_id(QString::number(u_a_id));
        QString struser_id(QString::number(user_id));
        QString str_id(QString::number(id));
        request = DBRequest::Insert(T_USER_ACHIEVEMENT, {stru_a_id, struser_id, str_id});
        dbtask->Insert(request);
        left_achievement_id.removeOne(id);
        earned++;
        double value = static_cast<double>(earned) / static_cast<double>(amount) * 100.0;
        ui->progressBar_progress->setValue(static_cast<int>(value));
        emit NewAchievement();
        if (left_achievement_id.isEmpty())
        {
            ui->textEdit_log->append("Все достижения получены!");
            ui->label_progress->setText("Игра пройдена!");
            ui->label_progress->setStyleSheet("color:lime;");
            timer->stop();
        }
    }
}
