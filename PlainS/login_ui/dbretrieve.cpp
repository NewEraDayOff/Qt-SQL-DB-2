#include "dbretrieve.h"
#include "ui_dbretrieve.h"

DBRetrieve::DBRetrieve(DBTask *dbt, QWidget *parent) : //конструктор
    QDialog(parent),
    ui(new Ui::DBRetrieve)
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

    ui->label_password->hide();
    ui->lineEdit_password->hide();

    ui->label_confirm->hide();
    ui->lineEdit_confirm->hide();

    ui->pushButton_confirm->hide();

    dbtask = dbt;
    login = QString();

    ui->lineEdit_login->setFocus();

    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &QDialog::close);
}

DBRetrieve::~DBRetrieve() //деструктор
{
    delete ui;
}

void DBRetrieve::on_pushButton_next_clicked() //кнопка продолжить
{
    const QString curlogin = ui->lineEdit_login->text().replace("'", "''");
    const QString username = ui->lineEdit_name->text().replace("'", "''");

    bool isCorrect = false;

    if (!curlogin.isEmpty() && !username.isEmpty())
    {
        const QString c_login(DBRequest::Statement("u_login", curlogin));
        const QString c_username(DBRequest::Statement("u_username", username));
        const QString condition(DBRequest::ConditionConnector({c_login, c_username}, "AND"));

        const QString request(DBRequest::SelectWhere(T_USER, "id", condition));

        const dbUser requestedUser = dbtask->SelectUser(request, {"id"});

        if (requestedUser.id != -1)
        {
            id = requestedUser.id;

            isCorrect = !isCorrect;

            this->hide();

            ui->label_error->hide();

            ui->label_login->hide();
            ui->lineEdit_login->hide();

            ui->label_name->hide();
            ui->lineEdit_name->hide();

            ui->pushButton_next->hide();

            ui->label_password->show();
            ui->lineEdit_password->show();

            ui->label_confirm->show();
            ui->lineEdit_confirm->show();

            ui->pushButton_confirm->show();

            ui->lineEdit_password->setFocus();

            this->show();
        }
    }

    if (!isCorrect)
    {
        ui->label_error->setText("Логин и/или имя введен/ы неверно.");
        ui->label_error->show();
    }
}

void DBRetrieve::on_pushButton_confirm_clicked() //кнопка подтвердить смену пароля
{
    errors e(E_);

    if (Check(ui->lineEdit_password, e))
    {
        if (ui->lineEdit_password->text().trimmed().isEmpty())
            e = E_PASSWORD_EMPTY;
        else if (ui->lineEdit_password->text().count() < 8)
            e = E_SIZE_PASSWORD;
        else
            if (ui->lineEdit_password->text() != ui->lineEdit_confirm->text())
                e = E_DIFFERENT_PASSWORDS;
    }

    if (e != E_)
        ErrorHandler(&e);
    else
    {
        const QString password = ui->lineEdit_password->text().replace("'", "''");

        const QString c_login(DBRequest::Statement("u_login", ui->lineEdit_login->text().replace("'", "''")));
        const QString c_id(DBRequest::Statement("id", id));
        const QString s_password(DBRequest::Statement("u_password", password));
        const QString condition(DBRequest::ConditionConnector({c_id, c_login}, "AND"));

        const QString request(DBRequest::Update(T_USER, s_password, condition));
        const QString lastError = dbtask->Update(request);

        if (lastError.isEmpty())
        {
            login = ui->lineEdit_login->text();
            this->close();
        }
        else
        {
            ui->label_error->setText(lastError);
        }
    }
}

bool DBRetrieve::Check(QLineEdit *lineEdit, errors &e) //проверка на пробелы и юникод
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

void DBRetrieve::ErrorHandler(errors *e) //обработчик ошибок
{
    QString error_text;

    switch (*e)
    {
    case E_UNICODE: error_text = "Пароль должен быть написан на английском"; break;
    case E_SPACES: error_text = "Пароль не должен содержать пробелов"; break;
    case E_SIZE_PASSWORD: error_text = "Минимальная длина пароля 8 символов"; break;
    case E_DIFFERENT_PASSWORDS: error_text = "Пароли не совпадают"; break;
    case E_PASSWORD_EMPTY: error_text = "Придумайте пароль"; break;
    default: break;
    }

    if (*e != errors::E_)
    {
        ui->label_error->setText(error_text);
        ui->label_error->show();
    }
}

void DBRetrieve::closeEvent(QCloseEvent *event) //событие закрытия окна
{
    emit closeDialog(login);
    event->accept();
}
