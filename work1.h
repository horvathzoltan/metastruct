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

    enum StructFindStates:int {StructBegins=0,
                               NameBegins=1,
                               NameEnds=2,
                               BlockBegins=3,
                               BlockEnds=4};

    Result doWork(Params params);

    static int SkipQuotation(const QString& txt, int ix);

    struct FindStructR{
        FindStructR(int _ix);
        int ix;
        QString name;
        QString block;
    };

    struct FindFieldR{
        FindFieldR(int _ix);
        int ix;
        QString type;
        QString name;
    };

    static FindStructR FindStruct(const QString& txt, int ix);

    struct Struct{
        QString name;

        struct Field{
            QString type;
            QString name;
        };

        static Struct Parse(const QString& name, const QString& block);
    };

};

#endif // WORK1_H
