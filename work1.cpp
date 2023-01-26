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
                //Struct s = Struct::Parse(r.name, r.block);
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
    StructFindStates i = StructStarted;
    int level=0;
    QString line;

    while(r.ix<L)
    {
        auto& c = txt[r.ix];
        int ix2 = SkipQuotation(txt, r.ix);
        if(r.ix!=ix2) {r.ix = ix2;continue;}
        if(c.isSpace())
        {
            switch(i)
            {
            case StructStarted: i=NameStarted; break;//name következik
            case NameStarted: i=NameEnded; break;//name kész, blokk következik
            case BlockStarted: line+=c;break;// blokk belsejében szóköz
            default: break;
            }
        }
        else
        {
            switch(i)
            {
            case NameStarted:
            {
                if(c==';')
                {
                    i=BlockEnded; //nincs blokk;
                }
                else if(c=='{')
                {
                    i=BlockStarted; // a név blokkal végződik
                    level++;
                }
                else
                {
                    r._struct.name+=c; //name++
                }
                break;
            }
            case NameEnded: // blokk következik;
            {
                if(c==';')
                {
                    if(level==0) i=BlockEnded; //lezárva és nincs blokk;
                }
                else if(c=='{')
                {
                    level++;
                    i=BlockStarted;
                }                
                break;
            }
            case BlockStarted:
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
                    line+=c; // blokk belsejében nem szóköz
                    if(c==';') // megvan egy sor, ki kell értékelni
                    {
                        Field f = Field::Parse(line);
                        r._struct.fields.append(f);
                        line.clear();
                    }
                }
                else
                {
                    i=BlockEnded; // blokk vége megvan;
                }

                break;            
            }
            default: break;
            }
            if(i==BlockEnded) break; // megvan a vége, nem keresünk tovább
        }
        r.ix++;
    }
    //124, 29
    //int L2 = ix-ix2+1;
    //zInfo("s:"+txt.mid(ix2, L2));
    //zInfo("s:"+r.name+":"+r.block);
    return r;
}

Work1::Field Work1::Field::Parse(const QString &line)
{
    Field f;
    f.name=line.trimmed();
    zInfo("f:"+line);
    return f;
}

/*
 * https://en.cppreference.com/w/c/language/struct
*/
//Work1::Struct Work1::Struct::Parse(const QString& name, const QString& txt)
//{
//    Struct a;
//    FindFieldR r(5);
//    bool ok=true;
//    if(name.isEmpty()) ok=false;
//    if(txt.isEmpty()) ok=false;
//    int L = txt.length();
//    StructFindStates i = NameBegins;
//    int level=0;

//    if(ok)
//    {
//        zInfo("parse:"+name+":"+txt);

////        while(r.ix<L){

////            int ix2 = SkipQuotation(txt, r.ix);
////            if(r.ix!=ix2) {r.ix = ix2;continue;}

////            // L" "
////            // u8" "
////            // u" "
////            // U" "
////            // R" ( ) "

////            //

////            auto& c = txt[r.ix];

//////                FindFieldR f = FindField(txt, r.ix);
//////                if(r.ix!=f.ix)
//////                {
//////                    r.ix=f.ix;
//////                    Struct::Field s = Struct::Field::Parse(f.name, f.type);
//////                }

////            //Struct s = Struct::Parse(txt);
////            r.ix++;
////        }
//    }
//    return a;
//}

Work1::FindStructR::FindStructR(int _ix)
{
    ix = _ix;
}

//Work1::FindFieldR::FindFieldR(int _ix)
//{
//    ix = _ix;
//}


