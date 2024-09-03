#include "dbcheck.h"

DBCheck::DBCheck(QObject *parent) : QObject(parent) //конструктор
{

}

QString DBCheck::Check(const QString text, errors &e, int length) //проверка
{
    if (text.trimmed().isEmpty())
        e = E_EMPTY;
    else
    {
        foreach (QChar c, text)
        {
            if (c.unicode() > 127)
            {
                e = E_UNICODE;
                break;
            }
        }

        if (length == 4 && text.trimmed().count() < length)
            e = E_SIZE_4;
        if (length == 8 && text.trimmed().count() < length)
            e = E_SIZE_8;
    }

    return ErrorHandler(&e);
}

QString DBCheck::Check(const QString text, int length) //проверка
{
    QRegExp spaces("\\s");
    errors e(E_);
    if (!text.contains(spaces))
    {
        foreach (QChar c, text)
        {
            if (c.unicode() > 127)
            {
                e = E_UNICODE;
                break;
            }
        }
        if (length == 4 && text.trimmed().count() < length)
            e = E_SIZE_4;
        if (length == 8 && text.trimmed().count() < length)
            e = E_SIZE_8;
    }
    else
        e = E_SPACES;

    return ErrorHandler(&e);
}

QString DBCheck::ErrorHandler(errors *e) //обработчик ошибок
{
    QString error_text = QString();

    switch (*e)
    {
    case E_UNICODE: error_text = "Строка введена не на английском"; break;
    case E_SPACES: error_text = "Строка не должна содержать пробелов"; break;
    case E_SIZE_4: error_text = "Минимальная длина строки 4 символов"; break;
    case E_SIZE_8: error_text = "Минимальная длина строки 8 символов"; break;
    case E_EMPTY: error_text = "Строка пуста"; break;
    default: break;
    }

    return error_text;
}
