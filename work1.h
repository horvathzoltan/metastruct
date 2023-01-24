#ifndef WORK1_H
#define WORK1_H

#include <QHostAddress>
#include <QString>
#include <QThread>

class Work1
{
public:
    struct Params{
        Params(const QString& _inFile, const QString& _outFile);
        QString inFile;
        QString outFile;

        bool IsValid();
    };

    struct Result{
        enum State:int {NotCalculated = -3, NoResult=-1, Ok=1};
        State state;
        int value;

        QString ToString();
    };

    Result doWork(Params params);

    static int SkipQuotation(const QString& txt, int ix);

    static int FindStruct(const QString& txt, int ix);

    struct Struct{
        QString name;

        static Struct Parse(const QString& txt);
    };

};

#endif // WORK1_H
