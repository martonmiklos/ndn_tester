#ifndef NDNPRODUCTPRICEMODEL_H
#define NDNPRODUCTPRICEMODEL_H

#include <QSqlQueryModel>

class NDNProductPriceModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit NDNProductPriceModel(QObject *parent = 0);
    void selectPrices(QString productCode, bool validOnly);
};

#endif // NDNPRODUCTPRICEMODEL_H
