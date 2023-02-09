#include "hashhelper.h"
#include <QCryptographicHash>
#include <QFile>


QString HashHelper::FileHash(const QString& fileName)
{
    QFile file(fileName);
    if(!file.exists()) return {};
    QCryptographicHash hash(QCryptographicHash::Md5);
    if (file.open(QFile::ReadOnly))
    {
        hash.addData(&file);
    }
    auto r = hash.result().toHex();
    return r;
}

QString HashHelper::QByteArrayHash(const QByteArray& str)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(str);
    auto r = hash.result().toHex();
    return r;
}

QString HashHelper::QStringHash(const QString& str)
{
    return QByteArrayHash(str.toLocal8Bit());
}
