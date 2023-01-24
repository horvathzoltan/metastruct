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

        int ix2 = SkipQuotation(txt, ix);
        if(ix!=ix2) {ix = ix2;continue;}

        // L" "
        // u8" "
        // u" "
        // U" "
        // R" ( ) "

        //

        auto& c = txt[ix];
        if(c=='s')
        {
            FindStructR r = FindStruct(txt, ix);
            if(ix!=r.ix)
            {
                ix=r.ix;
                Struct s = Struct::Parse(r.name, r.block);
            }
        }
        //Struct s = Struct::Parse(txt);
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

Work1::FindStructR Work1::FindStruct(const QString &txt, int ix2)
{
    auto ok = txt.mid(ix2,6)==("struct");
    FindStructR r(ix2);
    if(!ok) return r;
    int L = txt.length();
    StructFindStates i = StructBegins;
    int level=0;

    while(r.ix<L)
    {
        auto& c = txt[r.ix];
        if(c.isSpace())
        {
            switch(i)
            {
            case StructBegins: i=NameBegins; break;//name következik
            case NameBegins: i=NameEnds; break;//name kész, blokk következik
            case BlockBegins: r.block+=c;break;// blokk belsejében szóköz
            default: break;
            }
        }
        else
        {
            switch(i)
            {
            case NameBegins:
            {
                if(c==';')
                {
                    i=BlockEnds; //nincs blokk;
                }
                else if(c=='{')
                {
                    i=BlockBegins; // a név blokkal végződik
                    level++;
                }
                else
                {
                    r.name+=c; //name++
                }
                break;
            }
            case NameEnds: // blokk következik;
            {
                if(c==';')
                {
                    if(level==0) i=BlockEnds; //nincs blokk;
                }
                else if(c=='{')
                {
                    level++;
                    i=BlockBegins;
                }                
                break;
            }
            case BlockBegins:
            {
                if(c=='{')
                {
                    level++;
                }
                else if(c=='}')
                {
                    level--; // blokk vége
                }

                if(level>0)
                {
                    r.block+=c; // blokk belsejében nem szóköz
                }
                else
                {
                    i=BlockEnds; // blokk vége megvan;
                }

                break;            
            }
            default: break;
            }
            if(i==BlockEnds) break; // megvan a vége, nem keresünk tovább
        }
        r.ix++;
    }
    //124, 29
    //int L2 = ix-ix2+1;
    //zInfo("s:"+txt.mid(ix2, L2));
    //zInfo("s:"+r.name+":"+r.block);
    return r;
}


/*
 * https://en.cppreference.com/w/c/language/struct
*/
Work1::Struct Work1::Struct::Parse(const QString& name, const QString& block)
{
    Struct a;
    bool ok=true;
    if(name.isEmpty()) ok=false;
    if(block.isEmpty()) ok=false;
    if(ok)
    {
        zInfo("parse:"+name+":"+block);
    }
    return a;
}

Work1::FindStructR::FindStructR(int _ix)
{
    ix = _ix;
}
