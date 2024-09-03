#include "dbadmin.h"
#include "ui_dbadmin.h"

DBAdmin::DBAdmin(DBTask *dbt, int index, QWidget *parent) : //конструктор
    QDialog(parent),
    ui(new Ui::DBAdmin)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    this->setWindowTitle("Plain Password Recovery");
    this->setWindowIcon(QIcon(QPixmap::fromImage(QImage(":/Res/logo.png"))));
    this->setFixedSize(this->size());

    ui->label_logo->setPixmap(
                QPixmap::fromImage(
                    QImage(":/Res/plain_wide.png").scaled(ui->label_logo->size(),
                                                          Qt::KeepAspectRatioByExpanding,
                                                          Qt::SmoothTransformation)));
    ui->label_error->hide();

    dbtask = dbt;
    id = index;

    ui->lineEdit_password->setFocus();

    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &QDialog::close);
}

DBAdmin::~DBAdmin() //деструктор
{
    delete ui;
}

void DBAdmin::closeEvent(QCloseEvent *event) //событие закрытия окна
{
    emit closeDialog(fl);
    event->accept();
}

void DBAdmin::on_pushButton_login_clicked() //попытка входа от имени администратора
{
    if (ui->lineEdit_password->text().isEmpty())
        ui->label_error->setText("Введите пароль");
    else
    {
        const QString admin(ui->lineEdit_password->text().replace("'", "''"));
        const QString c_admin(DBRequest::Statement("u_admin", admin));
        const QString c_id(DBRequest::Statement("id", id));
        const QString conditions(DBRequest::ConditionConnector({c_id, c_admin}, "AND"));
        const QString request(DBRequest::SelectWhere(T_USER, "u_admin", conditions));
        const dbUser user = dbtask->SelectUser(request, {"u_admin"});

        if (user.admin == admin)
        {
            fl = true;
            this->close();
        }
        else
        {
            ui->label_error->setText("Ошибка. Пароль не верен");
            ui->label_error->show();
        }
    }
}

