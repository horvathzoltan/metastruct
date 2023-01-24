#include "filehelper.h"
#include "logger.h"

QString FileHelper::load2(const QString& filename) {
    QFileInfo fi(filename);
    if(!fi.isAbsolute())
    {
        zInfo("load2:"+QStringLiteral("nem abszolut path: %1").arg(filename));
        return QString();
    }

    if(!fi.exists())
    {
        zInfo(QStringLiteral("a fájl nem létezik: %1").arg(filename));
        return QString();
    }

    QFile f(filename);
    QString e;

    if (f.open(QFile::ReadOnly | QFile::Text))  {
        e =  QTextStream(&f).readAll();
    }
    else{
        zInfo(QStringLiteral("A fájl nem nyitható meg: %1").arg(filename));
        e= QString();
    }
    return e;
}
