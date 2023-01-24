#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QTextStream>

class FileHelper
{
public:
    static QString load2(const QString &filename);
};

#endif // FILEHELPER_H
