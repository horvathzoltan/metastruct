#include "work1.h"
#include "logger.h"
#include "nameof.h"
#include "helpers/filehelper.h"
#include "helpers/filenamehelper.h"

Work1::Params::Params(const QString& _inFile, const QString& _outFile){
    inFile = _inFile;
    outFile = _outFile;
}

auto Work1::Params::IsValid() -> bool
{
    QStringList err;

    if(!err.isEmpty()) zInfo(err)
    return err.isEmpty();
}

auto Work1::Result::ToString() -> QString
{
    if(state==Ok) return QStringLiteral("a: ")+QString::number(value);
    if(state==NoResult) return nameof(NoResult);
    if(state==NotCalculated) return nameof(NotCalculated);
    return QStringLiteral("unknown");
}

auto Work1::doWork(Params params) -> Result
{    
    zInfo("hutty");
    QString testFilePathName = FileNameHelper::GetTestFolderPath();

    QDir testFilePath(testFilePathName);
    QString testFileFullPathName = testFilePath.filePath("test1.txt");

    QString txt = FileHelper::load2(testFileFullPathName);

    int ix =0;
    int L = txt.length();

    while(ix<L){
        //auto& c = txt[ix];
        int ix2 = SkipQuotation(txt, ix);
        if(ix!=ix2) {ix = ix2;continue;}

        // L" "
        // u8" "
        // u" "
        // U" "
        // R" ( ) "

        ix2 = FindStruct(txt, ix);
        Struct s = Struct::Parse(txt);
        ix++;
    }

    return {Result::State::Ok, 55};
}

int Work1::SkipQuotation(const QString &txt, int ix)
{
    auto& c = txt[ix];
    if(c!='\"') return ix;
    int i = txt.indexOf('"', ix+1);
    if(i==-1) return ix;
    int L = i-ix+1;
    zInfo(L("q:")+txt.mid(ix, L));
    return i+1;
}


// struct aaa { valami {}{}{}  };
int Work1::FindStruct(const QString &txt, int ix)
{
    int i = txt.indexOf("struct", ix+1);
    if(i==-1) return ix;

    return i+1;
}


Work1::Struct Work1::Struct::Parse(const QString &txt)
{
    Struct a;
    return a;
}
