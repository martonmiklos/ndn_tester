#include "databasehandler.h"

#include <QFile>

DataBaseHandler::DataBaseHandler(QObject *parent) :
    QObject(parent)
{

}

void DataBaseHandler::init()
{
    QString databasePath = "ndn_data.sqlite";
    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName(databasePath);
    bool ok = dataBase.open();
    if (!ok) {
        mErrorString = tr("Unable to open the database!\nError details: %1\nCode:%2")
                .arg(dataBase.lastError().text())
                .arg(dataBase.lastError().number());
        emit errorHappened();
    }
}

bool DataBaseHandler::addVATRate(NSMasterData::NDN__VATRate result)
{
    QSqlQuery query;
    query.prepare("INSERT INTO ndn_vatrates"
                  "(Active, CashRegisterVATCode, Code, Name, Percentage, Type, ValidFrom)"
                  " VALUES "
                  "(:Active, :CashRegisterVATCode, :Code, :Name, :Percentage, :Type, :ValidFrom)");
    query.bindValue(":Active", result.active());
    query.bindValue(":CashRegisterVATCode", result.cashRegisterVATCode().type());
    query.bindValue(":Code", result.code());
    query.bindValue(":Name", result.name());
    query.bindValue(":Percentage", result.percentage());
    query.bindValue(":Type", result.type().type());
    query.bindValue(":ValidFrom", result.validFrom());
    return executeQuery(query);
}


bool DataBaseHandler::addManufacturer(NSMasterData::NDN__Manufacturer result)
{
    QSqlQuery query;
    query.prepare("INSERT INTO ndn_manufacturers"
                  "(Code, Name)"
                  " VALUES "
                  "(:Code, :Name)");
    query.bindValue(":Code", result.code());
    query.bindValue(":Name", result.name());
    return executeQuery(query);
}


bool DataBaseHandler::addProductGroup(NSMasterData::NDN__ProductGroup result)
{
    QSqlQuery query;
    query.prepare("INSERT INTO ndn_productgroups"
                  "(Code, IsFixedPrice, Name)"
                  " VALUES "
                  "(:Code, :IsFixedPrice, :Name)");
    query.bindValue(":Code", result.code());
    query.bindValue(":IsFixedPrice", result.isFixedPrice());
    query.bindValue(":Name", result.name());
    return executeQuery(query);
}

bool DataBaseHandler::addProduct(NSMasterData::NDN__Product result)
{
    qWarning() << result.name();
    QSqlQuery query;
    query.prepare("INSERT INTO ndn_products"
                  "(BaseBarCode, BaseUnitOfMeasure, CashRegisterVATCode, Code, CustCommCode, ExciseCategory, LastModifiedOn, ManufacturerCode, Name, PackagingQuantity, PackagingUnitOfMeasure, ProductGroup, ShortName, ValidTo)"
                  " VALUES "
                  "(:BaseBarCode, :BaseUnitOfMeasure, :CashRegisterVATCode, :Code, :CustCommCode, :ExciseCategory, :LastModifiedOn, :ManufacturerCode, :Name, :PackagingQuantity, :PackagingUnitOfMeasure, :ProductGroup, :ShortName, :ValidTo)");
    query.bindValue(":BaseBarCode", result.baseBarCode());
    query.bindValue(":BaseUnitOfMeasure", result.baseUnitOfMeasure());
    query.bindValue(":CashRegisterVATCode", result.cashRegisterVATCode().type());
    query.bindValue(":Code", result.code());
    query.bindValue(":CustCommCode", result.custCommCode());
    query.bindValue(":ExciseCategory", result.exciseCategory().type());
    query.bindValue(":LastModifiedOn", result.lastModifiedOn());
    query.bindValue(":ManufacturerCode", result.manufacturerCode());
    query.bindValue(":Name", result.name());
    query.bindValue(":PackagingQuantity", result.packagingQuantity());
    query.bindValue(":PackagingUnitOfMeasure", result.packagingUnitOfMeasure());
    query.bindValue(":ProductGroup", result.productGroup());
    query.bindValue(":ShortName", result.shortName());
    query.bindValue(":ValidTo", result.validTo());
    return executeQuery(query);
}

bool DataBaseHandler::addProductPrice(NSMasterData::NDN__ProductPrice result)
{
    QSqlQuery query;
    query.prepare("INSERT INTO ndn_productprices"
                  "(Currency, LineNo, Price, ProductCode, ValidFrom, ValidTo)"
                  " VALUES "
                  "(:Currency, :LineNo, :Price, :ProductCode, :ValidFrom, :ValidTo)");
    query.bindValue(":Currency", result.currency());
    query.bindValue(":LineNo", result.lineNo());
    query.bindValue(":Price", result.price());
    query.bindValue(":ProductCode", result.productCode());
    query.bindValue(":ValidFrom", result.validFrom());
    query.bindValue(":ValidTo", result.validTo());
    return executeQuery(query);
}

bool DataBaseHandler::addProductPackagingUnit(NSMasterData::NDN__ProductPackagingUnit result)
{
    QSqlQuery query;
    query.prepare("INSERT INTO ndn_productpackagingunits"
                  "(BarCode, Multiplier, PackagingUnit, PackagingUnitCode, ProductCode)"
                  " VALUES "
                  "(:BarCode, :Multiplier, :PackagingUnit, :PackagingUnitCode, :ProductCode)");
    query.bindValue(":BarCode", result.barCode());
    query.bindValue(":Multiplier", result.multiplier());
    query.bindValue(":PackagingUnit", result.packagingUnit());
    query.bindValue(":PackagingUnitCode", result.packagingUnitCode());
    query.bindValue(":ProductCode", result.productCode());
    return executeQuery(query);
}


void DataBaseHandler::setErrorStringFromQuery(QSqlQuery &query)
{
    QString str = query.lastQuery();
    QMapIterator<QString, QVariant> it(query.boundValues());
    while (it.hasNext()) {
        it.next();
        if (it.value().type() == QVariant::String)
            str.replace(it.key(), "\""+it.value().toString()+"\"");
        else
            str.replace(it.key(), it.value().toString());
    }

    mErrorString = str+"\n";
    mErrorString.append(query.lastError().text());
    qWarning() << mErrorString;
}


bool DataBaseHandler::executeQuery(QSqlQuery &query)
{
    bool ret = query.exec();
    if (!ret) {
        setErrorStringFromQuery(query);
        emit errorHappened();
    }
    return ret;
}
