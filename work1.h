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
                               NameStarted,
                               NameEnded,
                               BlockStarted,
                               BlockEnded};

    enum FieldFindStates:int {FieldStarted=0,
                               FTypeStarted,
                               FTypeEnded,
                               FNameStarted,
                               FNameEnded,
                               FValueStarted,
                               FValueEnded,
                               FieldEnded,
                             FieldError};

    enum CommentType:int {Line=0,MultiLine, None};
    enum QuotationType:int {Simple=0,L,u8,u,U,Raw, QNone};

    Result doWork(Params params);

    static int SkipQuotation(const QString& txt, int ix);
    static int SkipBlock(const QString& txt, int ix);
    static int SkipComment(const QString& txt, int ix);


    struct FindFieldR{
        FindFieldR(int _ix);
        int ix;
        QString type;
        QString name= R"(2222 )";
    };


    struct T1{
        //int;
        QString z1{"aa"};
        QString z2 = {"aa"};
        QString z3;
        int a, a2;
        qreal b, t;
        void rrr(int u);
    };

    struct Field{
        QStringList keywords;
        QString type;
        QString name;
        QString value;

        static QList<Field> Parse(const QString& line);
        QString ToString();
    };

    struct Struct{
        QString name;
        QList<Field> fields;

        QString ToString();
        QString ToMetaString(const QString& fqn);
        bool isMeta();
    };

    struct FindStructR{
        FindStructR(int _ix);
        int ix;
        Struct _struct;
    };

    static FindStructR FindStruct(const QString& txt, int ix);

    struct FindNamespaceR{
        FindNamespaceR(int _ix);
        int ix;
        QString _namespace;
    };

    class Blocks{
public:
        enum Type:int {Namespace, Class};
private:
        struct Block{
            Block(Type _type, const QString& _name);
            Type type;
            QString name;
            int level;
        };
private:
        QList<Block> blocks;
public:

        void Add(Type type, const QString& name);
        QString ToString(const QString& separator);
        void In();
        void Out();
    };

    static FindNamespaceR FindNamespace(const QString& txt, int ix);

    struct FindClassR{
        FindClassR(int _ix);
        int ix;
        QString _class;
    };

    static FindClassR FindClass(const QString& txt, int ix);

    static QString ProcessFile(const QString& fileName);
};

#endif // WORK1_H
