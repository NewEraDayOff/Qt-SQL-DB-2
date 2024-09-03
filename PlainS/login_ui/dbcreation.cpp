#include "dbcreation.h"
#include "ui_dbcreation.h"

DBCreation::DBCreation(DBTask *dbt, QWidget *parent) : //конструктор
    QDialog(parent),
    ui(new Ui::DBCreation)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    this->setWindowTitle("Plain Create Account");
    this->setWindowIcon(QIcon(QPixmap::fromImage(QImage(":/Res/logo.png"))));
    this->setFixedSize(this->size());

    ui->label_logo->setPixmap(
                QPixmap::fromImage(
                    QImage(":/Res/plain_wide.png").scaled(ui->label_logo->size(),
                                                          Qt::KeepAspectRatioByExpanding,
                                                          Qt::SmoothTransformation)));

    ui->label_available_icon->setPixmap(
                QPixmap::fromImage(
                    QImage(":/Res/available_check_mark.png").scaled(ui->label_available_icon->size(),
                                                                    Qt::KeepAspectRatioByExpanding,
                                                                    Qt::SmoothTransformation)));
    ui->widget->hide();
    ui->label_error->hide();
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_confirm->setEchoMode(QLineEdit::Password);

    dbtask = dbt;
    login = QString();
    userExist = false;

    ui->lineEdit_login->setFocus();

    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &QDialog::close);
}

DBCreation::~DBCreation() //деструктор
{
    delete ui;
}

bool DBCreation::Check(QLineEdit *lineEdit, errors &e) //проверка на пробелы и юникод
{
    QRegExp spaces("\\s");
    if (!lineEdit->text().contains(spaces))
    {
        foreach (QChar c, lineEdit->text())
        {
            if (c.unicode() > 127)
            {
                e = E_UNICODE;
                break;
            }
        }
    }
    else
        e = E_SPACES;

    return e == E_;
}

void DBCreation::on_pushButton_done_clicked() //попытка создания юзера
{
    errors e(E_);

    if (Check(ui->lineEdit_login, e))
    {
        if (ui->lineEdit_login->text().trimmed().isEmpty())
            e = E_NAME_EMPTY;
        else if (ui->lineEdit_login->text().count() < 4)
            e = E_SIZE_NAME;
        else
        {
            if (Check(ui->lineEdit_password, e))
            {
                if (ui->lineEdit_password->text().trimmed().isEmpty())
                    e = E_PASSWORD_EMPTY;
                else if (ui->lineEdit_password->text().count() < 8)
                    e = E_SIZE_PASSWORD;
                else
                {
                    if (Check(ui->lineEdit_confirm, e))
                        if (ui->lineEdit_password->text() != ui->lineEdit_confirm->text())
                            e = E_DIFFERENT_PASSWORDS;
                }
            }
        }
    }

    if (userExist)
        e = E_EXIST;

    if (e != E_)
        ErrorHandler(&e);
    else
    {
        ui->pushButton_done->setEnabled(false);

        QString request(DBRequest::SelectLastID(T_USER, SEQ_USER));
        const QString id(QString::number(dbtask->SelectUser(request, {"id"}).id));

        const QString name("'" + ui->lineEdit_login->text().replace("'", "''") + "'");
        const QString password("'" + ui->lineEdit_password->text().replace("'", "''") + "'");

        request = DBRequest::Insert(T_USER, {id, name, password, name});

        const QString lastError(dbtask->Insert(request));

        if (lastError.isEmpty())
        {
            login = ui->lineEdit_login->text();
            this->close();
        }
        else
        {
            ui->label_error->setText(lastError);
            ui->pushButton_done->setEnabled(true);
        }
    }
}

void DBCreation::ErrorHandler(errors *e) //обработчик ошибок
{
    QString error_text;

    switch (*e)
    {
    case E_EXIST: error_text = "Такой логин уже занят"; break;
    case E_UNICODE: error_text = "Логин и пароль должны быть написаны на английском"; break;
    case E_SPACES: error_text = "Логин и пароль не должны содержать пробелов"; break;
    case E_SIZE_PASSWORD: error_text = "Минимальная длина пароля 8 символов"; break;
    case E_SIZE_NAME: error_text = "Минимальная длина логина 4 символа"; break;
    case E_DIFFERENT_PASSWORDS: error_text = "Пароли не совпадают"; break;
    case E_PASSWORD_EMPTY: error_text = "Придумайте пароль"; break;
    case E_NAME_EMPTY: error_text = "Придумайте логин"; break;
    default: break;
    }

    if (*e != errors::E_)
    {
        ui->label_error->setText(error_text);
        ui->label_error->show();
    }
}

void DBCreation::closeEvent(QCloseEvent *event) //событие закрытия окна
{
    emit closeDialog(login);
    event->accept();
}

void DBCreation::on_lineEdit_login_textEdited(const QString &arg1) //проверка имени на существование
{
    const QString column("u_login");
    QString text(arg1);
    const QString name(text.replace("'", "''"));
    const QString condition(DBRequest::Statement(column, name));

    const QString request(DBRequest::SelectWhere(T_USER, column, condition));

    const QString existLogin = dbtask->SelectUser(request, {column}).login;

    errors e(E_);

    if (!existLogin.isEmpty())
        e = E_EXIST;
    else
    {
        if (Check(ui->lineEdit_login, e))
        {
            if (ui->lineEdit_login->text().trimmed().isEmpty())
                e = E_NAME_EMPTY;
            else if (ui->lineEdit_login->text().count() < 4)
                e = E_SIZE_NAME;   
        }
    }
    userExist = e == E_EXIST;
    ui->widget->setVisible(e == E_);
    ui->label_error->setVisible(e != E_);

    ErrorHandler(&e);
}
