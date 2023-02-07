#ifndef MODELS_H
#define MODELS_H

#include "helpers/meta.h"
#include <QDateTime>
#include <QString>

namespace Model
{
    struct Device{
        QString address = "";
        bool connected = false;
        QString name = "";

        QString toString() const;
    };

    struct InsoleType{
        META
        static const QString CSV;
        static const QString CSV_header;
        int Id;
        QDateTime LastModified;
        QString Name;
        int InsoleGenderId;
        int InsoleAgeCategoryId;
        int InsoleSideId;
        qreal EUSize;
        QString GeometryCSV;
        int R;
        int VMax;
        int VMin;

        static QList<InsoleType_Model> ParseList(const QString& str);
        InsoleType_Model Parse("kani");
        static Meta<Model::InsoleType_Model> GetMeta();
    };
};

#endif // MODELS_H
