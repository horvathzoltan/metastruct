#ifndef MODELS_H
#define MODELS_H

#include "helpers/meta.h"
#include <QDateTime>
#include <QString>

namespace Model
{
namespace Mogyoro
{    
    QString a = "aaaa struct bbb"
    struct ccc; {grrrvavva}
    //struct bbb ; {uu}
    struct aaa { valami {} {}  {}  u};    
    /*struct ddd1; {uu}
    struct ddd2; {uu}
    struct ddd3; {uu}*/
    struct Device{
        META
        QString address = "a,bc", v2 = {"aaa"};
        // bool connected = false;
        QString name = R"(def)";
        QString abcg("hutty"), u="sss";
        QString name="a", b="u";
        int i,j,k = 88;
        QString toString() const;
    };

};

};
#endif // MODELS_H
