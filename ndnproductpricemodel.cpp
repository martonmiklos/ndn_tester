#include "ndnproductpricemodel.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

NDNProductPriceModel::NDNProductPriceModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void NDNProductPriceModel::selectPrices(QString productCode, bool validOnly)
{
    QSqlQuery query;
    if (validOnly) {
        query.prepare("SELECT Price || ' ' || Currency AS Price, "
                      "ValidFrom, ValidTo "
                      "FROM ndn_productprices WHERE ProductCode = :ProductCode AND"
                      "(strftime('%s', Validto) > strftime('%s', 'now') OR Validto is null) "
                      "ORDER BY Price ASC");
    } else {
        query.prepare("SELECT Price || ' ' || Currency AS price, "
                      "ValidFrom, ValidTo "
                      "FROM ndn_productprices WHERE ProductCode =  :ProductCode "
                      "ORDER BY Price ASC");
    }
    query.bindValue(":ProductCode", productCode);
    query.exec();
    setQuery(query);
    setHeaderData(0, Qt::Horizontal, tr("Price"));
    setHeaderData(1, Qt::Horizontal, tr("Valid from"));
    setHeaderData(2, Qt::Horizontal, tr("Valid until"));
}


