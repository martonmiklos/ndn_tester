#ifndef NDNENUMCONVERTER_H
#define NDNENUMCONVERTER_H

#include <QObject>

class NDNEnumConverter : public QObject
{
    Q_OBJECT
public:
    explicit NDNEnumConverter(QObject *parent = 0);
    static QString exciseCategoryName(int exciseCategory)
    {
        switch (exciseCategory) {
        case 0:
            return tr("Tobacco");
        case 1:
            return tr("Alcohol");
        }
        return tr("Other");
    }

signals:

public slots:

};

#endif // NDNENUMCONVERTER_H
