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
    QString testFilePathName = params.inFile.isEmpty()?FileNameHelper::GetTestFolderPath():params.inFile;

    QDir testFilePath(testFilePathName);
    if(! testFilePath.exists()) return {Result::State::NoResult, 1};

    QStringList files = testFilePath.entryList({"*.h"},QDir::Files);

    for(auto&file:files)
    {
        QString ffn = testFilePath.filePath(file);
        QString txt = FileHelper::Load(ffn);
        QFileInfo fi(file);
        QString fileName2 = fi.completeBaseName();
        if(fileName2.endsWith("_meta")) continue;

        QString txt_out = ProcessFile(txt);
        if(!txt.isEmpty())
        {
            fileName2 += "_meta."+fi.completeSuffix();
            zInfo("meta: "+fileName2);
            QString ffn2 = testFilePath.filePath(fileName2);
            FileHelper::Save(txt_out, ffn2, false);
        }
    }
    return {Result::State::Ok, 55};
}

QString Work1::ProcessFile(const QString& txt)
{
    if(txt.isEmpty()) return {};

    QString out_txt;

    int ix =0;
    int L = txt.length();

    Blocks blocks;

    while(ix<L){
        int ix2 = SkipQuotation(txt, ix);
        if(ix!=ix2) {ix = ix2;continue;}
        ix2 = SkipComment(txt, ix);
        if(ix!=ix2) {ix = ix2;continue;}

        auto& c = txt[ix];
        if(c=='s')
        {
            FindStructR r = FindStruct(txt, ix);
            if(ix!=r.ix)
            {
                ix=r.ix;

                QString blocknames = blocks.ToString("_");
                QString a = r._struct.ToMetaString(blocknames);
                //zInfo(a);
                if(!out_txt.isEmpty()) out_txt+='\n';
                out_txt+=a;
            }
        }
        else if(c=='n')
        {
            FindNamespaceR r = FindNamespace(txt, ix);
            if(ix!=r.ix)
            {
                blocks.Add(Blocks::Type::Namespace, r._namespace);
            }
        }
        else if(c=='c')
        {
            FindClassR r = FindClass(txt, ix);
            if(ix!=r.ix)
            {
                blocks.Add(Blocks::Type::Class, r._class);
            }
        }
        else if(c=='{'){
            blocks.In();
        }
        else if(c=='}'){
            blocks.Out();
        }
        ix++;
    }
    return out_txt;
}

// L" "
// u8" "
// u" "
// U" "
// R"( )"

int Work1::SkipQuotation(const QString &txt, int ix)
{
    QuotationType t;
    auto& c = txt[ix];
    if(c=='"')
    {
        auto& c1 = txt[ix-1];
        if(c1=='L'){
            t = L;
        } else if(c1=='u'){
            t=u;
        } else if(c1=='U'){
            t=U;
        } else if(c1=='R'){
            auto& c2 = txt[ix+1];
            if(c2=='('){
                t=Raw;
            } else{
                t=QNone;
            }
        } else if(c1=='8'){
            auto& c2 = txt[ix-2];
            if(c2=='u'){
                t = u8;
            } else {
                t = QNone;
            }
        } else {
        t = QNone;
        }
    } else{
        t = QNone;
    }

    switch(t){
    case L:
    case u8:
    case u:
    case U:
    case Simple:{
        int i = txt.indexOf('"', ix+1);
        if(i==-1) return ix;
        return i+1;
    }
    case Raw:{
        int i = txt.indexOf(")\"", ix+1);
        if(i==-1) return ix;
        return i+2;
    }
    case QNone: return ix;
    }
}

int Work1::SkipBlock(const QString &txt, int ix2)
{
    int L = txt.length();
    int ix=ix2;
    int l=0;
    while(ix<L)
    {
        auto& c = txt[ix];
        if(c=='{') // ha nem idézőjelben van!
        {
            l++;
        }
        else if(c=='}')
        {
            l--;
            if(l==0) break;
        }
        else if(c=='\"')
        {
            int ix2 = SkipQuotation(txt, ix);
            if(ix!=ix2)
            {
                QString v = txt.mid(ix, ix2-ix);
                ix=ix2;
                continue;
            }
            ix2 = SkipComment(txt, ix);
            if(ix!=ix2)
            {
                QString v = txt.mid(ix, ix2-ix);
                ix=ix2;
                continue;
            }
        }
        ix++;
    }
    //int L = ix-ix2+1;
    //zInfo(L("b:")+txt.mid(ix2, L2));
    return ix+1; // a találat mögötti, mivel átugrottuk
}


