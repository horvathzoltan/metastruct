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

    enum StructFindStates:int {StructStarted=0,
                               NameStarted=1,
                               NameEnded=2,
                               BlockStarted=3,
                               BlockEnded=4};

    Result doWork(Params params);

    static int SkipQuotation(const QString& txt, int ix);



//    struct FindFieldR{
//        FindFieldR(int _ix);
//        int ix;
//        QString type;
//        QString name;
//    };


    struct Field{
        QString name;
        QString type;
        QString value;

        static Field Parse(const QString& line);
    };

    struct Struct{
        QString name;
        QList<Field> fields;
    };

    struct FindStructR{
        FindStructR(int _ix);
        int ix;
        Struct _struct;
    };

    static FindStructR FindStruct(const QString& txt, int ix);
};

#endif // WORK1_H
