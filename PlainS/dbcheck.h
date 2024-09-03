#ifndef DBCHECK_H
#define DBCHECK_H

#include <QObject>
#include <QLineEdit>

class DBCheck : public QObject
{
    Q_OBJECT
public:
    explicit DBCheck(QObject *parent = nullptr);

    enum errors
    {
        E_,
        E_EMPTY,
        E_UNICODE,
        E_SPACES,
        E_SIZE_4,
        E_SIZE_8
    };

    static QString Check(const QString text, errors &e, int length = -1);
    static QString Check(const QString text, int length = -1);
    static QString ErrorHandler(errors *e);

};
//Q_DECLARE_METATYPE(DBCheck::errors);
#endif // DBCHECK_H