int Work1::SkipComment(const QString &txt, int ix)
{
    CommentType t;
    auto& c0 = txt[ix];

    if(c0=='/'){
        auto& c1 = txt[ix+1];
        if(c1=='/') {
            t = Line;
        } else if(c1=='*'){
            t = MultiLine;
        } else {
            t=None;
        }
    } else {
        t=None;
    }

    switch(t){
    case Line:{
        int i = txt.indexOf('\n', ix+2);
        if(i==-1) return ix;
        return i+1;// a találat mögötti, mivel átugrottuk
    }
    case MultiLine:{
        int i = txt.indexOf("*/", ix+2);
        if(i==-1) return ix;
        return i+1;// a találat mögötti, mivel átugrottuk
    }
    case None: return ix;
    }
}

Work1::FindNamespaceR Work1::FindNamespace(const QString &txt, int ix2)
{
    QString k = QStringLiteral("namespace");
    auto ok = txt.mid(ix2,k.length())==k;

    FindNamespaceR r(ix2);

    if(!ok) return r;
    int L = txt.length();
    StructFindStates i = StructStarted;
    int level=0;
    QString line;

    while(r.ix<L)
    {
        auto& c = txt[r.ix];

        if(c.isSpace())
        {
            switch(i)
            {
            case StructStarted: i=NameStarted; break;//name következik
            case NameStarted: i=NameEnded; break;//name kész, blokk következik
            //case BlockStarted: line+=c;break;// blokk belsejében szóköz
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
                    r._namespace+=c; //name++
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

            default: break;
            }
            if(i==BlockStarted) break; // megvan a vége, nem keresünk tovább
        }
        r.ix++;
    }
    return r;
}

Work1::FindClassR Work1::FindClass(const QString &txt, int ix2)
{
    QString k = QStringLiteral("class");
    auto ok = txt.mid(ix2,k.length())==k;

    FindClassR r(ix2);

    if(!ok) return r;
    int L = txt.length();
    StructFindStates i = StructStarted;
    int level=0;
    QString line;

    while(r.ix<L)
    {
        auto& c = txt[r.ix];

        if(c.isSpace())
        {
            switch(i)
            {
            case StructStarted: i=NameStarted; break;//name következik
            case NameStarted: i=NameEnded; break;//name kész, blokk következik
            //case BlockStarted: line+=c;break;// blokk belsejében szóköz
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
                    r._class+=c; //name++
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

            default: break;
            }
            if(i==BlockStarted) break; // megvan a vége, nem keresünk tovább
        }
        r.ix++;
    }
    return r;
}

