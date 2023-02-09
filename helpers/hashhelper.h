#ifndef HASHHELPER_H
#define HASHHELPER_H

#include <QString>

class HashHelper
{
public:
    static QString FileHash(const QString& fileName);
    static QString QByteArrayHash(const QByteArray& str);
    static QString QStringHash(const QString& str);
};

#endif // HASHHELPER_H
