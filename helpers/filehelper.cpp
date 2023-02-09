#include "filehelper.h"
#include "logger.h"

QString FileHelper::Load(const QString& filename) {
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

bool FileHelper::Save(const QString& txt, const QString& fn, bool isAppend) {

    QFile f(fn);

    auto om = QIODevice::WriteOnly | QIODevice::Text; // openmode
    if(isAppend) om |= QIODevice::Append;

    if (!f.open(om)){
        zInfo(QStringLiteral("cannot write file (%1): %2").arg(f.errorString()).arg(fn));
        return false;
        }

    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QTextStream out(&f);
    //out.setCodec(QTextCodec::codecForName("UTF-8"));
    //out.setGenerateByteOrderMark(true);
    out << txt;//.toUtf8();
    f.close();
    return true;
}

