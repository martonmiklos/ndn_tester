#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "gen_src/MasterData.h"

class DataBaseHandler : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseHandler(QObject *parent = nullptr);
    void init();

    bool addVATRate(NSMasterData::NDN__VATRate result);
    bool updateVATRate(NSMasterData::NDN__VATRate result);

    bool addManufacturer(NSMasterData::NDN__Manufacturer result);
    bool updateManufacturer(NSMasterData::NDN__Manufacturer result);

    bool addProductGroup(NSMasterData::NDN__ProductGroup result);
    bool updateProductGroup(NSMasterData::NDN__ProductGroup result);

    bool addProduct(NSMasterData::NDN__Product result);

    bool addProductPrice(NSMasterData::NDN__ProductPrice result);
    bool addProductPackagingUnit(NSMasterData::NDN__ProductPackagingUnit result);

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
