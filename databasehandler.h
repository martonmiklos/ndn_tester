#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "wsdl_MasterData.h"

class DataBaseHandler : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseHandler(QObject *parent = 0);
    void init();

    bool addVATRate(NSMasterData::__VATRate result);
    bool updateVATRate(NSMasterData::__VATRate result);

    bool addManufacturer(NSMasterData::__Manufacturer result);
    bool updateManufacturer(NSMasterData::__Manufacturer result);

    bool addProductGroup(NSMasterData::__ProductGroup result);
    bool updateProductGroup(NSMasterData::__ProductGroup result);

    bool addProduct(NSMasterData::__Product result);

    bool addProductPrice(NSMasterData::__ProductPrice result);
    bool addProductPackagingUnit(NSMasterData::__ProductPackagingUnit result);

    QString errorString() const {return mErrorString;}
private:
    QSqlDatabase dataBase;
    QString mErrorString;

    void setErrorStringFromQuery(QSqlQuery &query);
    bool executeQuery(QSqlQuery &query);
signals:
    void errorHappened();
public slots:
    
};

#endif // DATABASEHANDLER_H
