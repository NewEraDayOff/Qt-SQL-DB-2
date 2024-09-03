#include "dblogin.h"
#include "ui_dblogin.h"

DBLogin::DBLogin(DBTask *dbt, QString defaultName, QWidget *parent) : //конструктор
    QDialog(parent),
    ui(new Ui::DBLogin)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    this->setWindowTitle("Plain Login");
    this->setWindowIcon(QIcon(QPixmap::fromImage(QImage(":/Res/logo.png"))));
    this->setFixedSize(this->size());

    ui->label_logo->setPixmap(
                QPixmap::fromImage(
                    QImage(":/Res/plain_wide.png").scaled(ui->label_logo->size(),
                                                          Qt::KeepAspectRatioByExpanding,
                                                          Qt::SmoothTransformation)));
    dbtask = dbt;

    user = dbUser();

    SetDefault();

    ui->lineEdit_login->setFocus();
    if (!defaultName.isEmpty())
        ui->lineEdit_login->setText(defaultName);

    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &QDialog::close);
}

DBLogin::~DBLogin() //деструктор
{
    delete ui;
}

void DBLogin::SetToolTip() //настройка всплывающей подсказки с набором пользователей
{
    const QStringList columns{"u_login", "u_password"};
    const QString request(DBRequest::SelectByRandomLimit(T_USER, columns, 10));

    const QList<dbUser> random10users = dbtask->SelectUsers(request, columns);
    QString tip = QString();

    foreach (dbUser user, random10users)
        tip.append(user.login + " | " + user.password + "\n");

    ui->label_hint->setToolTip(tip);
}

void DBLogin::on_pushButton_login_clicked() //попытка входа в свой профиль
{
    const QString login = ui->lineEdit_login->text().replace("'", "''");
    const QString password = ui->lineEdit_password->text().replace("'", "''");

    bool isCorrect = false;

    if (!login.isEmpty() && !password.isEmpty())
    {
        const QString c_login(DBRequest::Statement("u_login", login));
        const QString c_password(DBRequest::Statement("u_password", password));
        const QString condition(DBRequest::ConditionConnector({c_login, c_password}, "AND"));

        const QString request(DBRequest::SelectAllWhere(T_USER, condition));

        const dbUser requestedUser = dbtask->SelectUser(request);

        if (requestedUser.id != -1)
        {
            user = requestedUser;
            isCorrect = !isCorrect;
            this->close();
        }
    }

    if (!isCorrect)
    {
        ui->label_error->setText("Логин и/или пароль введен/ы неверно.");
        ui->label_error->show();
    }
}

void DBLogin::on_pushButton_createNew_clicked() //создание нового пользователя
{
    this->hide();
    dbCreation = new DBCreation(dbtask);
    connect(dbCreation, &DBCreation::closeDialog, this, &DBLogin::closeDBCreation);
    dbCreation->show();
}

void DBLogin::on_pushButton_retrieve_clicked() //если забыл пароль
{
    this->hide();
    dbRetrieve = new DBRetrieve(dbtask);
    connect(dbRetrieve, &DBRetrieve::closeDialog, this, &DBLogin::closeDBRetrieve);
    dbRetrieve->show();
}

void DBLogin::on_checkBox_showPassword_stateChanged(int arg1) //показать пароль
{
    QLineEdit::EchoMode mode = arg1 ? QLineEdit::EchoMode::Normal : QLineEdit::EchoMode::Password;
    ui->lineEdit_password->setEchoMode(mode);
}

void DBLogin::closeDBCreation(QString name) //закрытие окна создания пользователя
{
    if (dbCreation != nullptr)
    {
        delete dbCreation;
        dbCreation = nullptr;
    }

    SetDefault(name);

    this->show();
}

void DBLogin::closeDBRetrieve(QString name) //закрытие окна восстановления пароля
{
    if (dbRetrieve != nullptr)
    {
        delete dbRetrieve;
        dbRetrieve = nullptr;
    }

    SetDefault(name);

    this->show();
}

void DBLogin::SetDefault(QString name) //установить дефолтные значения
{
    ui->lineEdit_login->setText(name.isEmpty() ? "" : name);

    ui->label_error->hide();
    ui->lineEdit_password->clear();
    ui->checkBox_showPassword->setChecked(false);
    SetToolTip();
}

void DBLogin::closeEvent(QCloseEvent *event) //событие закрытия окна
{
    emit loginSignal(user);
    event->accept();
}