Work1::FindStructR Work1::FindStruct(const QString &txt, int ix2)
{
    QString k = QStringLiteral("struct");
    auto ok = txt.mid(ix2,k.length())==k;

    FindStructR r(ix2);

    if(!ok) return r;
    int L = txt.length();
    StructFindStates i = StructStarted;
    int level=0;
    QString line;

    while(r.ix<L)
    {
        auto& c = txt[r.ix];
//        int ix2 = SkipComment(txt, r.ix);
//        if(r.ix!=ix2) {r.ix = ix2;continue;}
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
                        QList<Field> f = Field::Parse(line);
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
    //zInfo("s:"+r._struct.ToString());
    return r;
}

QList<Work1::Field> Work1::Field::Parse(const QString &line)
{
    QList<Field> fields;
    //f.name=line.trimmed();
    //zInfo("l:"+line);
    int L = line.length();
    int ix = 0;
    FieldFindStates i = FieldStarted;

    Field f;
    while(ix<L)
    {
        auto&c = line[ix];        
        if(c.isSpace())
        {
            switch(i)
            {
            case FTypeStarted: i=FTypeEnded; break;// type kész
            case FNameStarted: i=FNameEnded; break;// name kész
            //case FValueStarted: i=FValueEnded; break;// name kész
            default: break;
            }
        }
        else
        {
            if(c==';')
            {
                if(i!=FieldError)
                    fields.append(f);
                f.name.clear();
                f.value.clear();
                i=FieldEnded; //nincs field;
            }
            if(c==',')
            {
                if(i!=FieldError)
                    fields.append(f);
                f.name.clear();
                f.value.clear();
                i=FTypeEnded; //új érték lesz field;
            }
            else
            {
                switch(i)
                {
                case FieldStarted: // type kezdőbetű
                {
                    if(c.isLetter())
                    {
                        i = FTypeStarted;
                        f.type+=c; //type++
                    }
                    else
                    {
                        i=FieldError; //nincs field;
                    }
                    break;
                }
                case FTypeStarted:
                {
                    f.type+=c; //type++
                    break;
                }
                case FTypeEnded: // name kezdőbetű
                {
                    if(c.isLetter())
                    {
                        i = FNameStarted;
                        f.name+=c; //type++
                    }
                    break;
                }
                case FNameStarted: // name továbbiak
                {                    
                    if(c=='='){
                        i = FValueStarted;
                    }
                    else if(c=='(')
                    {
                        i=FieldError;
                    }
                    else
                    {
                        f.name+=c; //type++
                    }
                    break;
                }

                case FNameEnded: // name kezdőbetű
                {
                    i = FValueStarted;
                    if(c!='=')
                    {
                        f.value+=c; //type++
                    }

                    break;
                }
                case FValueStarted:
                {
                    if(c=='\"')
                    {
                        int ix2 = SkipQuotation(line, ix);
                        if(ix!=ix2){
                            QString v = line.mid(ix, ix2-ix);
                            f.value += v;
                            ix=ix2;
                            continue;
                        }
                    }
                    else if(c=='{')
                    {
                        int ix2 = SkipBlock(line, ix);
                        if(ix!=ix2){
                            QString v = line.mid(ix, ix2-ix);
                            f.value += v;
                            ix=ix2;
                            continue;
                        }
                    }
                    // nem lehet benne zárójel
                    // kommentet is át kell ugrani
                    else
                    {
                        if(c!='=')
                        {
                            f.value+=c;
                        }
                    }
                    break;
                }
                default: break;
                }
            }
            //if(i==FieldEnded) break; // megvan a vége, nem keresünk tovább
//            if(i==FieldError) // megvan a vége, nem keresünk tovább
//            {
//                f.name.clear();
//                f.value.clear();
//            }
        }
        ix++;
    }
//    for(auto&r:fields)
//    {
//        zInfo("f:"+r.type+":"+r.name+"="+r.value);
//    }
    return fields;
}

QString Work1::Field::ToString()
{
    QString s = "f:"+type+":"+name;
    if(!value.isEmpty()) s+= "="+value;
    return s;
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

Work1::FindNamespaceR::FindNamespaceR(int _ix)
{
    ix = _ix;
}



QString Work1::Struct::ToString()
{
    QString s = "s:"+name;
    if(fields.isEmpty()) return s;

    QString fs;
    for(auto&f:fields)
    {
        if(!fs.isEmpty())fs+='\n';
        fs+=f.ToString();
    }
    return s+"{\n"+fs+"}\n";
}

QString Work1::Struct::ToMetaString(const QString& fqn){

    QString fullName = fqn.isEmpty()?name:fqn+"_"+name;
    QString fullName2 = fqn.isEmpty()?name:fqn+"::"+name;
    QString a00 = QStringLiteral(R"(#define META_%1(m) %2 r; \)"); //head1
    QString a01 = QStringLiteral(R"(Meta<%1> m(&r); \)"); //head2
    QString a1 = QStringLiteral(R"(m.AddRow(%1,&r.%2);)"); // row

    QString macro = a00.arg(fullName, fullName2)+"\n"+a01.arg(fullName2);
    for(auto&field:fields)
    {
        if(!macro.isEmpty()) macro+=QStringLiteral(" \\\n");
        macro += a1.arg(field.type,field.name);
    }
    return macro;
}


void Work1::Blocks::Add(Type type, const QString &name)
{
    Block block(type,name);
    blocks.append(block);
}

QString Work1::Blocks::ToString(const QString& separator)
{
    QString b;
    for(auto&block:blocks){
        if(!b.isEmpty()) b+=separator;
        b+=block.name;
    }
    return b;
}

void Work1::Blocks::In()
{
    auto&b = blocks.last();
    b.level++;
}

void Work1::Blocks::Out()
{
    auto&b = blocks.last();
    b.level--;
    if(b.level==0)
    {
        blocks.removeLast();
    }
}

Work1::Blocks::Block::Block(Type _type, const QString &_name)
{
    type = _type;
    name = _name;
}

Work1::FindClassR::FindClassR(int _ix)
{
    ix = _ix;
}
