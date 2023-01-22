#include "work1.h"
#include "logger.h"
#include "nameof.h"

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
    return {Result::State::Ok, 55};
}

