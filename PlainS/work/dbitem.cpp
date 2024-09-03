#include "dbitem.h"

DBItem::DBItem(DBTask *dbt, QObject *parent) : DBWork(dbt, parent) //конструктор
{

}

QString DBItem::Create(dbItem item, int g_id) //создать предмет
{
    int id = -1;
    QString error_text(this->CheckID(T_INVENTORY, SEQ_INVENTORY, id));
    if (error_text.isEmpty())
    {
        const QString str_id(QString::number(id));
        const QString strgame_id(QString::number(g_id));
        const QString strprice(QString::number(item.price, 'f', 2));
        const QString request(DBRequest::Insert(T_INVENTORY,
                                                {str_id,
                                                 strgame_id,
                                                 "'" + item.name + "'",
                                                 strprice,
                                                 "'" + item.description + "'"}));
        error_text = dbtask->Insert(request);
    }
    return error_text;
}
