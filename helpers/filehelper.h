#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QTextStream>

class FileHelper
{
public:
    static QString Load(const QString &filename);
    static bool Save(const QString& txt, const QString& fn, bool isAppend);
};

#endif // FILEHELPER_H
