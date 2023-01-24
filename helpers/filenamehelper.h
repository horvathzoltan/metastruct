#ifndef FILENAMEHELPER_H
#define FILENAMEHELPER_H

#include <QDir>

#define STRINGIFY(msg) #msg
#define STRING(msg) QStringLiteral(STRINGIFY(msg))
#define L(msg) QStringLiteral(msg)

class FileNameHelper{
public:
    static QString GetTestFolderPath();
};

#endif // FILENAMEHELPER_H
