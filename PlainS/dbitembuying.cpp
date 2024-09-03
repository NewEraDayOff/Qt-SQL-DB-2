#include "dbitembuying.h"
#include "ui_dbitembuying.h"

DBItemBuying::DBItemBuying(DBTask *dbt, int u_id, int i_id, int amount, QWidget *parent) : //конструктор
    QDialog(parent),
    ui(new Ui::DBItemBuying)
{
    ui->setupUi(this);

    dbtask = dbt;
    user_id = u_id;

    QString request(DBRequest::SelectAllWhere(T_INVENTORY, "id=" + QString::number(i_id)));
    QList<dbItem> i = dbtask->SelectItems(request);
    item = new dbItem(i.first());

    work = new DBWork(dbt, this);

    ui->label_name->setText(item->name);
    ui->spinBox_amount->setMinimum(0 - amount);

    connect(ui->pushButton_buy, &QPushButton::clicked, this, &DBItemBuying::add_to_cart);
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &DBItemBuying::close);
}

DBItemBuying::~DBItemBuying() //деструктор
{
    if (work != nullptr)
    {
        delete work;
        work = nullptr;
    }
    if (item != nullptr)
    {
        delete item;
        item = nullptr;
    }
    delete ui;
}

void DBItemBuying::closeEvent(QCloseEvent *event) //событие закрытия окна
{
    emit closeSignal();
    event->accept();
}

void DBItemBuying::add_to_cart() //добавить в корзину
{
    const QString struser_id(QString::number(user_id));
    const QString stritem_id(QString::number(item->id));
    QString request(DBRequest::SelectWhere(T_CART_INVENTORY, "id", "i_id=" + stritem_id + " AND u_id=" + struser_id));
    int user_item_id = dbtask->Select(request);
    int amount = -1;

    if (user_item_id + 1)
    {
        QString struser_item_id(QString::number(user_item_id));
        request = DBRequest::SelectWhere(T_CART_INVENTORY, "c_i_amount", "id=" + struser_item_id);
        amount = dbtask->Select(request);
        amount += ui->spinBox_amount->value();
        QString stra(QString::number(amount));
        request = DBRequest::Update(T_CART_INVENTORY, "c_i_amount=" + stra, "id=" + struser_item_id);
    }
    else
    {
        int id;
        work->CheckID(T_CART_INVENTORY, SEQ_CART_INVENTORY, id);
        QString str_id(QString::number(id));
        QString stra(QString::number(ui->spinBox_amount->value()));
        request = DBRequest::Insert(T_CART_INVENTORY, {str_id, stritem_id, struser_id, stra});
    }
    dbtask->Insert(request);

    this->close();
}
